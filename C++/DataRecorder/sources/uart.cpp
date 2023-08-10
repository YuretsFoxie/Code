#include "uart.h"
#include "dataModel.h"

// Public Methods

void UART::transmit(const string& str)
{
    connect();

    if(!checkConnection())
        return;
	
	LPCVOID data = str.c_str();
	DWORD length = str.length();
    DWORD bytesWritten = 0;
    OVERLAPPED overlapped = {0};
	
    ::PurgeComm(portHandle, PURGE_RXCLEAR);
    ::WriteFile(portHandle, data, length, &bytesWritten, &overlapped);
    ::GetOverlappedResult(portHandle, &overlapped, &bytesWritten, TRUE);
	
    if (::GetLastError())
        sendMessage("error: transmission failed");
}

void UART::startReceiving()
{
    connect();
	
    if(!checkConnection())
        return;
	
    threadHandle = ::CreateThread(NULL, 0, staticReceive, (void*) this, 0, NULL);
	
    if(::GetLastError())
        sendMessage("error: thread creation failed");
	
    else
        isReceiving = true;
}

void UART::stopReceiving()
{
    ::TerminateThread(threadHandle, 0);
	
    if(::GetLastError())
        sendMessage("error: thread termination failed");
	
    else
        isReceiving = false;
	
	::CloseHandle(threadHandle);
}

// Private Methods

void UART::connect()
{
    if (!isConnected)
    {
        findPort();
        initPort();
    }
}

void UART::disconnect()
{
    stopReceiving();
    deinitPort();
}

void UART::findPort()
{
    for(unsigned int i = 0; i < 10; i++)
    {
        char name[] = "\\\\.\\COM ";
        name[7] = 48 + i;

        HANDLE hFile = CreateFile(
                        name,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_OVERLAPPED,
                        NULL);

        if (hFile != INVALID_HANDLE_VALUE)
        {
            portHandle = hFile;
            return;
        }
    }

    portHandle = INVALID_HANDLE_VALUE;
}

void UART::initPort()
{
    if(!checkPort())
        return;

    ::SetCommMask(portHandle, EV_RXCHAR);
 	::SetupComm(portHandle, 32, 32);

 	COMMTIMEOUTS commTimeOuts;
 	commTimeOuts.ReadIntervalTimeout = MAXDWORD;
 	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
 	commTimeOuts.ReadTotalTimeoutConstant = timeout;
 	commTimeOuts.WriteTotalTimeoutMultiplier = 0;
 	commTimeOuts.WriteTotalTimeoutConstant = timeout;

 	if(!::SetCommTimeouts(portHandle, &commTimeOuts)) {
 		deinitPort();
        return;
 	}

 	DCB comDCM = {0};

 	comDCM.DCBlength = sizeof(DCB);
 	::GetCommState(portHandle, &comDCM);

 	comDCM.BaudRate = DWORD(baudrate);
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

 	if(!::SetCommState(portHandle, &comDCM)) {
 		deinitPort();
        return;
 	}

 	isConnected = true;
}

void UART::deinitPort()
{
    ::CloseHandle(portHandle);

    portHandle = INVALID_HANDLE_VALUE;
    isConnected = false;
}

void UART::sendMessage(const string& text)
{
	::SendMessage(window, WM_USER, 0, (LPARAM) text.c_str());	
}

void UART::saveData()
{
	for (int i = 0; i < bytesRead; i++)
		DataModel::shared().push( convertTwosComplementToInt(bitset<8>(buffer[i])) );
}

bool UART::checkPort()
{
    if(portHandle != INVALID_HANDLE_VALUE)
        return true;

    isConnected = false;
    return false;
}

bool UART::checkConnection()
{
    if(isConnected)
        return true;
	
    sendMessage("port is not connected");
    return false;
}

DWORD WINAPI UART::staticReceive(void* Param)
{
    UART* This = (UART*) Param;
    return This->receive();
}

DWORD UART::receive()
{
	fill_n(buffer, 64, 0);
	bytesRead = 0;
	
    unsigned long state = 0;
	
    DWORD errors = 0;
    DWORD bytesNumber = 0;
    COMSTAT stat = {0};
    OVERLAPPED overlapped = {0};
	
    overlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
	
    while(true)
    {
        if(!::WaitCommEvent(portHandle, &state, &overlapped))
            if (::GetLastError() == ERROR_IO_PENDING)
                ::WaitForSingleObject(overlapped.hEvent, INFINITE);

        ::ClearCommError(portHandle, &errors, &stat);
		
        bytesNumber = stat.cbInQue;
		memset(buffer, 0, sizeof(buffer));
		
        if(bytesNumber > 0)
            if(::ReadFile(portHandle, buffer, bytesNumber, &bytesRead, &overlapped))
                (bytesNumber <= 64) ? saveData() : sendMessage("error: more bytes received");
    }
}

int UART::convertTwosComplementToInt(const bitset<8> byte)
{
	return byte[7] ? -bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
}
