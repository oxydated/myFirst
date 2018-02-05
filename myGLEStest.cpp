// myGLEStest.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#include "stdafx.h"
#include <Windowsx.h>
#endif
#include "initGL.h"
#include "initScene.h"
#include "myGLEStest.h"
#include "vertexArrays.h"
#include "interactivity.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYGLESTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	HWND hWnd = InitInstance (hInstance, nCmdShow);
	if (!hWnd)
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGLESTEST));

	ZeroMemory(&msg, sizeof(msg));
	static float rot = 0.0;
	bool goOn = true;
	while(goOn){
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
			if(msg.message == WM_QUIT){
				goOn = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		{		
			
			drawVertexArray();
			SwapBuffers(wglGetCurrentDC());
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGLESTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYGLESTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return hWnd;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return hWnd;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	RECT theWindowRect;
	HDC hdc;
	int i;

	POINT p = {0, 0};

	switch (message)
	{
	case WM_SIZE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		GetWindowRect( hWnd, &theWindowRect);
		GetClientRect(hWnd, &theWindowRect);
		break;

	case WM_LBUTTONDOWN:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		setWinX(p.x);
		setWinY(p.y);
		setButtonPressed(true);	 
		OutputDebugString(TEXT("\n\nDOWN\n"));
		break;
	case WM_LBUTTONUP:
		setButtonPressed(false);
		OutputDebugString(TEXT("\nUP\n\n"));
		break;

	case WM_MOUSEMOVE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		setWinX(p.x);
		setWinY(p.y);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &theWindowRect);
		initGLContext(hdc);
		initGLES(theWindowRect.left, theWindowRect.top, theWindowRect.right - theWindowRect.left, theWindowRect.bottom - theWindowRect.top);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
