#include "renderer.h"
#include "settingsModel.h"
#include "dataModel.h"

#include <string>

// Public Functions

void Renderer::onStart()
{
	greenPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}

void Renderer::draw(HDC& hdc, RECT& window)
{
	prepareDrawing(hdc, window);
	drawMarks();
	fillBufferWithVoltage();
	fillBufferWithTime();
	drawBuffer();
}

void Renderer::onStop()
{
	::DeleteObject(greenPen);
}

void Renderer::clear()
{
	buffer.clear();
	dx = 0;
}

// Private Functions

void Renderer::prepareDrawing(HDC& hdc, RECT& window)
{
	this->hdc = hdc;
	this->window = window;
	this->scaleX = window.right / SettingsModel::shared().getTime();
	this->halfHeight = window.bottom / 2.0;
	this->dy = (window.bottom - 20) / 256.0;
	
	::FillRect(hdc, &window, reinterpret_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
	::SelectObject(hdc, greenPen);
	::SetBkColor(hdc, RGB(0, 0, 0));
	::SetTextColor(hdc, RGB(0, 255, 0));
}

void Renderer::drawMarks()
{
	drawLine({0, (long int)halfHeight}, {window.right, (long int)halfHeight});
	
	for (int i = -128; i <= 128; i += 8)
		if (i)
		{
			int y = halfHeight - i * dy;
			string str = to_string(i / 100.0).substr(0, (i > 0 ? 4 : 5)) + " V";
			
			drawLine({70, y}, {80, y});
			::TextOut(hdc, 20, y - 8, str.c_str(), str.length());
		}
}

void Renderer::drawLine(POINT from, POINT to)
{
	::MoveToEx(hdc, from.x, from.y, NULL);
    ::LineTo(hdc, to.x, to.y);
}

void Renderer::fillBufferWithVoltage()
{
	vector<int> data = DataModel::shared().getData();
	int dataSize = data.size();
	
	for (int i = buffer.size() / 2; i < dataSize; i++)
	{
		int y = halfHeight - data[i] * dy;
		
		buffer.push_back({0, y});
		buffer.push_back({0, y});
	}
}

void Renderer::fillBufferWithTime()
{
	int bufferSize = buffer.size();
	
	if (bufferSize)
		buffer[0].x = window.right - dx;
	
	dx += SettingsModel::shared().getSpeed();
	
	for (int i = 2; i < bufferSize; i += 2)
		buffer[i].x = buffer[i - 2].x + scaleX;
	
	for (int i = 1; i < bufferSize; i += 2)
		buffer[i].x = buffer[i - 1].x + scaleX;
}

void Renderer::drawBuffer()
{
	::Polyline(hdc, &buffer[0], buffer.size());
}
