#pragma once
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

class CProgramLog
{
public:
	CProgramLog();
	virtual ~CProgramLog();
public:
	void Log(int nlevel, const char *szText, ...);
	void MultithreadingOn();
	void MultithreadingOff();
	void ShowPrintOn();
	void ShowPrintOff();
	void ClearLog();
	time_t SystemTimeToTimeT(SYSTEMTIME& st);
private:
	void LogFileWrite(char* logStr, int len);
protected:
	CRITICAL_SECTION m_kCritical;
	DWORD m_dwLogCount;
	bool m_bShowPrint;
	bool m_bMultithreading;
	char m_szBuffer[1024 * 6];
	DWORD m_dwLastClearTime;
private:
	CProgramLog(const CProgramLog &object);
	CProgramLog &operator= (const CProgramLog &object);
	DWORD GetCountTimer();
};

CProgramLog* __ProgramLogMgr();

#define LOG_ERROR(fmt, ...)       __ProgramLogMgr()->Log(0, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...)       __ProgramLogMgr()->Log(1, fmt, __VA_ARGS__)