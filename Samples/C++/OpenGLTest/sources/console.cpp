#include <algorithm>
#include <iostream>
#include "console.h"

// Public Methods

void Console::read(WPARAM wParam)
{
    switch (wParam)
    {
    case VK_BACK:   onBack(); break;
    case VK_RETURN: onEnter(); break;
    case VK_PRIOR:  onPageUp(); break;
    case VK_NEXT:   onPageDown(); break;
    default:        onCharachter(wParam);
    }
}

void Console::print(const unsigned char* value)
{
    print(reinterpret_cast<char const*>(value));
}

void Console::print(const string& value)
{
    hideCursor();

    if(!buffer.empty())
    {
        nextString();
        buffer.clear();
    }

    printString(value);
    nextString();
    showCursor();
}

// Private Methods

Console::Console(): commandNumber(-1), file(File("data/Console History.txt")), history(file.load())
{
    commands["test"] = &onCommandTest;
    commands["c"]    = &onCommandClear;

    showCursor();
}

// Key Handlers

void Console::onBack()
{
    buffer = buffer.substr(0, buffer.size() - 1);

    hideCursor();
    printBuffer();
}

void Console::onEnter()
{
    if(buffer.empty())
        return;

    hideCursor();
    nextString();
    showCursor();

    string temp;
    stringstream ss(buffer);

    while(ss >> temp)
        command.push_back(temp);

    temp = buffer;
    buffer.clear();

    if(commands.find(command[0]) == commands.end())
        print("error: unknown command");

    else
        (this->*commands[command[0]])() ? save(temp) : print("error: invalid arguments");

    command.clear();
}

void Console::onPageUp()
{
    if (history.empty())
        return;

    if(commandNumber == -1)
        commandNumber = history.size() - 1;

    buffer = history[commandNumber--];

    moveToStart();
    printString("                                        ");
    printBuffer();
}

void Console::onPageDown()
{
    if (history.empty())
        return;

    if (commandNumber == -1)
        commandNumber = 0;

    buffer = history[commandNumber++];

    if (commandNumber == (int)history.size())
        commandNumber = 0;

    moveToStart();
    printString("                                        ");
    printBuffer();
}

void Console::onCharachter(WPARAM wParam)
{
    char c = char(wParam);

    if(isLetter(c))
        buffer += char(c + 32);

    if(isDigit(c))
        buffer += c;

    if(c == ' ' && !buffer.empty() && buffer[buffer.size() - 1] != ' ')
        buffer += c;

    if(wParam == 189)
        buffer += '-';

    if(wParam == 190)
        buffer += '.';

    printBuffer();
}

// Print Helpers

inline void Console::printString(const string& str)
{
    cout << str;
}

inline void Console::printBuffer()
{
    cout << "\r" + buffer + "_";
}

inline void Console::showCursor()
{
    cout << "_";
}

inline void Console::hideCursor()
{
    cout << "\b \b";
}

inline void Console::nextString()
{
    cout << endl;
}

inline void Console::moveToStart()
{
    cout << "\r";
}

void Console::hideSystemCursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

// Saving the buffer

void Console::save(const string& buffer)
{
    history.erase(remove(history.begin(), history.end(), buffer), history.end());
    history.push_back(buffer);
    file.save(history);
}

// Validation

bool Console::isInt(const string& str)
{
    for(unsigned int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        if( !(isDigit(c) || (c == '-' && i == 0)) )
            return false;
    }

    return true;
}

bool Console::isFloat(const string& str)
{
    for(unsigned int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        if( !(isDigit(c) || (c == '-' && i == 0) || (c == '.' && i != 0 && i != str.size() - 1 && isDigit(str[i - 1]))) )
            return false;
    }

    return true;
}

inline bool Console::isDigit(const char c)
{
    return c >= '0' && c <= '9';
}

inline bool Console::isLetter(const char c)
{
    return c >= 'A' && c <= 'Z';
}

// Console Commands

bool Console::onCommandTest() // the arguments should be the two floats
{
    if( command.size() == 3 && isFloat(command[1]) && isFloat(command[2]) )
    {
        print("onCommandTest(" + command[1] + ", " + command[2] + ")");
        return true;
    }

    return false;
}

bool Console::onCommandClear()
{
    system("cls");
    hideSystemCursor();
    showCursor();
    return true;
}
