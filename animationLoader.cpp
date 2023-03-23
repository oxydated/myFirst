#include "animationLoader.h"
#include "XMLDocument.h"

void loadAnimation(HWND hWnd) 
{
	IFileDialog* openFile = NULL;
	COMDLG_FILTERSPEC fileFilter[1];
	fileFilter[0].pszName = L"oxyde model file";
	fileFilter[0].pszSpec = L"*.oxy";
	HRESULT hr = S_OK;

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

}

