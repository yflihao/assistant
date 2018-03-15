#pragma once


// CcanvasDlg �Ի���

#define WM_CHILD_EXIT_CANVAS  WM_USER+250

typedef BOOL (WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);

class CcanvasDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CcanvasDlg)

public:
	CcanvasDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CcanvasDlg();

// �Ի�������
	enum { IDD = IDD_CANVAS_DIALOG };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	BOOL SetOpacity(int value);         // ����͸����
	BOOL SetDraw(int x, int y);         // ��ʵ��Բ
	BOOL SetBitmap(HBITMAP _pbmp);      // ��ʾͼƬ

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

private:
	CWnd* m_parent;						// �����ھ��

	HINSTANCE  m_hInst;					// ͸����
	MYFUNC  SetLayeredWindowAttributes;

	CPoint m_point;						// �Ҽ��϶�
	BOOL m_bIsMoving;

	HBITMAP m_hBitmap;					// Ҫ��ʾ��ͼƬ���
};
