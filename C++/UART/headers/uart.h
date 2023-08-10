#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#define WIN32_LEAN_AND_MEAN 1
#define WIN32_EXTRA_LEAN 1

#include <windows.h>
#include <string>

using namespace std;

class UART
{
public:
    static UART& shared()
    {
        static UART uart;
        return uart;
    }

    void init(const HWND mainWindow)
    {
        window = mainWindow;
    }

    bool getIsReceiving()
    {
        return isReceiving;
    }

    void transmit(const string& str);
    void startReceiving();
    void stopReceiving();

private:
    UART(): isConnected(false), isReceiving(false), timeout(1000), baudrate(250000) {} // 9600 250000

    ~UART()
    {
        disconnect();
    }

    void connect();
    void disconnect();

    void findPort();
    void initPort();
    void deinitPort();
    void sendMessage(const string& text, const bool isMessage = true);

    bool checkPort();
    bool checkConnection();

    static DWORD WINAPI staticReceive(void* Param);
    DWORD receive();

    // Variables

    bool isConnected;
    bool isReceiving;

    int timeout;
    int baudrate;

    HWND window;
    HANDLE portHandle;
    HANDLE threadHandle;
};

#endif // UART_H_INCLUDED
