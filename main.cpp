//--------------------------------------------------------------------------------------------
//
// File: 	main.cpp
// Author:	P. Katarzynski (CCE)
//
// Description: Glowny plik programu
//
//--------------------------------------------------------------------------------------------
//									ZALEZNOSI 
//--------------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------------
// ZMIENNE GLOBALNE 
//--------------------------------------------------------------------------------------------
int window;  // uchwyt do okna OGL 
Scene *SC; // scena OpenGL

#define IDT_TIMER WM_USER + 100

bool    arbMultisampleSupported = false;
int arbMultisampleFormat = 0;
bool moveForward, moveBackward, strafeLeft, strafeRight;


//--------------------------------------------------------------------------------------------
// KOD WINAPI DLA VISUAL STUDIO 
//--------------------------------------------------------------------------------------------
HGLRC           hRC=NULL;
HDC             hDC=NULL;
HWND            hWnd=NULL;
HINSTANCE       hInstance;
HWND hListBox=NULL;

int wHeight = INITIAL_HEIGHT;
int wWidth = INITIAL_WIDTH;
int xMysz = 0;
int yMysz = 0;
int oldxMysz = 0;
int oldyMysz = 0;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------------------------
// usuwa zasoby zwiazane z oknem OpenGL
GLvoid KillGLWindow(GLvoid)
{
	if (hRC) {
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(hRC);
		hRC=NULL;
	}
	
	if (hDC) {
		ReleaseDC(hWnd,hDC);
		hDC=NULL;
	}

	if (hWnd) {
		DestroyWindow(hWnd);
		hWnd=NULL;
	}

	UnregisterClass("OpenGL",hInstance);
	hInstance=NULL;
}

//--------------------------------------------------------------------------------------------
// Alokuje zasoby i tworzy okno aplikacji z OpenGL 
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint		PixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;

	RECT WindowRect;
	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;

	hInstance		= GetModuleHandle(NULL);
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "OpenGL";

	if (!RegisterClass(&wc))
	{
	  MessageBox(NULL,
       "Rejestracja klasy zakonczona niepowodzeniem", 
       "ERROR",MB_OK|MB_ICONEXCLAMATION);
	  return FALSE;
	}

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle=WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	if (!(hWnd=CreateWindowEx(	dwExStyle,
					"OpenGL",
					title,
					WS_CLIPSIBLINGS |	WS_CLIPCHILDREN | dwStyle,
					30, 30,
					WindowRect.right-WindowRect.left,
					WindowRect.bottom-WindowRect.top,
					NULL,
					NULL,
					hInstance,
					NULL)))
	{
		KillGLWindow();
		MessageBox(NULL,"Utworzenie okna zakonczone niepowodzeniem", 
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 
		bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,       
		0, 0, 0
	};

	if (!(hDC=GetDC(hWnd)))	{
		KillGLWindow();
		return FALSE;
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	{
		KillGLWindow();
		return FALSE;
	}

	if (!arbMultisampleSupported)
	{
		if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
			KillGLWindow();
			return FALSE;
		}
	}
	else
	{
		if (!SetPixelFormat(hDC, arbMultisampleFormat, &pfd)) {
			KillGLWindow();
			return FALSE;
		}
	}

	if (!(hRC=wglCreateContext(hDC)))	{
		KillGLWindow();
		return FALSE;
	}

	if(!wglMakeCurrent(hDC,hRC)) {
		KillGLWindow();
		return FALSE;
	}


	ShowWindow(hWnd,SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	if (!arbMultisampleSupported)
	{
		if (InitMultisample(hInstance, hWnd, pfd))
		{
			wglDeleteContext(hRC);
			DestroyWindow(hWnd);
			UnregisterClass(wc.lpszClassName, hInstance);

			return CreateGLWindow(PROJECT_NAME, wWidth, wHeight, 16);
		}
	}

	//hListBox = CreateWindowEx( WS_EX_CLIENTEDGE, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |WS_VSCROLL,
    //0, wHeight-100, wWidth, 100, hWnd, NULL, hInstance, NULL );

	HFONT hLogFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hListBox,WM_SETFONT,(WPARAM) hLogFont,0);
	
	SC->Resize(width, height);
	SC->Init();

	return TRUE;
}
bool WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	// Try To Use wglGetExtensionStringARB On Current DC, If Possible
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return false;

	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return false;                       // No Match

												// Make Sure That Match Is At The Start Of The String Or That
												// The Previous Char Is A Space, Or Else We Could Accidentally
												// Match "wglFunkywglExtension" With "wglExtension"

												// Also, Make Sure That The Following Character Is Space Or NULL
												// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return true;                        // Match
	}
}
bool InitMultisample(HINSTANCE hInstance, HWND hWnd, PIXELFORMATDESCRIPTOR pfd)
{
	// See If The String Exists In WGL!
	if (!WGLisExtensionSupported("WGL_ARB_multisample"))
	{
		arbMultisampleSupported = false;
		return false;
	}

	// Get Our Pixel Format
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		arbMultisampleSupported = false;
		return false;
	}

	// Get Our Current Device Context
	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	int		valid;
	UINT	numFormats;
	float	fAttributes[] = { 0,0 };

	// These Attributes Are The Bits We Want To Test For In Our Sample
	// Everything Is Pretty Standard, The Only One We Want To 
	// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
	// These Two Are Going To Do The Main Testing For Whether Or Not
	// We Support Multisampling On This Hardware.
	int iAttributes[] =
	{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,24,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,16,
		WGL_STENCIL_BITS_ARB,0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,8,
		0,0
	};

	// First We Check To See If We Can Get A Pixel Format For 4 Samples
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);

	// If We Returned True, And Our Format Count Is Greater Than 1
	if (valid && numFormats >= 1)
	{
		arbMultisampleSupported = true;
		arbMultisampleFormat = pixelFormat;
		return arbMultisampleSupported;
	}

	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
	iAttributes[19] = 2;
	valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
	if (valid && numFormats >= 1)
	{
		arbMultisampleSupported = true;
		arbMultisampleFormat = pixelFormat;
		return arbMultisampleSupported;
	}

	// Return The Valid Format
	return  arbMultisampleSupported;
}

//--------------------------------------------------------------------------------------------
// Przetwarza komunikaty systemowe wysylane do okna aplikacji 
LRESULT CALLBACK WndProc(	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM	lParam)
{
	switch (uMsg)
	{

	case WM_CREATE: // utworzenie okna 

		break;


	case WM_ACTIVATE:         // aktywacja okna 
	{
		return 0;
	}

	case WM_PAINT: // odrysowanie okna
	{
		SC->Draw();
		SwapBuffers(hDC);
		break;
	}

	case WM_TIMER: // zdarzenie timera
	{
		SC->Animate();
		SC->Draw();
		SwapBuffers(hDC);
		break;
	}


	case WM_SYSCOMMAND:     // zdarzenia systemowe
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}
	case WM_CLOSE:         // zamkniecie okna 
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:      // nacisniecie klawisza 
	{
		POINT cPos;
		GetCursorPos(&cPos);
		SC->KeyPressed(wParam, cPos.x, cPos.y);

		return 0;
	}
	case WM_KEYUP:        // zwolnienie klawisza 
	{
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
			
		    yMysz = GET_WHEEL_DELTA_WPARAM(wParam);			  
			SC->RuszMysz(0.0, yMysz);
			oldyMysz = yMysz;
			return 0;\

	}
	case WM_MOUSEMOVE:      // zmiana rozmiaru okna 
	{
			  xMysz = LOWORD(lParam);
			 // yMysz = HIWORD(lParam);
			 
			  SC->RuszMysz(xMysz-oldxMysz, 0.0);

			  oldxMysz = xMysz;
			 // oldyMysz = yMysz;


			  return 0;
	}
	case WM_SIZE:      // zmiana rozmiaru okna 
	{
		// aktualizuj zmienne z rozmiarem okna 
		wWidth = LOWORD(lParam);
		wHeight = HIWORD(lParam);

		// przeskaluj scene OGL 
		SC->Resize(wWidth, wHeight);

		//przesun kontrolke z logiem 
		MoveWindow(hListBox, 0, wHeight - 100, wWidth, 100, true);

		return 0;
	}
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);

}

//--------------------------------------------------------------------------------------------
// glowny podprogram 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG	msg;

	try
	{
		SC = new Scene(wWidth, wHeight);
		// utworz okno z widokiem sceny OPENGL
		if (!CreateGLWindow(PROJECT_NAME, wWidth, wHeight, 16))
			return 0;
		// ustaw obiekt timera
		SetTimer(hWnd, IDT_TIMER, 10, (TIMERPROC)NULL);
		// pobierz komunikat z kolejki systemowej
		while (GetMessage(&msg, NULL, 0, 0))
		{
			// przetwarzaj komunikat w obszarze okna
			TranslateMessage(&msg);
			// usun komunikat z kolejki systemowej
			DispatchMessage(&msg);
		}
	}
	catch (char *e)
	{
		MessageBox(0, e, "Wystapil wyjatek", 0);
	}
	catch (...) // unhandled exceptions
	{
		KillGLWindow(); // usun zasoby okna
		if (SC) delete SC;
	}
	// usun zasob timera
	KillTimer(hWnd, IDT_TIMER);
	KillGLWindow(); // usun zasoby okna

	if (SC) delete SC;

	return 0;
}
//--------------------------------------------------------------------------------------------
// koniec pliku 
