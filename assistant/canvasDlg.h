#pragma once


// CcanvasDlg 对话框

#define WM_CHILD_EXIT_CANVAS  WM_USER+250

typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);

class CcanvasDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CcanvasDlg)

public:
	CcanvasDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CcanvasDlg();

// 对话框数据
	enum { IDD = IDD_CANVAS_DIALOG };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL SetOpacity(int value);         // 设置透明度
	BOOL SetDraw(int x, int y);         // 画实心圆
	BOOL SetBitmap(HBITMAP _pbmp);      // 显示图片

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
	CWnd* m_parent;						// 父窗口句柄

	HINSTANCE  m_hInst;					// 透明度
	MYFUNC  SetLayeredWindowAttributes;

	CPoint m_point;						// 右键拖动
	BOOL m_bIsMoving;

	HBITMAP m_hBitmap;					// 要显示的图片句柄
};
