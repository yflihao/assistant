#include "StdAfx.h"
#include "Script.h"

#define SLEEPUNIT 100
#define SLEEPINTERVAL  (1000/SLEEPUNIT)		// 单位 ms


float speedarray[7] = {0.1, 0.3, 0.5, 1, 1.5, 2, 3};

CScript::CScript(void)
{
}

CScript::CScript(CWnd* wnd, CString path, int speed)
{
	m_pOwner = wnd;

	path.Replace(' ', '\ ');
	m_script_path = path;

	m_thread = NULL;
	m_thread_enable = FALSE;

	m_exe_count = 0;
	m_exe_percent = 0;

	if(speed >= 0 || speed < (sizeof(speedarray)/sizeof(speedarray[0])))
		m_exe_speed = speedarray[speed];
	else
		m_exe_speed = 1;

#ifdef _SCRIPT_READ_PROGRESS_
	m_read_size = 0;
	m_script_size = 0;
#endif
}


CScript::~CScript(void)
{
}

BOOL CScript::StartScript()
{
	m_thread_enable = TRUE;
	m_last_tick = GetTickCount();	// 开始后立即计时
	if (!(m_thread = AfxBeginThread(MainThread, this)))
		return FALSE;
	return TRUE;
}

BOOL CScript::StopScript()
{
	m_thread_enable = FALSE;
	return TRUE;	
}

UINT CScript::MainThread(LPVOID pParam)
{
	int cmd;
	int time;
	char status[1024];
	CString cstrLine;
	CStringArray cstrarray;
	CScript* script = (CScript*)pParam;

	
#ifdef _SCRIPT_READ_PROGRESS_
	CFile   h_file;
	if(!(h_file.Open(script->m_script_path, CFile::modeRead | CFile::typeBinary))) 
	{
		AfxMessageBox("Open file failed!");
		::SendMessage(script->m_pOwner->m_hWnd, WM_SC_END, (WPARAM)0, (LPARAM)0);
		return FALSE;
	}
	h_file.SeekToEnd();
	script->m_script_size = h_file.GetLength();
	h_file.SeekToBegin();
	h_file.Close();
#endif

	// 读取文件
	if(!(script->m_script_file.Open(script->m_script_path, CFile::modeRead | CFile::typeBinary))) 
	{
		AfxMessageBox("Open script file failed!");
		::SendMessage(script->m_pOwner->m_hWnd, WM_SC_END, (WPARAM)0, (LPARAM)0);
		return FALSE;
	}

	script->m_script_file.SeekToBegin();

	while (script->m_script_file.ReadString(cstrLine))
	{
		cstrarray.RemoveAll();
		if(!script->parser(cstrLine, &cstrarray)){
			sprintf(status, "parser script failed, %s", cstrLine);
			AfxMessageBox(status);
			script->m_script_file.Close();
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_END, (WPARAM)0, (LPARAM)0);
			return FALSE;
		}

		time = _ttoi(cstrarray.GetAt(0));
		cmd = _ttoi(cstrarray.GetAt(1));

		script->m_current_tick = GetTickCount();
		
		while((script->m_current_tick - script->m_last_tick) < (int)(time * script->m_exe_speed)){
			::Sleep(SLEEPINTERVAL);
			script->m_current_tick = GetTickCount();

			if(!script->m_thread_enable){
				script->m_script_file.Close();
				return TRUE;
			}
		}

		script->m_last_tick = script->m_current_tick;
		script->m_exe_count++;

#ifdef _SCRIPT_READ_PROGRESS_
		script->m_read_size += (cstrLine.GetLength() * sizeof(TCHAR) + sizeof('\r'));
		script->m_exe_percent = ((float)script->m_read_size/(float)script->m_script_size) * 100;
#endif


		if(cmd == BUTTON_L_DOWN){
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_BUTTON_L_DOWN, (WPARAM)_ttoi(cstrarray.GetAt(2)), (LPARAM)_ttoi(cstrarray.GetAt(3)));
		}else if(cmd == BUTTON_L_UP){
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_BUTTON_L_UP, (WPARAM)_ttoi(cstrarray.GetAt(2)), (LPARAM)_ttoi(cstrarray.GetAt(3)));
		}else if(cmd == BUTTON_R_DOWN){
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_BUTTON_R_DOWN, (WPARAM)_ttoi(cstrarray.GetAt(2)), (LPARAM)_ttoi(cstrarray.GetAt(3)));
		}else if(cmd == BUTTON_R_UP){
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_BUTTON_R_UP, (WPARAM)_ttoi(cstrarray.GetAt(2)), (LPARAM)_ttoi(cstrarray.GetAt(3)));
		}else if(cmd == KEY_DOWN){
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_KEYDOWN, (WPARAM)_ttoi(cstrarray.GetAt(2)), (LPARAM)(LPCTSTR)(cstrarray.GetAt(3)));
		}
		else{
			sprintf(status, "script unkown cmd, %d", cmd);
			AfxMessageBox(status);
			script->m_script_file.Close();
			::SendMessage(script->m_pOwner->m_hWnd, WM_SC_END, (WPARAM)0, (LPARAM)0);
			return FALSE;
		}
	}

	script->m_script_file.Close();

	::SendMessage(script->m_pOwner->m_hWnd, WM_SC_END, (WPARAM)0, (LPARAM)0);

	return TRUE;
}

BOOL CScript::parser(CString src, CStringArray *dst)
{
	int find = 0;
	BOOL ret = FALSE;

	src.Remove('\r');
	while((find = src.Find(',')) >= 0){
		ret = TRUE;
		dst->Add(src.Mid(0, find));
		src = src.Mid(find+1);
	}
	dst->Add(src);

	return ret;
}

int CScript::getEXEcount()
{
	return m_exe_count;
}

int CScript::getEXEpercent()
{
	return m_exe_percent;
}

BOOL CScript::setEXEspeed(int speed)
{
	if(speed >= 0 || speed < (sizeof(speedarray)/sizeof(speedarray[0]))){
		m_exe_speed = speedarray[speed];
		return TRUE;
	}
	else{
		m_exe_speed = 1;
		return FALSE;
	}
}


