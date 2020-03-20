// EventLog.h: Schnittstelle für die Klasse CEventLog.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTLOG_H__7D48CC33_4E41_4E0C_B16A_5FC714CAC457__INCLUDED_)
#define AFX_EVENTLOG_H__7D48CC33_4E41_4E0C_B16A_5FC714CAC457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>

class CEventLog : public CObject  
{
  DECLARE_DYNCREATE(CEventLog)
public:
	CEventLog();
	virtual ~CEventLog();

	HANDLE GetHandle(){return m_hLog;};

	BOOL FireWithData(WORD wType, WORD wCategory, DWORD dwEventID, DWORD dwData, LPVOID ptr, ...);
	BOOL Fire(WORD wType, WORD wCategory, DWORD dwEventID, ...);

  BOOL Initialize(CString csApp);
	CString LoadMessage( DWORD dwMsgId, ... );
	BOOL LaunchViewer(void);

	DWORD AddEventSource(CString csName, DWORD dwCategoryCount = 0);
  DWORD RemoveEventSource(CString csApp);

protected:
  PSID GetUserSID(PSID * ppSid);
	HANDLE m_hLog;
};

extern CEventLog theEventLog;
#endif // !defined(AFX_EVENTLOG_H__7D48CC33_4E41_4E0C_B16A_5FC714CAC457__INCLUDED_)
