#include <Window.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <glad/glad_wgl.h>

const wchar_t* className = L"Toasty_Window"; // Reused class name.
unsigned int windowCount = 0;

Window::Window()
{
}

Window::~Window()
{
}

void Window::Create(int width, int height, const String& title)
{
	RegisterWindowClass();

	HMODULE hInstance = GetModuleHandle(NULL);
	WString wideTitle = converter.from_bytes(title);

	HWND fakeWND = CreateWindow(
		className, wideTitle.c_str(),
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,						// position x, y
		1, 1,						// width, height
		NULL, NULL,					// parent window, menu
		hInstance, NULL);			// instance, param

	HDC fakeDC = GetDC(fakeWND);	// Device Context

	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;

	const int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
	if (fakePFDID == 0) {
		printf("Fatal Error: ChoosePixelFormat failed.\n");
		fflush(stdout);
		exit(1);
	}

	if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
		printf("Fatal Error: SetPixelFormat failed.\n");
		fflush(stdout);
		exit(1);
	}

	HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex

	if (fakeRC == 0) {
		printf("Fatal Error: wglCreateContext failed.\n");
		fflush(stdout);
		exit(1);
	}

	if (wglMakeCurrent(fakeDC, fakeRC) == false) {
		printf("Fatal Error: wglMakeCurrent failed.\n");
		fflush(stdout);
		exit(1);
	}

	// get pointers to functions (or init opengl loader here)
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == nullptr) {
		printf("Fatal Error: wglChoosePixelFormatARB failed.\n");
		fflush(stdout);
		exit(1);
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == nullptr) 
	{
			printf("Fatal Error: wglCreateContextAttribsARB failed.\n");
			fflush(stdout);
			exit(1);
	}

	// create a new window and context
	WND = CreateWindow(className, wideTitle.c_str(),
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,							// styles
		CW_USEDEFAULT, 0,
		width, height,
		NULL, NULL,
		hInstance, this);

	DC = GetDC(WND);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pixelFormatID; UINT numFormats;
	const bool status = wglChoosePixelFormatARB(DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) 
	{
		printf("Fatal Error: wglChoosePixelFormatARB failed.\n");
		fflush(stdout);
		exit(1);
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(DC, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 0;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
	};

	RC = wglCreateContextAttribsARB(DC, 0, contextAttribs);
	if (RC == NULL) {
		printf("Fatal Error: wglCreateContextAttribsARB failed.\n");
		fflush(stdout);
		exit(1);
	}

	// delete temporary context and window
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(fakeWND, fakeDC);
	DestroyWindow(fakeWND);
	if (!wglMakeCurrent(DC, RC)) {
		printf("Fatal Error: wglMakeCurrent failed.\n");
		fflush(stdout);
		exit(1);
	}

	gladLoadGL();

	ShowWindow(WND, SW_SHOW);
	windowCount++;
	isOpen = true;
}

void Window::Destroy()
{
	wglMakeCurrent(NULL, NULL);

	if (RC) 
	{
		wglDeleteContext(RC);
	}

	if (DC) 
	{
		ReleaseDC(WND, DC);
	}

	if (WND) 
	{
		DestroyWindow(WND);
	}
}

bool Window::IsOpen()
{
	return isOpen;
}

void Window::Display()
{
	SwapBuffers(DC);
	ProcessEvents();
}

void Window::RegisterWindowClass()
{
	if (windowCount > 0)
		return;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = className;

	if (!RegisterClassEx(&wcex))
	{
		printf("Fatal Error: RegisterClassEx failed.\n");
		fflush(stdout);
		exit(1);
	}
}


void Window::ProcessEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			isOpen = false;
			break;
		}
	}
}

void Window::ProcessEvents()
{
	MSG message;
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

// Microsoft Windows event handling function:
LRESULT CALLBACK Window::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Associate handle and Window instance when the creation message is received
	if (message == WM_CREATE)
	{
		// Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
		LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

		// Set as the "user data" parameter of the window
		SetWindowLongPtrW(windowHandle, GWLP_USERDATA, window);
	}

	// Get the WindowImpl instance corresponding to the window handle
	Window* window = windowHandle ? reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA)) : NULL;

	// Forward the event to the appropriate instance
	if (window)
	{
		window->ProcessEvent(message, wParam, lParam);
	}

	// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
	if (message == WM_CLOSE)
		return 0;

	// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
	if ((message == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
		return 0;

	return DefWindowProc(windowHandle, message, wParam, lParam);
}