#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "resource.h"
#include "canvasDlg.h"
#include "config.h"
#include "EventTable.h"
#include <opencv2/opencv.hpp>  

#define WM_ALGM_OPENCV_WIN_NEW      WM_USER+150
#define WM_ALGM_OPENCV_WIN_DESTROY  WM_USER+151


#define _OPENCV_DEBUG_

class CAlgorithm
{
public:
	CAlgorithm();
	virtual ~CAlgorithm();

	virtual void SetAlgorithmEvent(int event, WPARAM wParam, LPARAM lParam);

protected:
	CWnd*       m_pOwner;			// owner window
	HWND        m_hcapture;
	CcanvasDlg *m_canvas;
	CPoint      m_base;

	HBITMAP CaptureBmp(HWND _capture);					// bitmap句柄需要自行释放
	void SaveBmp(HBITMAP hBitmap, CString name);

	IplImage* Bitmap2IplImage(HBITMAP _hBitmap);
};


#endif