#include "stdafx.h"
#include "WicImage.h"
#include "WICImagingFactory.h"
#include <assert.h>

#pragma comment(lib, "Windowscodecs.lib")

#define DIB_WIDTHBYTES(bits)    ((((bits) + 31)>>5)<<2)

// #undef these at the end of the file.
#define IfFailedThrowHR(expr) {HRESULT hr = (expr); if (FAILED(hr)) throw hr;}
//###########################################################################################################
CWicImage::CWicImage()
	: m_pDecoder(nullptr)
	, m_pFrame(nullptr)
	, m_pConvertedFrame(nullptr)
	, m_nWidth(0)
	, m_nHeight(0)
{
	// Get the WIC factory from the singleton wrapper class
	pFactory = CWICImagingFactory::GetInstance().GetFactory();
}
//###########################################################################################################
CWicImage::~CWicImage()
{
	Cleanup();
}
//###########################################################################################################
void CWicImage::Cleanup()
{
	m_nWidth = m_nHeight = 0;

	if (m_pConvertedFrame)
	{
		m_pConvertedFrame.Release();
		m_pConvertedFrame = nullptr;
	}
	if (m_pFrame)
	{
		m_pFrame.Release();
		m_pFrame = nullptr;
	}
	if (m_pDecoder)
	{
		m_pDecoder.Release();
		m_pDecoder = nullptr;
	}
}
//###########################################################################################################
void CWicImage::Open(const wchar_t* pszFile, bool bAlphaChannel, UINT nFrame/* = 0*/)
{
	try
	{
		// Cleanup a previously loaded image
		Cleanup();

		// Get the WIC factory from the singleton wrapper class
		//IWICImagingFactory* pFactory = CWICImagingFactory::GetInstance().GetFactory();
		assert(pFactory);
		if (!pFactory)
			throw WINCODEC_ERR_NOTINITIALIZED;

		// Create a decoder for the given image file
		IfFailedThrowHR(pFactory->CreateDecoderFromFilename(
			pszFile, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &m_pDecoder));

		// Validate the given frame index nFrame
		UINT nCount = 0;
		// Get the number of frames in this image
		if (SUCCEEDED(m_pDecoder->GetFrameCount(&nCount)))
		{
			if (nFrame >= nCount)
				nFrame = nCount - 1; // If the requested frame number is too big, default to the last frame
		}
		// Retrieve the requested frame of the image from the decoder
		IfFailedThrowHR(m_pDecoder->GetFrame(nFrame, &m_pFrame));

		// Retrieve the image dimensions
		IfFailedThrowHR(m_pFrame->GetSize(&m_nWidth, &m_nHeight));

		// Convert the format of the image frame to 32bppBGR
		IfFailedThrowHR(pFactory->CreateFormatConverter(&m_pConvertedFrame));

		if (!bAlphaChannel) // without alpha-channel
		{
			IfFailedThrowHR(m_pConvertedFrame->Initialize(
				m_pFrame,                        // Source frame to convert
				GUID_WICPixelFormat32bppBGR,     // The desired pixel format
				WICBitmapDitherTypeNone,         // The desired dither pattern
				NULL,                            // The desired palette 
				0.f,                             // The desired alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			));
		}
		else // with alpha-channel
		{
			IfFailedThrowHR(m_pConvertedFrame->Initialize(
				m_pFrame,                        // Source frame to convert
				GUID_WICPixelFormat32bppBGRA,     // The desired pixel format
				WICBitmapDitherTypeNone,         // The desired dither pattern
				NULL,                            // The desired palette 
				0.f,                             // The desired alpha threshold
				WICBitmapPaletteTypeCustom       // Palette translation type
			));
		}
	}
	catch (...)
	{
		// Cleanup after something went wrong
		Cleanup();
		// Rethrow the exception, so the client code can handle it
		throw;
	}
}
//#####################################################################################################################
bool CWicImage::IsLoaded() const
{
	return m_pConvertedFrame != nullptr;
}
//#####################################################################################################################
void CWicImage::Render(HDC hDC, UINT x, UINT y, UINT cx, UINT cy)
{
	// Make sure an image has been loaded
	if (!IsLoaded())
		throw WINCODEC_ERR_WRONGSTATE;

	// Get the WIC factory from the singleton wrapper class
	//IWICImagingFactory* pFactory = CWICImagingFactory::GetInstance().GetFactory();
	if (!pFactory)
		throw WINCODEC_ERR_NOTINITIALIZED;

	// Create a WIC image scaler to scale the image to the requested size
	CComPtr<IWICBitmapScaler> pScaler = nullptr;
	IfFailedThrowHR(pFactory->CreateBitmapScaler(&pScaler));
	IfFailedThrowHR(pScaler->Initialize(m_pConvertedFrame, cx, cy, WICBitmapInterpolationModeFant));

	// Render the image to a GDI device context
	HBITMAP hDIBBitmap = NULL;
	try
	{
		// Get a DC for the full screen
		HDC hdcScreen = GetDC(NULL);
		if (!hdcScreen)
			throw 1;

		BITMAPINFO bminfo;
		ZeroMemory(&bminfo, sizeof(bminfo));
		bminfo.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
		bminfo.bmiHeader.biWidth        = cx;
		bminfo.bmiHeader.biHeight       = -(LONG)cy;
		bminfo.bmiHeader.biPlanes       = 1;
		bminfo.bmiHeader.biBitCount     = 32;
		bminfo.bmiHeader.biCompression  = BI_RGB;		

		void* pvImageBits = nullptr;	// Freed with DeleteObject(hDIBBitmap)
		hDIBBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
		if (!hDIBBitmap)
			throw 2;

		ReleaseDC(NULL, hdcScreen);

		// Calculate the number of bytes in 1 scanline
		UINT nStride = DIB_WIDTHBYTES(cx * 32);
		// Calculate the total size of the image
		UINT nImage = nStride * cy;
		// Copy the pixels to the DIB section
		IfFailedThrowHR(pScaler->CopyPixels(nullptr, nStride, nImage, reinterpret_cast<BYTE*>(pvImageBits)));

		// Copy the bitmap to the target device context
		::SetDIBitsToDevice(hDC, x, y, cx, cy, 0, 0, 0, cy, pvImageBits, &bminfo, DIB_RGB_COLORS);

		DeleteObject(hDIBBitmap);
	}
	catch (...)
	{
		if (hDIBBitmap)
			DeleteObject(hDIBBitmap);
		// Rethrow the exception, so the client code can handle it
		throw;
	}
}
//#############################################################################################################################
void CWicImage::RenderAlpha(HDC hDC, UINT x, UINT y, UINT cx, UINT cy, int iAlpha, COLORREF cTransparent)
{
	// Make sure an image has been loaded
	if (!IsLoaded())
		throw WINCODEC_ERR_WRONGSTATE;

	// Get the WIC factory from the singleton wrapper class
	//IWICImagingFactory* pFactory = CWICImagingFactory::GetInstance().GetFactory();
	if (!pFactory)
		throw WINCODEC_ERR_NOTINITIALIZED;

	// Create a WIC image scaler to scale the image to the requested size
	CComPtr<IWICBitmapScaler> pScaler = nullptr;
	IfFailedThrowHR(pFactory->CreateBitmapScaler(&pScaler));
	IfFailedThrowHR(pScaler->Initialize(m_pConvertedFrame, cx, cy, WICBitmapInterpolationModeFant));

	// Render the image to a GDI device context
	HBITMAP hDIBBitmap = NULL;
	HDC memDC = NULL;

	try
	{
		// Get a DC for the full screen
		HDC hdcScreen = GetDC(NULL);
		if (!hdcScreen)
			throw 1;

		HDC memDC = CreateCompatibleDC(hdcScreen);
		if (!memDC)
			throw 2;

		BITMAPINFO bminfo;
		ZeroMemory(&bminfo, sizeof(bminfo));
		bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bminfo.bmiHeader.biWidth = cx;
		bminfo.bmiHeader.biHeight = -(LONG)cy;
		bminfo.bmiHeader.biPlanes = 1;
		bminfo.bmiHeader.biBitCount = 32;
		bminfo.bmiHeader.biCompression = BI_RGB;

		void* pvImageBits = nullptr;    // Freed with DeleteObject(hDIBBitmap)
		hDIBBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
		if (!hDIBBitmap)
			throw 3;

		ReleaseDC(NULL, hdcScreen);

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, hDIBBitmap);

		// Calculate the number of bytes in 1 scanline
		UINT nStride = DIB_WIDTHBYTES(cx * 32);
		// Calculate the total size of the image
		UINT nImage = nStride * cy;
		// Copy the pixels to the DIB section
		IfFailedThrowHR(pScaler->CopyPixels(nullptr, nStride, nImage, reinterpret_cast<BYTE*>(pvImageBits)));

		// Copy the bitmap to the target device context

		::SelectObject(memDC, hOldBitmap);
		AlphaBlendWithGDIplus(hDC, hDIBBitmap, x, y, cx, cy, 0, 0, cx, cy, iAlpha, cTransparent);

		DeleteDC(memDC);
		DeleteObject(hDIBBitmap);
	}
	catch (...)
	{
		if (hDIBBitmap)
			DeleteObject(hDIBBitmap);
		if (memDC)
			DeleteDC(memDC);
		
		// Rethrow the exception, so the client code can handle it
		throw;
	}
}
//###############################################################################################################
void CWicImage::AlphaBlendWithGDIplus(HDC hDC, HBITMAP hBitmap, int x, int y, int cx, int cy, int srcx, int srcy, int srccx, int srccy, int iAlpha, COLORREF cTransparentColor)
{
	Graphics g(hDC);
	Bitmap* pBitmap = new Bitmap(srccx, srccy, PixelFormat32bppPARGB);
	if (!pBitmap) return;
	
	float fBlend;
	fBlend = (float)iAlpha/ (float)255.0;

	ColorMatrix BitmapMatrix = 
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, fBlend, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;
	Color ColorLow(GetRValue(cTransparentColor), GetGValue(cTransparentColor), GetBValue(cTransparentColor));
	Color ColorHigh(GetRValue(cTransparentColor), GetGValue(cTransparentColor), GetBValue(cTransparentColor));

	ImgAttr.SetColorKey(ColorLow, ColorHigh, ColorAdjustTypeBitmap);

	ImgAttr.SetColorMatrix(&BitmapMatrix,
		ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);

	RectF destination((REAL)x, (REAL)y, (REAL)cx, (REAL)cy);

	g.DrawImage(pBitmap->FromHBITMAP(hBitmap, 0), destination, (REAL)0, (REAL)0, (REAL)srccx, (REAL)srccy, UnitPixel, &ImgAttr);
	delete pBitmap;
}
//###############################################################################################################
UINT CWicImage::GetWidth() const
{
	return m_nWidth;
}
//###############################################################################################################
UINT CWicImage::GetHeight() const
{
	return m_nHeight;
}
//###############################################################################################################
#undef IfFailedThrowHR
