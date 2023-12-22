#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <windows.h>
#include <sstream>
#include <map>
#include "file.h"

class Console
{
public:
    static Console& shared()
    {
        static Console c;
        return c;
    }

    void read(WPARAM wParam);

    template <typename T>
    void print(const T value);
    void print(const unsigned char* value);
    void print(const string& value);

private:
    Console();
    ~Console() {}

// Key Handlers

    void onBack();
    void onEnter();
    void onPageUp();
    void onPageDown();
    void onCharachter(WPARAM wParam);

// Print Helpers

    void printString(const string& str);
    void printBuffer();
    void showCursor();
    void hideCursor();
    void nextString();
    void moveToStart();
    void hideSystemCursor();

// Saving the buffer

    void save(const string& buffer);

// Validation

    bool isInt(const string& str);
    bool isFloat(const string& str);
    bool isDigit(const char c);
    bool isLetter(const char c);

// Console Commands

    bool onCommandTest();
    bool onCommandClear();

// Variables

    int commandNumber;
    File file;
    string buffer;
    vector<string> command;
    vector<string> history;
    map<string, bool(Console::*)()> commands;
};



template <typename T>
void Console::print(const T value)
{
    ostringstream ss;
    ss << value;
    print(ss.str());
}

#endif // CONSOLE_H_INCLUDED
