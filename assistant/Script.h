#pragma once

#include "EventTable.h"

#define WM_SC_BUTTON_L_DOWN			WM_USER+200
#define WM_SC_BUTTON_L_UP			WM_USER+201
#define WM_SC_BUTTON_R_DOWN		    WM_USER+202
#define WM_SC_BUTTON_R_UP			WM_USER+203
#define WM_SC_END					WM_USER+204

#define _SCRIPT_READ_PROGRESS_


class CScript
{
	
public:
	CScript(void);
	CScript(CWnd* wnd, CString path, int speed);
	~CScript(void);

	int getEXEcount();
	int getEXEpercent();
	BOOL setEXEspeed(int speed);

	BOOL StartScript();
	BOOL StopScript();

	static BOOL parser(CString src, CStringArray *dst);

private:
	CWnd*  m_pOwner;

	CString     m_script_path;
	CStdioFile  m_script_file;

	int m_last_tick;
	int m_current_tick;

	int m_exe_count;
	int m_exe_percent;
	float m_exe_speed;

#ifdef _SCRIPT_READ_PROGRESS_
	UINT m_read_size;
	UINT m_script_size;
#endif

	CWinThread *m_thread;
	BOOL m_thread_enable;
	static UINT	MainThread(LPVOID pParam);
};

