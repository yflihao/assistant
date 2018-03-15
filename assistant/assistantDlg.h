
// assistantDlg.h : ͷ�ļ�
//

#pragma once

#include "Script.h"
#include "Record.h"
#include "Config.h"
#include "Param.h"
#include "canvasDlg.h"
#include "algorithm.h"
#include "fqjs.h"

// CassistantDlg �Ի���
class CassistantDlg : public CDialogEx
{
// ����
public:
	CassistantDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CassistantDlg(void);

// �Ի�������
	enum { IDD = IDD_ASSISTANT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	static BOOL LoadDll(HINSTANCE* _hInstDll, HWND _hWnd);
	static BOOL DelDLL(HINSTANCE* _hInstDll);

protected:
	
	/***********************   ϵͳ��Ϣ����  *************************/
	afx_msg LRESULT OnMsgMouseLDowm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgMouseLUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgMouseMove(WPARAM wParam, LPARAM lParam);

	/***********************   �ڲ���Ϣ����  *************************/
	afx_msg LRESULT OnAlgmOpencvWinNew(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlgmOpencvWinDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScButtonLDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScButtonLUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScEventEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildExitCanvas(WPARAM wParam, LPARAM lParam);

	/***********************   �û���Ϣ����  *************************/
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedParam();
	afx_msg void OnBnClickedConfig();
	afx_msg void OnBnClickedCapture();
	afx_msg void OnBnClickedDebug();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedBrowser();
	afx_msg void OnBnClickedRun();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	BOOL m_debug_enable;
	BOOL m_game_enable;
	BOOL m_script_enable;
	BOOL m_record_enable;
	BOOL m_capture_enable;

	CScript *m_sc;
	CRecord *m_rec;
	CConfig *m_cfg;
	CParam  *m_pm;
    CAlgorithm *m_algm;
	CcanvasDlg *m_canvas;

	HWND m_hcap;				// ץͼ���ھ��
	HINSTANCE m_hInstDll;       // ���ӿ���

	BOOL ClickIsValid(int x, int y);
	VOID MakeMouseClick(int event, int x, int y);
	VOID MakeMouseClickEx(int event, int x, int y);

	BOOL m_multiple;			//���ն�		 
	BOOL TermIsMultiple(); 		
};
