// canvasDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "assistant.h"
#include "canvasDlg.h"
#include "afxdialogex.h"


// CcanvasDlg 对话框

IMPLEMENT_DYNAMIC(CcanvasDlg, CDialogEx)

CcanvasDlg::CcanvasDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcanvasDlg::IDD, pParent)
{
	m_parent = pParent;

	// 资源初始化
	m_hBitmap = NULL;

	// 右键移动
	m_bIsMoving = FALSE;
	m_point = 0;

	// 加载库
	m_hInst = LoadLibrary("User32.DLL");    
	if(m_hInst != NULL)   
	{   
		SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(m_hInst, "SetLayeredWindowAttributes");   
	}
}

CcanvasDlg::~CcanvasDlg()
{
	// 释放库及库函数
	if(m_hInst != NULL){
		FreeLibrary(m_hInst);   
		m_hInst = NULL;
		SetLayeredWindowAttributes = NULL;
	}
}

void CcanvasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CcanvasDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CcanvasDlg 消息处理程序

void CcanvasDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID == SC_CLOSE)
		::PostMessage(m_parent->m_hWnd, WM_CHILD_EXIT_CANVAS, 0, 0);

	CDialogEx::OnSysCommand(nID, lParam);
}

BOOL CcanvasDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 无框
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(this->GetSafeHwnd(),GWL_STYLE)& (WS_SYSMENU | WS_CLIPCHILDREN| WS_CLIPSIBLINGS)); 
	// 置顶
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	// 透明度
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)^0x80000);  

	SetLayeredWindowAttributes(m_hWnd, 0, 180, 2);    

	return TRUE;
}

void CcanvasDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	if(m_hBitmap != NULL){
		CDC MemDC; 
		if (!MemDC.CreateCompatibleDC(&dc))
			return;
		HBITMAP OldBitmap = (HBITMAP)MemDC.SelectObject(m_hBitmap); 

		BITMAP bm;
		::GetObject(m_hBitmap, sizeof(bm), (LPSTR)&bm);    
		CRect rect;
		GetClientRect(&rect);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		MemDC.SelectObject(OldBitmap); 
		::DeleteDC(MemDC); 
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	// 不为绘图消息调用 CDialogEx::OnPaint()
}

void CcanvasDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bIsMoving = TRUE;
	SetCapture();	   //捕获鼠标消息,就算鼠标超过窗口也可以捕获到
	m_point = point;   //记录按下时的坐标

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CcanvasDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_bIsMoving)
	{
		//偏移量
		int dx = point.x - m_point.x;
		int dy = point.y - m_point.y;

		//如果窗口闪烁,可以使dx,dy大于1再移动
		CRect rect;
		GetWindowRect(&rect);
		OffsetRect(&rect, dx, dy);	//整个窗口偏移dx,dy
		MoveWindow(&rect);			//移动窗口
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CcanvasDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_bIsMoving)
	{
		m_bIsMoving = FALSE;
		ReleaseCapture();
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CcanvasDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码

	//如果窗口失去焦点,则停止移动
	if (nState == WA_INACTIVE && m_bIsMoving)
	{
		m_bIsMoving = FALSE;
		ReleaseCapture();
	}
}


BOOL CcanvasDlg::SetOpacity(int value)
{
	if(SetLayeredWindowAttributes != NULL)   
	{   
		SetLayeredWindowAttributes(m_hWnd, 0, value, 2);    
		return TRUE;
	}

	return FALSE;
}


BOOL CcanvasDlg::SetDraw(int x, int y)
{

	return TRUE;
}

BOOL CcanvasDlg::SetBitmap(HBITMAP _pbmp)
{
	BITMAP bm = {0};
	::GetObject(_pbmp, sizeof(bm), &bm);
	MoveWindow(0, 0, bm.bmWidth, bm.bmHeight);

	CDC sourceDC;
	CDC destDC;
	sourceDC.CreateCompatibleDC(NULL);
	destDC.CreateCompatibleDC(NULL);

	m_hBitmap = ::CreateCompatibleBitmap(CClientDC(NULL), bm.bmWidth, bm.bmHeight); 
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(sourceDC.m_hDC, _pbmp);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(destDC.m_hDC, m_hBitmap);
	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY); 

	::SelectObject(sourceDC.m_hDC, hbmOldSource);
	::SelectObject(destDC.m_hDC, hbmOldDest);
	::DeleteDC(sourceDC.m_hDC);
	::DeleteDC(destDC.m_hDC); 

	Invalidate();

	return TRUE;
}
