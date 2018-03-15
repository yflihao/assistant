// Param.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "assistant.h"
#include "assistantDlg.h"
#include "Param.h"
#include "afxdialogex.h"


// CParam �Ի���

IMPLEMENT_DYNAMIC(CParam, CDialogEx)

CParam::CParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParam::IDD, pParent)
{
}

CParam::CParam(CWnd* pParent /*=NULL*/, CString filename)
	: CDialogEx(CParam::IDD, pParent)
{
	for(int i=0; i<MAX_PARAM_NUM; i++)
		m_param[i].Empty();

	m_file_name.Format("param_%s.pm", filename);
	ReadParam(m_file_name);
}

CParam::~CParam()
{
	WriteParam(m_file_name);
}

void CParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CParam::OnInitDialog()
{
	for(int i=0; i<MAX_PARAM_NUM; i++)
	{
		if(!m_param[i].IsEmpty())
			GetDlgItem(IDC_EDIT2+i)->SetWindowText(m_param[i]);
	}
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParam::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CParam::OnBnClickedCancel)
END_MESSAGE_MAP()

VOID CParam::ReadParam(CString name)
{
	CStdioFile h_pfile;
	CString ReadLine;
	CStringArray ParamLine;
	CString FilePath = CConfig::GetCurrentPath();
	if(h_pfile.Open(FilePath+name, CFile::modeRead | CFile::typeBinary)) 
	{
		while (h_pfile.ReadString(ReadLine))
		{
			ParamLine.RemoveAll();
			if(!CScript::parser(ReadLine, &ParamLine))
				continue;

			m_param[_ttoi(ParamLine.GetAt(0))] = ParamLine.GetAt(1);
		}

		h_pfile.Close();
	}
}

VOID CParam::WriteParam(CString name)
{
	CString ParamLine;
	CStdioFile  h_pfile;
	CString FilePath = CConfig::GetCurrentPath();
	if(h_pfile.Open(FilePath+name, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) 
	{
		for(int i=0; i<MAX_PARAM_NUM; i++){
			if(!m_param[i].IsEmpty()){
				ParamLine.Format("%d,%s\r\n", i, (LPSTR)(LPCSTR)m_param[i]);
				h_pfile.WriteString(ParamLine);
			}
		}

		h_pfile.Close();
	}
}


// CParam ��Ϣ�������


void CParam::OnBnClickedOk()
{	
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString EditString;
	for(int i=0; i<MAX_PARAM_NUM; i++)
	{
		GetDlgItem(IDC_EDIT2+i)->GetWindowText(EditString);
		m_param[i] = EditString;
	}

	CDialogEx::OnOK();
}


void CParam::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
