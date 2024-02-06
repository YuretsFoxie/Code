#ifndef GDIVIEW_H_INCLUDED
#define GDIVIEW_H_INCLUDED

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
//#include "console.h"

using namespace std;

class Console
{
public:
	Console() {}
	Console(const HDC& memDC): memDC(memDC) {}
	
	void print(const string& message)
	{
		split("> " + message);
		addLines();
	}
	
	void scrollUp()
	{
		
	}
	
	void scrollDown()
	{
		
	}
	
	void render()
	{
		// ::DrawText(memDC, buffer.c_str(), buffer.length(), &frame, DT_WORDBREAK | DT_EDITCONTROL);
	}
	
private:
	void split(const string& s)
	{
		istringstream stream(s); 
		string word;
		
		while (stream >> word)
			words.push_back(word);
	}
	
	void addLines()
	{
		string buffer = words[0] + " ";
		RECT rect = frame;
		int height = ::DrawText(memDC, buffer.c_str(), buffer.length(), &rect, DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		
		for (int i = 1; i < words.size(); i++)
		{
			string s = buffer + words[i] + " ";
			rect = frame;
			int newHeight = ::DrawText(memDC, s.c_str(), s.length(), &rect, DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
			
			if (height < newHeight)
			{
				lines.push_back(s);
				buffer = "";
			}
			else
				buffer = s;
			
			height = newHeight;
		}
		
		lines.push_back(buffer);
		words.clear();
	}
	
	vector<string> words;
	vector<string> lines;
	
	RECT frame = {10, 10, 100, 500};
	HDC memDC;
};



class Graph
{
public:
	Graph() {}
	Graph(const HDC& memDC): memDC(memDC) {}
	
	void render()
	{
		buffer = {{100, 100}, {100, 200}, {200, 200}, {100, 100}};
		::Polyline(memDC, &buffer[0], buffer.size());
	}
	
private:
	HDC memDC;
	vector<POINT> buffer;
};



class GDIView
{
public:	
	void onStart(HINSTANCE instance, HWND parent);
	void onStop();
	void paint();
	void print(const string& message);
	void scrollUp();
	void scrollDown();
	
private:
	void registerWindowClass();
	void createWindow();
	void setup();
	void addSubviews();
	
	static DWORD WINAPI staticDraw(void* Param);
	DWORD drawIfNeeded();
	void prepareDrawing();
	void draw();
	
	HINSTANCE hInstance;
	HWND parentWindow;
	HWND gdiWnd;
	RECT window;
	HDC hDC;
	HDC memDC;
	HBITMAP bitmap;
	HANDLE drawingThreadHandle;
	HPEN greenPen;
	
	Console console;
	Graph graph;
	
	bool isDrawing;
};

#endif // GDIVIEW_H_INCLUDED
