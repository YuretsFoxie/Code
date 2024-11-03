#include <windows.h>
#include <GL/glew.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <iostream>
#include <bitset>
#include <stdexcept>
#include <fstream>
#include <string>



class OpenGLException: public std::runtime_error
{
public:
	OpenGLException(const std::string& message): std::runtime_error(message) {}
};



class SerialException: public std::runtime_error
{
public:
	SerialException(const std::string& message): std::runtime_error(message) {}
};



class ShaderException: public std::runtime_error
{
public:
	ShaderException(const std::string& message): std::runtime_error(message) {}
};



class Settings
{
public:
	Settings(const std::string& filePath)
	{
		std::ifstream configFile(filePath);
		if (!configFile.is_open())
		{
			std::ofstream newConfigFile(filePath);
			if (!newConfigFile.is_open())
				throw std::runtime_error("unable to create configuration file");
				
			newConfigFile << "serialPort: COM3\n";
			newConfigFile << "baudRate: 57600\n";
			newConfigFile.close();
			configFile.open(filePath);
		}
		
		std::string line;
		while (std::getline(configFile, line))
		{
			auto delimiterPos = line.find(": ");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 2);
			
			if (name == "serialPort")
				serialPort = value;
			else if (name == "baudRate")
				baudRate = std::stoi(value);
		}
	}
	
	std::string getSerialPort() const
	{
		return serialPort;
	}
	
	int getBaudRate() const
	{
		return baudRate;
	}
	
private:
	std::string serialPort;
	int baudRate;
};



class COMPort
{
public:
	COMPort(): handle(INVALID_HANDLE_VALUE) {}
	
   ~COMPort()
	{
		if (handle != INVALID_HANDLE_VALUE)
			::CloseHandle(handle);
	}
	
	void setup(const std::string& portName, int baudRate)
	{
		handle = ::CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (handle == INVALID_HANDLE_VALUE)
			throw SerialException("error opening serial port");
		
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		
		if (!::GetCommState(handle, &dcbSerialParams))
		{
			::CloseHandle(handle);
			throw SerialException("error getting serial port state");
		}
		
		dcbSerialParams.BaudRate = baudRate;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;
		
		if (!::SetCommState(handle, &dcbSerialParams))
		{
			::CloseHandle(handle);
			throw SerialException("error setting serial port state");
		}
		
		timeouts.ReadIntervalTimeout = 50;
		
		if (!::SetCommTimeouts(handle, &timeouts))
		{
			::CloseHandle(handle);
			throw SerialException("error setting timeouts");
		}
		
		char startCmd = '1';
		DWORD bytesWritten;
		
		if (!::WriteFile(handle, &startCmd, 1, &bytesWritten, NULL))
		{
			::CloseHandle(handle);
			throw SerialException("error writing to serial port");
		}
	}
	
	HANDLE getHandle() const
	{
		return handle;
	}
	
private:
	HANDLE handle;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };
};



class Shaders
{
public:
	Shaders(): program(0) {}
	
   ~Shaders()
	{
		if (program)
			glDeleteProgram(program);
	}
	
	void initialize()
	{
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, R"(
			#version 330 core
			layout (location = 0) in vec2 aPos;
			void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
		)");
		
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, R"(
			#version 330 core
			out vec4 FragColor;
			void main() { FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); }
		)");
		
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		
		if (!success)
		{
			GLint logLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> log(logLength);
			glGetProgramInfoLog(program, logLength, NULL, log.data());
			throw ShaderException("program linking failed: " + std::string(log.data()));
		}
	}
	
	GLuint getProgram() const
	{
		return program;
	}
	
private:
	GLuint compileShader(GLenum type, const char* source)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		
		if (!success)
		{
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<char> log(logLength);
			glGetShaderInfoLog(shader, logLength, NULL, log.data());
			throw ShaderException("shader compilation failed: " + std::string(log.data()));
		}
		
		return shader;
	}
	
	GLuint program;
};



class OpenGLBuffer
{
public:
	OpenGLBuffer(): VBO(0), VAO(0) {}
	
   ~OpenGLBuffer()
	{
		if (VBO)
			glDeleteBuffers(1, &VBO);
		
		if (VAO)
			glDeleteVertexArrays(1, &VAO);
	}
	
	void prepare(int maxPoints)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxPoints * sizeof(float) * 2, nullptr, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void draw(const std::vector<float>& vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size() / 2));
	}
	
private:
	GLuint VBO, VAO;
};



class Graphics
{
public:
	Graphics() {}
	
	void initialize(HWND hwnd)
	{
		if (!setupPixelFormat(hwnd))
			throw OpenGLException("failed to setup pixel format");
		
		shaders.initialize();
	}
	
	void prepareBuffers(int maxPoints)
	{
		buffer.prepare(maxPoints);
	}
	
	void drawVertices(const std::vector<float>& vertices)
	{
		glUseProgram(shaders.getProgram());
		buffer.draw(vertices);
	}
	
private:
	bool setupPixelFormat(HWND hwnd)
	{
		HDC hdc = ::GetDC(hwnd);
		
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR), 1, 
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 
			32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
		};
		
		int pf = ::ChoosePixelFormat(hdc, &pfd);
		
		if (pf == 0 || !SetPixelFormat(hdc, pf, &pfd))
		{
			::ReleaseDC(hwnd, hdc);
			throw OpenGLException("failed to set pixel format");
		}
		
		HGLRC hglrc = wglCreateContext(hdc);
		
		if (!hglrc || !wglMakeCurrent(hdc, hglrc))
		{
			::ReleaseDC(hwnd, hdc);
			throw OpenGLException("failed to create or activate OpenGL context");
		}
		
		if (glewInit() != GLEW_OK)
		{
			::ReleaseDC(hwnd, hdc);
			throw OpenGLException("failed to initialize GLEW");
		}
		
		return true;
	}
	
	Shaders shaders;
	OpenGLBuffer buffer;
};



class DataBuffer
{
public:
	DataBuffer()
	{
		dataBuffer.reserve(MAX_POINTS);
	}
	
	void push(float newData)
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		dataBuffer.push_back(newData);
		
		if (dataBuffer.size() > MAX_POINTS)
			dataBuffer.erase(dataBuffer.begin());
	}
	
	void prepare(std::vector<float>& vertices)
	{
		std::lock_guard<std::mutex> lock(bufferMutex);
		vertices.reserve(dataBuffer.size() * 2);
		
		for (size_t i = 0; i < dataBuffer.size(); ++i)
		{
			float x = (2.0f * i / MAX_POINTS) - 1.0f;
			float y = convertTwosComplementToInt(std::bitset<8>(dataBuffer[i])) / 128.0f;
			vertices.push_back(x);
			vertices.push_back(y);
		}
	}
	
private:
	int convertTwosComplementToInt(const std::bitset<8>& byte)
	{
		return byte[7] ? -std::bitset<8>(byte.to_ulong() - 1).flip().to_ulong() : byte.to_ulong();
	}
	
	static const int MAX_POINTS = 256;
	std::vector<float> dataBuffer;
	std::mutex bufferMutex;
};



class Application
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, const Settings& settings): isRunning(true), hwnd(NULL), settings(settings)
	{
		hwnd = setupWindow(hInstance, nCmdShow);
		try
		{
			glGraphics.initialize(hwnd);
			glGraphics.prepareBuffers(MAX_POINTS);
		}
		catch (const OpenGLException& e)
		{
			std::cout << "graphics initialization failed: " << e.what() << std::endl;
			std::cin.get();
			return;
		}
	}
	
	void run()
	{
		try
		{			
			port.setup(settings.getSerialPort(), settings.getBaudRate());
		}
		catch (const SerialException& e)
		{
			std::cout << "serial port setup failed: " << e.what() << std::endl;
			std::cin.get();
			return;
		}
		
		std::thread uartThread(&Application::runUARTThread, this);
		runLoop();
		isRunning = false;
		uartThread.join();
	}
	
private:
	HWND setupWindow(HINSTANCE hInstance, int nCmdShow)
	{
		WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_OWNDC, wndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "OpenGL", NULL};
		::RegisterClassEx(&wc);
		
		HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Foxie Window", 
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
			800, 600, NULL, NULL, wc.hInstance, NULL);
		
		::ShowWindow(hwnd, nCmdShow);
		return hwnd;
	}
	
	void runUARTThread()
	{
		char array[1];
		DWORD bytesRead;
		
		while (isRunning)
			if (::ReadFile(port.getHandle(), array, 1, &bytesRead, NULL) && bytesRead > 0)
				buffer.push(static_cast<float>(array[0]));
	}
	
	void render(HDC hdc, int& updateCounter)
	{
		if (++updateCounter >= BATCH_SIZE)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			
			std::vector<float> vertices;
			buffer.prepare(vertices);
			
			if (!vertices.empty())
				glGraphics.drawVertices(vertices);
			
			::SwapBuffers(hdc);
			updateCounter = 0;
		}
	}
	
	void runLoop()
	{
		HDC hdc = ::GetDC(hwnd);
		int updateCounter = 0;
		
		while (isRunning)
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					isRunning = false;
				
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			render(hdc, updateCounter);
		}
		
		::ReleaseDC(hwnd, hdc);
	}
	
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CLOSE)
			PostQuitMessage(0);
		
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	static const int MAX_POINTS = 256;
	static const int BATCH_SIZE = 100;
	const Settings& settings;
	
	std::atomic<bool> isRunning;
	HWND hwnd;
	
	COMPort port;
	Graphics glGraphics;
	DataBuffer buffer;
};



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Settings settings("settings.ini");
		Application app(hInstance, nCmdShow, settings);
		app.run();
	} 
	catch (const std::exception& e) 
	{
		std::cout << "application initialization failed: " << e.what() << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
