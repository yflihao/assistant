// CConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "assistant.h"
#include "assistantDlg.h"
#include "Config.h"
#include "afxdialogex.h"

#define WM_MSG_MOUSE_L_DOWM   WM_USER + 305
#define WM_MSG_MOUSE_L_UP     WM_USER + 306
#define WM_MSG_MOUSE_MOVE     WM_USER + 309

#define FILENAME "assistant.cfg"

// CConfig 对话框

IMPLEMENT_DYNAMIC(CConfig, CDialogEx)

CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfig::IDD, pParent)
{
	ReadConfig();
}

CConfig::~CConfig()
{
	WriteConfig();
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfig, CDialogEx)
	ON_MESSAGE(WM_MSG_MOUSE_L_DOWM, &CConfig::OnMsgMouseLDowm)
	ON_MESSAGE(WM_MSG_MOUSE_MOVE, &CConfig::OnMsgMouseMove)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfig::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CConfig::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &CConfig::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CConfig::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CConfig::OnBnClickedButton6)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CConfig 消息处理程序

BOOL CConfig::OnInitDialog()
{
	char status[20];
	mode = -1;

	for(int i=0; i<sizeof(m_mouse)/sizeof(m_mouse[0]); i++)
	{
		sprintf(status, "%d,%d", m_mouse[i].pos.x, m_mouse[i].pos.y);
		GetDlgItem(IDC_EDIT1+i)->SetWindowText(status);
		((CButton *)GetDlgItem(IDC_CHECK1+i))->SetCheck(m_mouse[i].enable == 1);
	}

	return TRUE;
}

HBRUSH CConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if (nCtlColor == CTLCOLOR_EDIT)
	{
		if(mode >= 0){
			pDC-> SetTextColor(RGB(255,0,0));  
		}
		else{
			pDC-> SetTextColor(RGB(0,0,0)); 
		}
	}

	return hbr;
}

int CConfig::getMouseSize()
{
	return (sizeof(m_mouse)/sizeof(m_mouse[0]));
}

MouseCalibration * CConfig::getMouseConfig(int index)
{
	if(index >= 0 && index < (sizeof(m_mouse)/sizeof(m_mouse[0])))
		return &m_mouse[index];
	else
		return NULL;
}

VOID CConfig::ReadConfig()
{
	CString ReadString;
	CStringArray ParserArray;
	CString filepath = GetCurrentPath();
	if(m_config_file.Open(filepath+FILENAME, CFile::modeRead | CFile::typeBinary)) 
	{
		while (m_config_file.ReadString(ReadString))
		{
			ParserArray.RemoveAll();
			if(!CScript::parser(ReadString, &ParserArray))
				continue;

			int index = _ttoi(ParserArray.GetAt(0));
			if(index >= 0 && index < (sizeof(m_mouse)/sizeof(m_mouse[0])))
			{
				m_mouse[index].enable = _ttoi(ParserArray.GetAt(1));
				m_mouse[index].pos.x = _ttoi(ParserArray.GetAt(2));
				m_mouse[index].pos.y = _ttoi(ParserArray.GetAt(3));
			}
		}

		m_config_file.Close();
	}

	m_mouse[0].enable = 1; //终端1默认必须使能
}

VOID CConfig::WriteConfig()
{
	CString WriteCstring;
	CString FilePath = GetCurrentPath();
	if(m_config_file.Open(FilePath+FILENAME, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) 
	{
		for(int i=0; i<sizeof(m_mouse)/sizeof(m_mouse[0]); i++)
		{
			WriteCstring.Format("%d,%d,%d,%d\r\n", i, (m_mouse[i].enable==1?1:0), m_mouse[i].pos.x, m_mouse[i].pos.y);
			m_config_file.WriteString(WriteCstring);
		}
		m_config_file.Close();
	}
}

VOID CConfig::SyncConfig()
{
	CString tmpString;
	CStringArray vlString;
	for(int i=0; i<sizeof(m_mouse)/sizeof(m_mouse[0]); i++)
	{
		vlString.RemoveAll();
		GetDlgItem(IDC_EDIT1+i)->GetWindowText(tmpString);
		if(CScript::parser(tmpString, &vlString))
		{
			m_mouse[i].pos.x = _ttoi(vlString.GetAt(0));
			m_mouse[i].pos.y = _ttoi(vlString.GetAt(1));
		}
		m_mouse[i].enable = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1+i)? 1 : 0);
	}
}

afx_msg LRESULT CConfig::OnMsgMouseLDowm(WPARAM wParam, LPARAM lParam)
{
	char status[20];

	if(ClickIsValid(LOWORD(lParam), HIWORD(lParam))){
		sprintf(status, "%d,%d", LOWORD(lParam), HIWORD(lParam));
		GetDlgItem(IDC_EDIT1+mode)->SetWindowText(status);
	}
	else{
		GetDlgItem(IDC_EDIT1+mode)->SetWindowText(last);
	}
	
	GetDlgItem(IDC_BUTTON1+mode)->EnableWindow(TRUE);
	CassistantDlg::DelDLL(&m_hInstDll);
	mode = -1;

	return 0;
}


afx_msg LRESULT CConfig::OnMsgMouseMove(WPARAM wParam, LPARAM lParam)
{
	char status[20];
	sprintf(status, "%d,%d",LOWORD(lParam), HIWORD(lParam));
	GetDlgItem(IDC_EDIT1+mode)->SetWindowText(status);

	return 0;
}

/************************************   user msg   ****************************************/

void CConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	if(CassistantDlg::LoadDll(&m_hInstDll, this->m_hWnd)){
		mode = 0;
		GetDlgItem(IDC_EDIT1)->GetWindowText(last);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
}

void CConfig::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CassistantDlg::LoadDll(&m_hInstDll, this->m_hWnd)){
		mode = 1;
		GetDlgItem(IDC_EDIT6)->GetWindowText(last);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	}
}


void CConfig::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CassistantDlg::LoadDll(&m_hInstDll, this->m_hWnd)){
		mode = 2;
		GetDlgItem(IDC_EDIT7)->GetWindowText(last);
		GetDlgItem(IDC_BUTTON6)->EnableWindow(FALSE);
	}
}

void CConfig::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	SyncConfig();
	EndDialog(IDOK);
}


void CConfig::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDCANCEL);
}

/************************************   public   *****************************************/

BOOL CConfig::ClickIsValid(int x, int y)
{
	CRect ClientRect;
	GetWindowRect(&ClientRect);
	CPoint pt(x, y);
	if (ClientRect.PtInRect(pt))
		return FALSE;
	else
		return TRUE;
}

CString CConfig::GetCurrentPath()
{
	CString filepath;
	GetModuleFileName(NULL, filepath.GetBuffer(256), 256);
	filepath.ReleaseBuffer(256);
	int nPos  = filepath.ReverseFind('\\');
	filepath = filepath.Left(nPos + 1);

	return filepath;
}


