#include "StdAfx.h"
#include "fqjs.h"

Cfqjs::Cfqjs(void)
{
}

Cfqjs::Cfqjs(CWnd* pOwner, HWND hcapture, CcanvasDlg *canvas, CPoint base)
{
	m_pOwner = pOwner;
	m_hcapture = hcapture;
	m_canvas = canvas;
	m_base = base;

	StartCapture();
}


Cfqjs::~Cfqjs(void)
{
	StopCapture();
}

BOOL Cfqjs::StartCapture()
{
	m_thread_enable = TRUE;
	m_thread_exit = FALSE;
	if (!(m_thread = AfxBeginThread(MainThread, this)))
		return FALSE;
	return TRUE;
}

BOOL Cfqjs::StopCapture()
{
	m_thread_enable = FALSE;
	while(!m_thread_exit)
		::Sleep(10);

	return TRUE;	
}


UINT Cfqjs::MainThread(LPVOID pParam)
{
	Cfqjs *pfqjs = (Cfqjs*)pParam;

	IplImage *pImageSrc = NULL;

	if(pfqjs->m_hcapture != NULL){
		HBITMAP pbmp = pfqjs->CaptureBmp(pfqjs->m_hcapture);
		pfqjs->SaveBmp(pbmp, CConfig::GetCurrentPath()+"1.bmp");
		if(pfqjs->m_canvas != NULL){
			pfqjs->m_canvas->SetBitmap(pbmp);
		}
		pImageSrc = pfqjs->Bitmap2IplImage(pbmp);

		::DeleteObject(pbmp);
	}
#ifdef _OPENCV_DEBUG_
	else{
		pImageSrc = cvLoadImage(CConfig::GetCurrentPath()+"1.bmp", CV_LOAD_IMAGE_UNCHANGED); 
	}
#endif
	
	if(pImageSrc != NULL){
		::SendMessage(pfqjs->m_pOwner->m_hWnd, WM_ALGM_OPENCV_WIN_NEW, (WPARAM)"1", 0);
		cvShowImage("1", pImageSrc);
	}
	
	while(pfqjs->m_thread_enable) 
		::Sleep(100);

	if(pImageSrc != NULL){
		::PostMessage(pfqjs->m_pOwner->m_hWnd, WM_ALGM_OPENCV_WIN_DESTROY, (WPARAM)"1", 0);
		cvReleaseImage(&(pImageSrc)); 
	}

	pfqjs->m_thread_exit = TRUE;
	return TRUE;
}