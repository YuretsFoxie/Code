#include "comports.h"
#include "controller.h"

// Public Functions

vector<string> COMPorts::findPorts()
{
	vector<string> result;
	
	for (int i = 0; i <= 255; i++)
	{
		string comport = "COM" + to_string(i);
		WCHAR targetPath[64];
		
		if (::QueryDosDevice(comport.c_str(), (LPSTR)targetPath, 64))
			result.push_back(comport);
	}
	
	return result;
}

void COMPorts::selectPort(const string& port)
{
	string name = "\\\\.\\" + port;
	
	HANDLE handle = CreateFile(
                        name.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_OVERLAPPED,
                        NULL);
		
    if (handle != INVALID_HANDLE_VALUE)
	{
		Controller::shared().showMessage(name + " selected");
		portHandle = handle;
		initPort();
	}
	else
	{
		Controller::shared().showMessage("error in selectPort: " + to_string(::GetLastError()));
		portHandle = INVALID_HANDLE_VALUE;
	}
}

void COMPorts::disconnect()
{
	stopReceiving();
	deinitPort();
}

void COMPorts::transmit(const string& data)
{
	if (portHandle == INVALID_HANDLE_VALUE)
	{
		Controller::shared().showMessage("error in transmit (1): port is not connected");
		return;
	}
	
	LPCVOID buffer = data.c_str();
	DWORD length = data.length();
    DWORD bytesWritten = 0;
    OVERLAPPED overlapped = {0};
	
    ::PurgeComm(portHandle, PURGE_RXCLEAR);
	DWORD code = ::GetLastError();
    if (code)
        Controller::shared().showMessage("error in transmit (2): " + to_string(code));
	
    ::WriteFile(portHandle, buffer, length, &bytesWritten, &overlapped);
	code = ::GetLastError();
    if (code)
        Controller::shared().showMessage("error in transmit (3): " + to_string(code));
	
    ::GetOverlappedResult(portHandle, &overlapped, &bytesWritten, TRUE);
	code = ::GetLastError();
    if (code)
        Controller::shared().showMessage("error in transmit (4): " + to_string(code));
}

void COMPorts::startReceiving()
{
	if (portHandle == INVALID_HANDLE_VALUE)
	{
		Controller::shared().showMessage("error in startReceiving (1): port is not connected");
		return;
	}
	
    threadHandle = ::CreateThread(NULL, 0, staticReceive, (void*) this, 0, NULL);
	
	DWORD code = ::GetLastError();
    if(code)
        Controller::shared().showMessage("error in startReceiving (2): " + to_string(code));
    else
        isReceiving = true;
}

void COMPorts::stopReceiving()
{
	if (!isReceiving)
		return;
	
	::TerminateThread(threadHandle, 0);
	DWORD code = ::GetLastError();
	
	if(code)
		Controller::shared().showMessage("error in stopReceiving (1): " + to_string(code));
	else
		isReceiving = false;
	
	::CloseHandle(threadHandle);
	code = ::GetLastError();
	
	if(code)
		Controller::shared().showMessage("error in stopReceiving (2): " + to_string(code));
}

// Private Functions

COMPorts::~COMPorts()
{
	disconnect();
}

void COMPorts::initPort()
{
    ::SetCommMask(portHandle, EV_RXCHAR);
 	::SetupComm(portHandle, 32, 32);

 	COMMTIMEOUTS commTimeOuts;
 	commTimeOuts.ReadIntervalTimeout = MAXDWORD;
 	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
 	commTimeOuts.ReadTotalTimeoutConstant = timeout;
 	commTimeOuts.WriteTotalTimeoutMultiplier = 0;
 	commTimeOuts.WriteTotalTimeoutConstant = timeout;
	
 	if(!::SetCommTimeouts(portHandle, &commTimeOuts))
	{
		Controller::shared().showMessage("error in initPort (1): " + to_string(::GetLastError()));
 		deinitPort();
        return;
 	}
	
 	DCB comDCM = {0};
	
 	comDCM.DCBlength = sizeof(DCB);
	
 	if(!::GetCommState(portHandle, &comDCM))
	{
		Controller::shared().showMessage("error in initPort (2): " + to_string(::GetLastError()));
		deinitPort();
		return;
	}
	
 	comDCM.BaudRate = baudrate;
 	comDCM.ByteSize = 8;
	comDCM.Parity = NOPARITY;
 	comDCM.StopBits = ONESTOPBIT;
	comDCM.fAbortOnError = TRUE;
 	comDCM.fDtrControl = DTR_CONTROL_DISABLE;
 	comDCM.fRtsControl = RTS_CONTROL_DISABLE;
 	comDCM.fBinary = TRUE;
 	comDCM.fParity = FALSE;
	comDCM.fInX = FALSE;
    comDCM.fOutX = FALSE;
 	comDCM.XonChar = 0;
 	comDCM.XoffChar = (unsigned char)0xFF;
 	comDCM.fErrorChar = FALSE;
 	comDCM.fNull = FALSE;
 	comDCM.fOutxCtsFlow = FALSE;
 	comDCM.fOutxDsrFlow = FALSE;
 	comDCM.XonLim = 32;
 	comDCM.XoffLim = 32;
	
 	if(!::SetCommState(portHandle, &comDCM))
	{
		Controller::shared().showMessage("error in initPort (3): " + to_string(::GetLastError()));
 		deinitPort();
        return;
 	}
	
	Controller::shared().showMessage("port initialized");
}

DWORD WINAPI COMPorts::staticReceive(void* Param)
{
    COMPorts* This = (COMPorts*) Param;
    return This->receive();
}

DWORD COMPorts::receive()
{
	fill_n(buffer, 64, 0);
	bytesRead = 0;
	DWORD errors = 0;
	DWORD bytesNumber = 0;
	COMSTAT stat = {0};
	OVERLAPPED overlapped = {0};
	
    overlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
	unsigned long state = 0;
	
    while(true)
    {
		if(!::WaitCommEvent(portHandle, &state, &overlapped))
		{
			DWORD code = ::GetLastError();
			if (code == ERROR_IO_PENDING)
				::WaitForSingleObject(overlapped.hEvent, INFINITE);
			else if (code)
				Controller::shared().showMessage("error in receive (1): " + to_string(code));
		}
		
		//=====
		
		// https://learn.microsoft.com/ru-ru/windows/win32/api/winbase/nf-winbase-clearcommerror
		// https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
		// https://learn.microsoft.com/ru-ru/windows/win32/api/winbase/ns-winbase-dcb
		
        ::ClearCommError(portHandle, &errors, &stat);
		
		if (errors & CE_BREAK)		Controller::shared().showMessage("error in receive (2): CE_BREAK");
		if (errors & CE_FRAME)		Controller::shared().showMessage("error in receive (2): CE_FRAME");
		if (errors & CE_OVERRUN)	Controller::shared().showMessage("error in receive (2): CE_OVERRUN");
		if (errors & CE_RXOVER)		Controller::shared().showMessage("error in receive (2): CE_RXOVER");
		if (errors & CE_RXPARITY)	Controller::shared().showMessage("error in receive (2): CE_RXPARITY");
		
		//=====
		
        bytesNumber = stat.cbInQue;
		memset(buffer, 0, sizeof(buffer));
		
		if(bytesNumber > 0)
		{
            if(::ReadFile(portHandle, buffer, bytesNumber, &bytesRead, &overlapped))
            {
				if (bytesNumber <= 64)
					saveData();
				else
					Controller::shared().showMessage("error in receive (3): more bytes received");
			}
			else
				Controller::shared().showMessage("error in receive (4): " + to_string(::GetLastError()));
		}
    }
}

void COMPorts::saveData()
{
	for (int i = 0; i < bytesRead; i++)
	{
		int data = convertTwosComplementToInt(bitset<8>(buffer[i]));
		Controller::shared().handleReceivedData(data);
	}
}

void COMPorts::deinitPort()
{
	::CloseHandle(portHandle);
	
	DWORD code = ::GetLastError();
    if(code)
        Controller::shared().showMessage("error in deinitPort (1): " + to_string(code));
	
	portHandle = INVALID_HANDLE_VALUE;
	Controller::shared().showMessage("port disconnected");
}

int COMPorts::convertTwosComplementToInt(const bitset<8> byte)
{
	return byte[7] ? -bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}
