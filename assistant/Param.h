#pragma once

#define MAX_PARAM_NUM 5

// CParam �Ի���

class CParam : public CDialogEx
{
	DECLARE_DYNAMIC(CParam)

public:
	CParam(CWnd* pParent = NULL);   // ��׼���캯��
	CParam(CWnd* pParent, CString filename);
	virtual ~CParam();

// �Ի�������
	enum { IDD = IDD_PARAM };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CString m_file_name;
	CString m_param[MAX_PARAM_NUM];

	VOID ReadParam(CString name);
	VOID WriteParam(CString name);
};
