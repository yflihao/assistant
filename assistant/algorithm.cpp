#include "stdafx.h"
#include "algorithm.h"

#include <assert.h>
#include <atltypes.h>


CAlgorithm::CAlgorithm()
{

}

CAlgorithm::~CAlgorithm()
{

}

void CAlgorithm::SetAlgorithmEvent(int event, WPARAM wParam, LPARAM lParam)
{

}

HBITMAP CAlgorithm::CaptureBmp(HWND _capture)	// bitmap�����Ҫ�����ͷ�
{
	HWND DeskHwnd = ::GetDesktopWindow(); 
	HDC DeskDC = ::GetWindowDC(DeskHwnd); 
	
	RECT CaptureRect;  
	::GetWindowRect(_capture, &CaptureRect);
	if( CaptureRect.left < 0 ) CaptureRect.left = 0;  
	if (CaptureRect.top < 0 ) CaptureRect.top = 0; 

	SIZE CaptureSize;  
	CaptureSize.cx = CaptureRect.right-CaptureRect.left;  
	CaptureSize.cy = CaptureRect.bottom-CaptureRect.top; 

	HDC MemDC = ::CreateCompatibleDC(DeskDC);
	HBITMAP hBitmap = ::CreateCompatibleBitmap(DeskDC, CaptureSize.cx, CaptureSize.cy);
	HGDIOBJ hOldBMP = ::SelectObject(MemDC, hBitmap); 
	::BitBlt(MemDC, 0, 0, CaptureSize.cx, CaptureSize.cy, DeskDC, CaptureRect.left, CaptureRect.top, SRCCOPY);
	
	::SelectObject(MemDC, hOldBMP); 
	::DeleteDC(MemDC);  
	::ReleaseDC(DeskHwnd, DeskDC);  

	return hBitmap;
}

void CAlgorithm::SaveBmp(HBITMAP hBitmap, CString name)  
{  
	HDC hDC =::CreateDC("DISPLAY", NULL, NULL, NULL);   
	int iBits = ::GetDeviceCaps(hDC, BITSPIXEL) * ::GetDeviceCaps(hDC, PLANES);//��ǰ�ֱ�����ÿ��������ռ�ֽ���    
	::DeleteDC(hDC);  

	WORD   wBitCount;   //λͼ��ÿ��������ռ�ֽ���      
	if (iBits <= 1)  
		wBitCount = 1;  
	else if (iBits <= 4)  
		wBitCount = 4;  
	else if (iBits <= 8)  
		wBitCount = 8;  
	else if (iBits <= 24)  
		wBitCount = 24;  
	else  
		wBitCount = iBits;  

	DWORD dwPaletteSize=0;    //��ɫ���С�� λͼ�������ֽڴ�С   
	if (wBitCount <= 8)        
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);      


	BITMAP bm;        //λͼ���Խṹ  
	::GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);    


	BITMAPINFOHEADER   bi;       //λͼ��Ϣͷ�ṹ       
	bi.biSize            = sizeof(BITMAPINFOHEADER);    
	bi.biWidth           = bm.bmWidth;  
	bi.biHeight          = bm.bmHeight;  
	bi.biPlanes          = 1;  
	bi.biBitCount        = wBitCount;  
	bi.biCompression     = BI_RGB; //BI_RGB��ʾλͼû��ѹ��  
	bi.biSizeImage       = 0;  
	bi.biXPelsPerMeter   = 0;  
	bi.biYPelsPerMeter   = 0;  
	bi.biClrUsed         = 0;  
	bi.biClrImportant    = 0;  

	DWORD dwBmBitsSize = ((bm.bmWidth * wBitCount+31)/32) * 4 * bm.bmHeight;      
	HANDLE hDib  = ::GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  //Ϊλͼ���ݷ����ڴ�  
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  

	HANDLE hPal = ::GetStockObject(DEFAULT_PALETTE);  // �����ɫ��   
	HANDLE  hOldPal=NULL;   
	if (hPal)  
	{  
		hDC = ::GetDC(NULL);  
		hOldPal = SelectPalette(hDC,(HPALETTE)hPal, FALSE);  
		RealizePalette(hDC);  
	}  
	::GetDIBits(hDC, hBitmap, 0, (UINT) bm.bmHeight,(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,(BITMAPINFO*)lpbi,DIB_RGB_COLORS);// ��ȡ�õ�ɫ�����µ�����ֵ  
	if (hOldPal)//�ָ���ɫ��  
	{  
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);  
		RealizePalette(hDC);  
		::ReleaseDC(NULL, hDC);  
	}     

	BITMAPFILEHEADER   bmfHdr; //λͼ�ļ�ͷ�ṹ       
	bmfHdr.bfType = 0x4D42;  // "BM"  ����λͼ�ļ�ͷ  
	DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;    
	bmfHdr.bfSize = dwDIBSize;  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  

	HANDLE hFile = CreateFile((LPCSTR)name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);//����λͼ�ļ�     
	DWORD dwWritten;  
	WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);   // д��λͼ�ļ�ͷ  
	WriteFile(hFile, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);// д��λͼ�ļ���������  

	GlobalUnlock(hDib);   //���     
	GlobalFree(hDib);  
	CloseHandle(hFile);       

}  

IplImage* CAlgorithm::Bitmap2IplImage(HBITMAP _hBitmap)
{
	BITMAP bmp;    
	GetObject(_hBitmap, sizeof(BITMAP), &bmp);    

	// get channels which equal 1 2 3 or 4    
	// bmBitsPixel :   
	// Specifies the number of bits    
	// required to indicate the color of a pixel.    
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;   

	// get depth color bitmap or grayscale   
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    


	// create header image   
	IplImage* img = cvCreateImage(cvSize(bmp.bmWidth,bmp.bmHeight), depth, nChannels);    

	// allocat memory for the pBuffer   
	BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];    

	// copies the bitmap bits of a specified device-dependent bitmap into a buffer   
	GetBitmapBits(_hBitmap, bmp.bmHeight*bmp.bmWidth*nChannels, pBuffer);    

	// copy data to the imagedata   
	memcpy(img->imageData, pBuffer, bmp.bmHeight*bmp.bmWidth*nChannels);   
	delete pBuffer;    

	// create the image   
	IplImage *dst = cvCreateImage(cvGetSize(img), img->depth, 3);   
	// convert color   
	cvCvtColor(img, dst, CV_BGRA2BGR);   
	cvReleaseImage(&img);  

	return dst;   
}
