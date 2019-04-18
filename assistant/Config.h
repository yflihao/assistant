#pragma once

typedef struct _MouseCalibration{
	BOOL enable;
	CPoint pos;
}MouseCalibration;

// Config �Ի���

class CConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CConfig)

public:
	CConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfig();

// �Ի�������
	enum { IDD = IDD_CONFIG };

	int getMouseSize();
	MouseCalibration *getMouseConfig(int index);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	static CString GetCurrentPath();

protected:
	/***********************   ϵͳ��Ϣ����  *************************/
	afx_msg LRESULT OnMsgMouseLDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	/***********************   �û���Ϣ����  *************************/
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton8();

private:
	int      mode;
	CString  last;
	HINSTANCE m_hInstDll;

	CStdioFile  m_config_file;
	MouseCalibration m_mouse[3];

	VOID ReadConfig();
	VOID WriteConfig();
	VOID SyncConfig();

	BOOL LoadDll(HINSTANCE* _hInstDll);
	BOOL DelDLL(HINSTANCE* _hInstDll);
	BOOL ClickIsValid(int x, int y);
};
