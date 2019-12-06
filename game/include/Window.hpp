#pragma once

#include <String.hpp>
#include <Vector2.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Window
{
    public:
        Window();
        ~Window();

        void Create(int width, int height, const String& title);
        void Destroy();
		bool IsOpen();
		void Display();
private:
	Vector2i minimumSize;
	bool isOpen;
	void RegisterWindowClass();
	HGLRC RC;
	HDC DC;
	HWND WND;
	void ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam);

	void ProcessEvents();
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};