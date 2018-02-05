#ifdef _WIN32
#include "stdafx.h"
#endif
#include "initGL.h"

void initGLContext(HDC hDc){
	PIXELFORMATDESCRIPTOR pfd;

	int i_pixelFormat = GetPixelFormat(hDc);
	DescribePixelFormat(hDc, i_pixelFormat, sizeof(pfd), &pfd);

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = 8;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	i_pixelFormat = ChoosePixelFormat(hDc, &pfd);
	int isSet = SetPixelFormat(hDc, i_pixelFormat, &pfd);
	if (isSet) {
		HGLRC hGl = wglCreateContext(hDc);
		if (hGl) {
			wglMakeCurrent(hDc, hGl);
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				/* Problem: glewInit failed, something is seriously wrong. */
				MessageBox(NULL, (LPCWSTR)L"FAILED", (LPCWSTR)L"GLEW", MB_OK);
			}

			//if (glewIsSupported("GL_VERSION_2_0 GL_ARB_ES2_compatibility")) {

			//	MessageBox(NULL, (LPCWSTR)L"GLES 2.0", (LPCWSTR)L"OK", MB_OK);
			//}

			if (glewIsSupported("GL_VERSION_4_3")) {

				MessageBox(NULL, (LPCWSTR)L"GL 4.3", (LPCWSTR)L"OK", MB_OK);
			}

			//if (wglewIsSupported("WGL_EXT_create_context_es2_profile")) {
			//	MessageBox(NULL, (LPCWSTR)L"create context es2", (LPCWSTR)L"SUPPORTED", MB_OK);
			//}

			//int attribList[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,	2,
			//	WGL_CONTEXT_MINOR_VERSION_ARB,	0,
			//	WGL_CONTEXT_PROFILE_MASK_ARB ,	WGL_CONTEXT_ES2_PROFILE_BIT_EXT,
			//	0
			//};

			int attribList[] = { 
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 3,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			SetLastError(ERROR_SUCCESS);
			DWORD contextError = GetLastError();
			HGLRC hGLES = wglCreateContextAttribsARB(hDc, hGl, attribList);
			contextError = GetLastError();
			if (contextError == ERROR_SUCCESS) {
				//MessageBox(NULL, (LPCWSTR)L"context es2", (LPCWSTR)L"CREATED", MB_OK);
				MessageBox(NULL, (LPCWSTR)L"context GL4.3", (LPCWSTR)L"CREATED", MB_OK);
			}
			else {
				MessageBox(NULL, (LPCWSTR)L"context GL4.3", (LPCWSTR)L"FAILED", MB_OK);
			}
			if (wglMakeCurrent(hDc, hGLES) != TRUE) {
				contextError = GetLastError();
			}
		}


	}

}