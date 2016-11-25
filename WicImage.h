#pragma once

#include <atlbase.h>
#include <wincodec.h>
#include <wincodecsdk.h>

class CWicImage
{
public:
	CWicImage();
	virtual ~CWicImage();

	// Opens the nFrame-th frame of the given image file.
	// Throws HRESULT in case of failure.
	virtual void Open(const wchar_t* pszFile, bool bAlphaChannel, UINT nFrame = 0);

	// Returns true if an image is loaded successfully, false otherwise
	virtual bool IsLoaded() const;

	// Renders the loaded image to the given device context hDC,
	// at position x,y and size cx, cy.
	// Throws HRESULT in case of failure.
	virtual void Render(HDC hDC, UINT x, UINT y, UINT cx, UINT cy);
	virtual void RenderAlpha(HDC hDC, UINT x, UINT y, UINT cx, UINT cy, int iAlpha, COLORREF cTransparent);

	// Returns the width of the loaded image.
	virtual UINT GetWidth() const;

	// Returns the height of the loaded image.
	virtual UINT GetHeight() const;

	// Alpha blending with transparent color
	void AlphaBlendWithGDIplus(HDC hDC, HBITMAP hBitmap, int x, int y, int cx, int cy, int srcx, int srcy, int srccx, int srccy, int iAlpha, COLORREF cTransparentColor);

protected:
	virtual void Cleanup();
	IWICImagingFactory* pFactory;
	CComPtr<IWICBitmapDecoder> m_pDecoder;
	CComPtr<IWICBitmapFrameDecode> m_pFrame;
	CComPtr<IWICFormatConverter> m_pConvertedFrame;
	UINT m_nWidth;
	UINT m_nHeight;
};