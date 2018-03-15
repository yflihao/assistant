
// assistantDlg.h : 头文件
//

#pragma once

#include "Script.h"
#include "Record.h"
#include "Config.h"
#include "Param.h"
#include "canvasDlg.h"
#include "algorithm.h"
#include "fqjs.h"

// CassistantDlg 对话框
class CassistantDlg : public CDialogEx
{
// 构造
public:
	CassistantDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CassistantDlg(void);

// 对话框数据
	enum { IDD = IDD_ASSISTANT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	static BOOL LoadDll(HINSTANCE* _hInstDll, HWND _hWnd);
	static BOOL DelDLL(HINSTANCE* _hInstDll);

protected:
	
	/***********************   系统消息处理  *************************/
	afx_msg LRESULT OnMsgMouseLDowm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgMouseLUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgMouseMove(WPARAM wParam, LPARAM lParam);

	/***********************   内部消息处理  *************************/
	afx_msg LRESULT OnAlgmOpencvWinNew(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlgmOpencvWinDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScButtonLDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScButtonLUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScEventEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChildExitCanvas(WPARAM wParam, LPARAM lParam);

	/***********************   用户消息处理  *************************/
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

	HWND m_hcap;				// 抓图窗口句柄
	HINSTANCE m_hInstDll;       // 钩子库句柄

	BOOL ClickIsValid(int x, int y);
	VOID MakeMouseClick(int event, int x, int y);
	VOID MakeMouseClickEx(int event, int x, int y);

	BOOL m_multiple;			//多终端		 
	BOOL TermIsMultiple(); 		
};
