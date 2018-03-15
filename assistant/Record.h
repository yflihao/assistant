#pragma once

#include "EventTable.h"

class CRecord
{
public:
	CRecord(void);
	CRecord(CString filepathname);
	~CRecord(void);

	BOOL SetRecord(int event, WPARAM wp, LPARAM lp);

private:
	CString     m_script_path;
	CStdioFile  m_script_file;

	int m_last_tick;
	int m_current_tick;
};

