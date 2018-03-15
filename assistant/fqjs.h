#pragma once
#include "algorithm.h"

class Cfqjs :
	public CAlgorithm
{
public:
	Cfqjs(void);
	Cfqjs(CWnd* pOwner, HWND hcapture, CcanvasDlg *canvas, CPoint base);
	~Cfqjs(void);

	CWinThread *m_thread;
	BOOL m_thread_enable;
	BOOL m_thread_exit;
	static UINT	MainThread(LPVOID pParam);

	BOOL StartCapture();
	BOOL StopCapture();
};

