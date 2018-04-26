#include "imageLoader.h"
#include <wincodec.h>

namespace oxyde {
	namespace utility {

		std::vector<unsigned char>  loadImageFromFile(const std::wstring &fileName, unsigned int &Width, unsigned int &Height)
		{
			std::vector<unsigned char> buffer(0);

			IWICImagingFactory *imagingFactory = NULL;
			HRESULT hr = S_OK;
			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&imagingFactory));
			if (hr == S_OK) {
				IWICBitmapDecoder *bitmapDecoder = NULL;
				hr = imagingFactory->CreateDecoderFromFilename(fileName.data(), NULL, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnLoad, &bitmapDecoder);
				if (hr == S_OK) {
					IWICBitmapFrameDecode *frameDecode = NULL;
					hr = bitmapDecoder->GetFrame(0, &frameDecode);
					if (hr == S_OK) {
						IWICBitmapFlipRotator *flipper = NULL;
						hr = imagingFactory->CreateBitmapFlipRotator(&flipper);
						if (hr == S_OK) {
							hr = flipper->Initialize(frameDecode, WICBitmapTransformFlipVertical);
							if (hr == S_OK) {
								///
								IWICFormatConverter *formatConverter = NULL;
								hr = imagingFactory->CreateFormatConverter(&formatConverter);
								if (hr == S_OK) {
									WICPixelFormatGUID sourcePixelFormat;
									hr = flipper->GetPixelFormat(&sourcePixelFormat);
									BOOL willConvert = FALSE;
									hr = formatConverter->CanConvert(sourcePixelFormat, GUID_WICPixelFormat32bppRGBA, &willConvert);
									if (hr == S_OK || willConvert) {
										hr = formatConverter->Initialize(flipper, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, .0f, WICBitmapPaletteTypeCustom);
										if (hr == S_OK) {
											IWICBitmap *theBitmap = NULL;
											hr = imagingFactory->CreateBitmapFromSource(formatConverter, WICBitmapCacheOnLoad, &theBitmap);
											if (hr == S_OK) {
												IWICBitmapLock *theLock = NULL;
												hr = theBitmap->Lock(NULL, WICBitmapLockRead, &theLock);
												if (hr == S_OK) {
													theLock->GetSize(&Width, &Height);
													unsigned int dataSize = 0;
													unsigned char *dataPointer = NULL;
													theLock->GetDataPointer(&dataSize, &dataPointer);
													buffer = std::vector<unsigned char>(dataPointer, dataPointer + dataSize);
													buffer.shrink_to_fit();

													theLock->Release();
												}
												theBitmap->Release();
											}
										}
									}
									formatConverter->Release();
								}
							}
							///
							flipper->Release();
						}
						frameDecode->Release();
					}
					bitmapDecoder->Release();
				}
				imagingFactory->Release();
			}
			return buffer;
		}

	}
}
