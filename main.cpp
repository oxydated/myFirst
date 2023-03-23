#include <windows.h>
#include <string>
#include <shellapi.h>
#include <Shobjidl.h>
//#include <PathCch.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <Windowsx.h>

#include <tchar.h>

#include "resource.h"
#include "initGL.h"
#include "initScene.h"
#include "myGLEStest.h"
#include "vertexArrays.h"
#include "interactivity.h"

//new headers

#include "XMLDocument.h"
#include "scene.h"
#include "ticker.h"
#include "mesh.h"
#include "skindata.h"
#include "renderer.h"
#include "camera.h"



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	static TCHAR szWindowClass[] = _T("oxyEngine");
	static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MYGLESTEST);;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	//wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500,
		500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(
			NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL
		);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg = {};
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

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	RECT theWindowRect;
	HDC hdc;
	int i;

	IFileDialog *openFile = NULL;
	COMDLG_FILTERSPEC fileFilter[1];
	fileFilter[0].pszName = L"oxyde model file";
	fileFilter[0].pszSpec = L"*.oxy";
	HRESULT hr = S_OK;

	POINT p = {0, 0};

	switch (message)
	{
	case WM_SIZE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		GetWindowRect(hWnd, &theWindowRect);
		GetClientRect(hWnd, &theWindowRect);
		oxyde::GL::renderer::camera::resizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_LBUTTONDOWN:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		setWinX(p.x);
		setWinY(p.y);
		setButtonPressed(true);	 
		OutputDebugString(TEXT("\n\nDOWN\n"));
		oxyde::GL::renderer::camera::mouseButtonDown(p.x, p.y);
		break;

	case WM_LBUTTONUP:
		setButtonPressed(false);
		OutputDebugString(TEXT("\nUP\n\n"));
		oxyde::GL::renderer::camera::mouseButtonUp();
		break;

	case WM_MOUSEMOVE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		setWinX(p.x);
		setWinY(p.y);
		oxyde::GL::renderer::camera::mouseMove(p.x, p.y);
		break;

	case WM_MOUSEWHEEL:
		oxyde::GL::renderer::camera::mouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_LEFT:
			oxyde::GL::renderer::camera::keyDown(-1);
			break;
		case VK_RIGHT:
			oxyde::GL::renderer::camera::keyDown(1);
			break;
		default:
			break;
		}
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
		case ID_FILE_OPENFILE:
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&openFile));;
			if (SUCCEEDED(hr)) {
				hr = openFile->SetDefaultExtension(L"oxy");
				if (SUCCEEDED(hr)) {
					hr = openFile->SetFileTypes(1, fileFilter);
					if (SUCCEEDED(hr)) {
						hr = openFile->Show(hWnd);
						if (SUCCEEDED(hr)) {
							IShellItem* fileToLoad = NULL;
							hr = openFile->GetResult(&fileToLoad);
							if (SUCCEEDED(hr)) {
								LPWSTR fileName = NULL;
								hr = fileToLoad->GetDisplayName(SIGDN_FILESYSPATH, &fileName);
								if (SUCCEEDED(hr)) {
									std::wstring fileNameW(fileName);
									int ilen = fileNameW.length();
									MSXML2::IXMLDOMDocumentPtr theDocument = oxyde::XML::loadDocument(fileNameW);
									if (theDocument) {
										
										std::array<WCHAR, MAX_PATH> dirName;
										StringCchCopy(dirName.data(), dirName.size(), fileNameW.data());
										PathRemoveFileSpecW(dirName.data());
										SetCurrentDirectoryW(dirName.data());
										MSXML2::IXMLDOMElementPtr documentElement = theDocument->GetdocumentElement();
										
										MSXML2::IXMLDOMNodePtr sceneNode = MSXML2::IXMLDOMNodePtr(documentElement)->selectSingleNode(L"./scene");
										std::wstring theXML(sceneNode->xml);
										int i = theXML.length();
										oxyde::scene::scenePtr theScene = oxyde::scene::scene::createScene(sceneNode);

										oxyde::scene::ticker::start();

										oxyde::GL::renderer::skinRenderer::reset();
										oxyde::geometry::skindata::buildSkindata(documentElement, oxyde::scene::bone::addModifier);
										
										GetClientRect(hWnd, &theWindowRect);
										
										LONG width = theWindowRect.right - theWindowRect.left;
										LONG height = theWindowRect.bottom - theWindowRect.top;
										
										oxyde::GL::renderer::camera::importCameraFromDocument(documentElement, width, height);

										theScene->updateFrame();

									}
									CoTaskMemFree(fileName);
								}
								fileToLoad->Release();
							}
						}
					}
				}
				openFile->Release();
			}

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
