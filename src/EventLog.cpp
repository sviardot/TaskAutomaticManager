// EventLog.cpp: Implementierung der Klasse CEventLog.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CEventLog, CObject)

CEventLog theEventLog;

CEventLog::CEventLog()
{
  m_hLog = NULL;
}

CEventLog::~CEventLog()
{
  if (m_hLog != NULL)
  {
    DeregisterEventSource(m_hLog);
    m_hLog = NULL;
  }
}


BOOL CEventLog::Initialize(CString csApp)
{
  // Try to add application to EventVwr
  if (AddEventSource(csApp, 3 ) != 0)
  {
    CString cs;
    cs.Format("Unable to register EventLog access for application %s.", cs);
    cs += "  Please log in with admin rights to do this.";
    cs += "  \nApplication will run without event logging";
    AfxMessageBox(cs, MB_ICONEXCLAMATION);
  }

  // Register to write
  m_hLog = ::RegisterEventSource( NULL, csApp);

  return TRUE;
}

DWORD CEventLog::AddEventSource(CString csName, DWORD dwCategoryCount)
{
	HKEY	hRegKey = NULL; 
	DWORD	dwError = 0;
	TCHAR	szPath[ MAX_PATH ];
	
	_stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), csName );

	// Create the event source registry key
	dwError = RegCreateKey( HKEY_LOCAL_MACHINE, szPath, &hRegKey );
  //Fehler  wird ignoriert
	//if (dwError != 0)
  //  return dwError;

	// Name of the PE module that contains the message resource
	GetModuleFileName( NULL, szPath, MAX_PATH );

	// Register EventMessageFile
	dwError = RegSetValueEx( hRegKey, _T("EventMessageFile"), 0, REG_EXPAND_SZ, (PBYTE) szPath, (_tcslen( szPath) + 1) * sizeof TCHAR ); 
	if (dwError == 0)
  {
	  // Register supported event types
	  DWORD dwTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
	  dwError = RegSetValueEx( hRegKey, _T("TypesSupported"),	0, REG_DWORD, (LPBYTE) &dwTypes, sizeof dwTypes );

	  // If we want to support event categories, we have also to register	the CategoryMessageFile.
	  // and set CategoryCount. Note that categories need to have the message ids 1 to CategoryCount!

	  if(dwError == 0 && dwCategoryCount > 0 ) 
    {
		  dwError = RegSetValueEx( hRegKey, _T("CategoryMessageFile"), 0, REG_EXPAND_SZ, (PBYTE) szPath, (_tcslen( szPath) + 1) * sizeof TCHAR );
      if (dwError == 0)
		    dwError = RegSetValueEx( hRegKey, _T("CategoryCount"), 0, REG_DWORD, (PBYTE) &dwCategoryCount, sizeof dwCategoryCount );
	  }
  }
	
	RegCloseKey( hRegKey );

  return dwError;
}

DWORD CEventLog::RemoveEventSource(CString csApp)
{
	DWORD dwError = 0;
	TCHAR szPath[ MAX_PATH ];
	
	_stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), csApp );
	return RegDeleteKey( HKEY_LOCAL_MACHINE, szPath );
}

CString CEventLog::LoadMessage(DWORD dwMsgId, ...)
{
  char    pszBuffer[1024];
  DWORD   cchBuffer = 1024;

  va_list args;
	va_start( args, cchBuffer );
	
	if (FormatMessage( 
		FORMAT_MESSAGE_FROM_HMODULE,
		NULL,					// Module (e.g. DLL) to search for the Message. NULL = own .EXE
		dwMsgId,				// Id of the message to look up (aus "Messages.h")
		LANG_NEUTRAL,			// Language: LANG_NEUTRAL = current thread's language
		pszBuffer,				// Destination buffer
		cchBuffer,				// Character count of destination buffer
		&args					// Insertion parameters
	))
    return pszBuffer;
  else
    return "";
}


BOOL CEventLog::Fire(WORD wType, WORD wCategory, DWORD dwEventID, ...)
{
  PSID sid = NULL;
  va_list args;
	va_start( args, dwEventID );

  CString cs;
  int iCount = 0;

  while(1)
  {
    char *p = va_arg( args, char *);
    if (*p != '\0')
      iCount++;
    else
      break;
  }

	va_start( args, dwEventID );

  if (m_hLog == NULL)
    return FALSE;

  BOOL bRet = ReportEvent(m_hLog, wType, wCategory, dwEventID,	GetUserSID(&sid), iCount, 0, (LPCTSTR *)args,	NULL);
  va_end(args);
  if (sid != NULL)
    delete [] sid;
  return bRet;
}

BOOL CEventLog::FireWithData(WORD wType, WORD wCategory, DWORD dwEventID, DWORD dwData, LPVOID ptr,...)
{
  PSID sid = NULL;
  va_list args;
	va_start( args, ptr );

  CString cs;
  int iCount = 0;

  while(1)
  {
    char *p = va_arg( args, char *);
    if (*p != '\0')
      iCount++;
    else
      break;
  }

	va_start( args, ptr );

  if (m_hLog == NULL)
    return FALSE;

  BOOL bRet = ReportEvent(m_hLog, wType, wCategory, dwEventID,	GetUserSID(&sid), iCount, dwData, (LPCTSTR *)args,	ptr);
  va_end(args);
  if (sid != NULL)
    delete [] sid;
  return bRet;
}

BOOL CEventLog::LaunchViewer()
{
  CString csVwr = "%SystemRoot%\\system32\\eventvwr.msc", csParam = " /s";
  CString csVwrExpand, csDefaultDir, csMsg;
  long lErr = ExpandEnvironmentStrings(csVwr,csVwrExpand.GetBufferSetLength(MAX_PATH), MAX_PATH);
  if (lErr == 0)
    return FALSE;

  csVwrExpand.ReleaseBuffer();
  int iPos = csVwrExpand.ReverseFind('\\');
  if (iPos != -1)
    csDefaultDir = csVwrExpand.Left(iPos);

  long hinst = (long)::FindExecutable(csVwrExpand, csDefaultDir, csVwr.GetBufferSetLength(MAX_PATH));
  csVwr.ReleaseBuffer();
  switch (hinst)
  {
    case 0:
      AfxMessageBox("The system is out of memory or resources.", MB_ICONSTOP);
      return FALSE;
    case 31:
      csMsg.Format("No association for file type of '%s' found.", csVwrExpand);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case ERROR_FILE_NOT_FOUND:
      csMsg.Format("File '%s' not found.", csVwrExpand);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case ERROR_PATH_NOT_FOUND:
      csMsg.Format("Path of file '%s' not found.", csVwrExpand);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case ERROR_BAD_FORMAT:
      csMsg.Format("The executable file '%s' is invalid (non-Win32® .exe or error in .exe image).", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    default:
      if (hinst < 32)
      {
        csMsg.Format("Unknown error %d returned from FindExecutable().", hinst);
        AfxMessageBox(csMsg, MB_ICONSTOP);
        return FALSE;
      }
      break;
  }

  hinst = (long)::ShellExecute(NULL, "open", csVwr, csVwrExpand + csParam, csDefaultDir, SW_SHOWNORMAL);
  switch (hinst)
  {
    case 0:
      AfxMessageBox("The operating system is out of memory or resources.", MB_ICONSTOP);
      return FALSE;
    case ERROR_FILE_NOT_FOUND:
      csMsg.Format("File '%s' not found.", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case ERROR_PATH_NOT_FOUND:
      csMsg.Format("Path of file '%s' not found.", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case ERROR_BAD_FORMAT:
      csMsg.Format("The executable for file '%s' is invalid (non-Win32® .exe or error in .exe image).", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case SE_ERR_ACCESSDENIED:
      csMsg.Format("The operating system denied access to file '%s'.", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case SE_ERR_ASSOCINCOMPLETE:
      csMsg.Format("Name association for file %s' is incomplete or invalid.", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case SE_ERR_DDEBUSY:
      AfxMessageBox("The DDE transaction could not be completed because other DDE transactions were being processed.", MB_ICONSTOP);
      return FALSE;
    case SE_ERR_DDEFAIL:
      AfxMessageBox("The DDE transaction failed.", MB_ICONSTOP);
      return FALSE;
    case SE_ERR_DDETIMEOUT:
      AfxMessageBox("The DDE transaction could not be completed because the request timed out.", MB_ICONSTOP);
      return FALSE;
    case SE_ERR_DLLNOTFOUND:
      AfxMessageBox("The specified dynamic-link library was not found.", MB_ICONSTOP);
      return FALSE;
    case SE_ERR_NOASSOC:
      csMsg.Format("No association for file type of '%s' found.", csVwr);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      return FALSE;
    case SE_ERR_OOM:
      AfxMessageBox("The system is out of memory or resources.", MB_ICONSTOP);
      return FALSE;
    case SE_ERR_SHARE:
      AfxMessageBox("A sharing violation occurred.", MB_ICONSTOP);
      return FALSE;
    default:
      if (hinst < 32)
      {
        csMsg.Format("Unknown error %d returned from ShellExecute().", hinst);
        AfxMessageBox(csMsg, MB_ICONSTOP);
        return FALSE;
      }
      return TRUE;
  }
  return FALSE;
}

PSID CEventLog::GetUserSID(PSID * ppSid)
{
  BOOL bRet = FALSE;
  const DWORD INITIAL_SIZE = MAX_PATH;

  CString csAccName;
  DWORD size = INITIAL_SIZE;

  ::GetUserName(csAccName.GetBufferSetLength(size), &size);
  csAccName.ReleaseBuffer(size);

  // Validate the input parameters.
  if (csAccName.IsEmpty() || ppSid == NULL)
  {
    return NULL;
  }


  // Create buffers that may be large enough.
  // If a buffer is too small, the count parameter will be set to the size needed.
  DWORD cbSid = 0;
  DWORD dwErrorCode = 0;
  DWORD dwSidBufferSize = INITIAL_SIZE;
  DWORD cchDomainName = INITIAL_SIZE;
  CString csDomainName;
  SID_NAME_USE eSidType;
  HRESULT hr = 0;


  // Create buffers for the SID and the domain name.
  *ppSid = (PSID) new BYTE[dwSidBufferSize];
  if (*ppSid == NULL)
  {
    return NULL;
  }
  memset(*ppSid, 0, dwSidBufferSize);


  // Obtain the SID for the account name passed.
  for ( ; ; )
  {

    // Set the count variables to the buffer sizes and retrieve the SID.
    cbSid = dwSidBufferSize;
    bRet = LookupAccountName(NULL, csAccName, *ppSid, &cbSid, csDomainName.GetBufferSetLength(cchDomainName), &cchDomainName,&eSidType);
    csDomainName.ReleaseBuffer();
    if (bRet)
    {
      if (IsValidSid(*ppSid) == FALSE)
      {
        CString csMsg;
        csMsg.Format("The SID for %s is invalid.\n", csAccName);
        AfxMessageBox(csMsg, MB_ICONSTOP);
        bRet = FALSE;
      }
      break;
    }
    dwErrorCode = GetLastError();


    // Check if one of the buffers was too small.
    if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
    {
      if (cbSid > dwSidBufferSize)
      {
        // Reallocate memory for the SID buffer.
        TRACE("The SID buffer was too small. It will be reallocated.\n");
        FreeSid(*ppSid);
        *ppSid = (PSID) new BYTE[cbSid];
        if (*ppSid == NULL)
        {
          return NULL; 
        }
        memset(*ppSid, 0, cbSid);
        dwSidBufferSize = cbSid;
      }
    }
    else
    {
      CString csMsg;
      csMsg.Format("LookupAccountNameW failed. GetLastError returned: %d\n", dwErrorCode);
      AfxMessageBox(csMsg, MB_ICONSTOP);
      hr = HRESULT_FROM_WIN32(dwErrorCode);
      break;
    }
  }
 
  // If we had an error, free memory of SID
  if (!bRet && *ppSid != NULL)
  {
    delete [] *ppSid;
    *ppSid = NULL; 
  }

  return *ppSid;
 }
