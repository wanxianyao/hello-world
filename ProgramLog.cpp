#include "stdafx.h"
#include "ProgramLog.h"

CProgramLog::CProgramLog()
{
	m_dwLastClearTime = 0;
	m_bShowPrint = true;
	InitializeCriticalSection(&m_kCritical);
	char buf[128];
	char tmpTime[32];
	char tmpDate[32];
	_strtime(tmpTime);
	_strdate(tmpDate);

	sprintf(buf, "日志文件开始时间: 【%s %s】\r\n", tmpDate, tmpTime );
	LogFileWrite(buf, strlen(buf));
}

CProgramLog::~CProgramLog()
{
	DeleteCriticalSection(&m_kCritical);
	char buf[128];
	char tmpTime[32];
	char tmpDate[32];
	_strtime( tmpTime );
	_strdate( tmpDate );

	sprintf( buf, "日志文件结束时间: 【%s %s】\r\n", tmpDate, tmpTime );
	LogFileWrite(buf, strlen(buf));
}

void CProgramLog::LogFileWrite(char* logStr, int len)
{
	char lpfile[1024]={0};
	GetModuleFileNameA(NULL,lpfile,1024);
	(strrchr(lpfile, '\\'))[1] = 0;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strcat(lpfile,"程序日志文件");
	CreateDirectoryA(lpfile,NULL);
	char str[128]={0};
	sprintf(str,"\\%04d年%02d月%02d日.txt",st.wYear, st.wMonth, st.wDay);
	strcat(lpfile,str);

	FILE* pf = fopen(lpfile, "a+");
	if (pf != NULL)
	{
		fwrite(logStr, 1, len, pf);
		fclose(pf);
	}
	ClearLog();
}

DWORD CProgramLog::GetCountTimer()
{
	//LARGE_INTEGER Frequency;
	//QueryPerformanceCounter(&Frequency);
	//return (DWORD)Frequency.QuadPart;

	return GetTickCount();
}

void CProgramLog::ClearLog()
{
	//每间隔24小时清理一次
	if (GetCountTimer() - m_dwLastClearTime >= 1000 * 60 * 60 * 24)
	{
		m_dwLastClearTime = GetCountTimer();
		char lpfile[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, lpfile, MAX_PATH);
		(strrchr(lpfile, '\\'))[1] = 0;
		SYSTEMTIME st;
		GetLocalTime(&st);
		strcat(lpfile, "程序日志文件\\*");
		CreateDirectoryA(lpfile, NULL);

		WIN32_FIND_DATAA findFileData;
		HANDLE hFind = ::FindFirstFileA(lpfile, &findFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			char szFileName[MAX_PATH] = { 0 };
			strcpy(szFileName, lpfile);
			BOOL find = TRUE;
			while (find == TRUE)
			{
				memset(szFileName, 0, MAX_PATH);
				strcat(szFileName, findFileData.cFileName);
				if (strlen(findFileData.cFileName) > 2)
				{
					SYSTEMTIME createTime1, createTime2;
					FileTimeToSystemTime(&findFileData.ftCreationTime, &createTime1);
					SystemTimeToTzSpecificLocalTime(NULL, &createTime1, &createTime2);
					time_t fileCreateTime = SystemTimeToTimeT(createTime2);
					time_t currentTime = SystemTimeToTimeT(st);
					//如果一个日志文件创建到现在有30天了，那么删除
					if (currentTime - fileCreateTime > 60 * 60 * 24 * 30)
						remove(szFileName);
				}
				find = FindNextFileA(hFind, &findFileData);
			}
			FindClose(hFind);
		}
	}
}

time_t CProgramLog::SystemTimeToTimeT(SYSTEMTIME& st)
{
	struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
	return mktime(&gm);
}

void CProgramLog::Log(int nlevel, const char *szText, ... )
{
	
#ifndef NO_PROGRAM_LOG
	if(m_bMultithreading)
		EnterCriticalSection(&m_kCritical);
	++m_dwLogCount;

	int iPos = 0;
	iPos += sprintf( m_szBuffer + iPos,"【%05d】", m_dwLogCount );
	char tmpTime[32];
	_strtime( tmpTime );
	iPos += sprintf( m_szBuffer + iPos, "【%s】 ", tmpTime );

	va_list args;
	va_start( args, szText );
	iPos += _vsnprintf( m_szBuffer + iPos,6*1024 - 50, szText, args );
	m_szBuffer[iPos] = '\r';
	m_szBuffer[iPos + 1] = '\n';
	m_szBuffer[iPos + 2] = '\0';

	//m_szBuffer[iPos] = '\n';
	//m_szBuffer[iPos + 1] = '\0';

	if(m_bShowPrint)
		printf(m_szBuffer);
#ifdef WIN32
	OutputDebugStringA(m_szBuffer);
#endif
	if (nlevel < 1)
	{
		LogFileWrite(m_szBuffer, iPos + 2);
	}
	
	va_end( args );

	if(m_bMultithreading)
		LeaveCriticalSection(&m_kCritical);
#endif
}

void CProgramLog::MultithreadingOn()
{
	m_bMultithreading = true;
}

void CProgramLog::MultithreadingOff()
{
	m_bMultithreading = false;
}

void CProgramLog::ShowPrintOn()
{
	m_bShowPrint = true;
}

void CProgramLog::ShowPrintOff()
{
	m_bShowPrint = false;
}

CProgramLog* __ProgramLogMgr()
{
	static CProgramLog s_kProgramLog;
	return &s_kProgramLog;
}