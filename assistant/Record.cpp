#include "StdAfx.h"
#include "Record.h"


CRecord::CRecord(void)
{
}

CRecord::CRecord(CString filepathname)
{
	m_script_path = filepathname;
	m_last_tick = GetTickCount();
	m_script_file.Open(m_script_path, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
}


CRecord::~CRecord(void)
{
	m_script_file.Close();
}

BOOL CRecord::SetRecord(int event, WPARAM wp, LPARAM lp)
{
	CString WriteCstring;
	m_current_tick = GetTickCount();

	switch (event)
	{
	case BUTTON_L_DOWN:
	case BUTTON_L_UP:
	case BUTTON_R_DOWN:
	case BUTTON_R_UP:
		WriteCstring.Format("%d,%d,%d,%d\r\n", m_current_tick - m_last_tick, event, wp, lp);
		break;
	default:
		break;
	}
	
	m_last_tick = m_current_tick;
	m_script_file.WriteString(WriteCstring);
	
	return TRUE;
}
