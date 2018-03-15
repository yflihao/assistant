// canvasDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "assistant.h"
#include "canvasDlg.h"
#include "afxdialogex.h"


// CcanvasDlg �Ի���

IMPLEMENT_DYNAMIC(CcanvasDlg, CDialogEx)

CcanvasDlg::CcanvasDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcanvasDlg::IDD, pParent)
{
	m_parent = pParent;

	// ��Դ��ʼ��
	m_hBitmap = NULL;

	// �Ҽ��ƶ�
	m_bIsMoving = FALSE;
	m_point = 0;

	// ���ؿ�
	m_hInst = LoadLibrary("User32.DLL");    
	if(m_hInst != NULL)   
	{   
		SetLayeredWindowAttributes = (MYFUNC)GetProcAddress(m_hInst, "SetLayeredWindowAttributes");   
	}
}

CcanvasDlg::~CcanvasDlg()
{
	// �ͷſ⼰�⺯��
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


// CcanvasDlg ��Ϣ�������

void CcanvasDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nID == SC_CLOSE)
		::PostMessage(m_parent->m_hWnd, WM_CHILD_EXIT_CANVAS, 0, 0);

	CDialogEx::OnSysCommand(nID, lParam);
}

BOOL CcanvasDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �޿�
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(this->GetSafeHwnd(),GWL_STYLE)& (WS_SYSMENU | WS_CLIPCHILDREN| WS_CLIPSIBLINGS)); 
	// �ö�
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	// ͸����
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)^0x80000);  

	SetLayeredWindowAttributes(m_hWnd, 0, 180, 2);    

	return TRUE;
}

void CcanvasDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������

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

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CcanvasDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bIsMoving = TRUE;
	SetCapture();	   //���������Ϣ,������곬������Ҳ���Բ���
	m_point = point;   //��¼����ʱ������

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CcanvasDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(m_bIsMoving)
	{
		//ƫ����
		int dx = point.x - m_point.x;
		int dy = point.y - m_point.y;

		//���������˸,����ʹdx,dy����1���ƶ�
		CRect rect;
		GetWindowRect(&rect);
		OffsetRect(&rect, dx, dy);	//��������ƫ��dx,dy
		MoveWindow(&rect);			//�ƶ�����
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CcanvasDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

	// TODO: �ڴ˴������Ϣ����������

	//�������ʧȥ����,��ֹͣ�ƶ�
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
