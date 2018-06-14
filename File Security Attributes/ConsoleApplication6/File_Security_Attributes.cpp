//header files
#include "stdafx.h"
#include "iostream"
#include<Windows.h>
#include<stdio.h>
#include<sddl.h>
#include<AccCtrl.h>
#include<Aclapi.h>
#include<tchar.h>

//namespaces
using namespace std;



int main()
{

	//LPCSTR f = _T("?C:\\Users\\fatem\\Desktop\\echobodo.jpg");
	DWORD ssd = 0;
	PSECURITY_DESCRIPTOR psd = NULL;
	PSID psid = NULL;
	BOOL flag = FALSE, flag2 = FALSE, lpbDaclDefaulted = FALSE;
	DWORD saccount = 1;
	LPTSTR account = NULL;
	DWORD sdomain = 1;
	LPTSTR domain = NULL;
	SID_NAME_USE snu;
	PACL pDacl;
	string s = "C:\\Users\\Administrator\\Desktop\\mohammed.txt";
	char ff[100];
	strcpy_s(ff, s.c_str());
	HANDLE process_handle =(HANDLE) GetCurrentProcess();
	PHANDLE TokenHandle = new HANDLE;
	BOOL df = OpenProcessToken( process_handle , ACCESS_SYSTEM_SECURITY ,TokenHandle);
	cout << df << endl;
	//DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION
	BOOL a;
	a = GetFileSecurity(ff,  LABEL_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION, NULL, 0, &ssd);	printf("allocating security descriptor (%lu bytes) ... ", ssd);
	if ((psd = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED, ssd)) == NULL) cout << "Error" << endl;
	printf("success\naccessing file '%s' ... ", ff);
	a = GetFileSecurity(ff, LABEL_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION, psd, ssd, &ssd);
	if (a == 0) cout << "Error" << endl;

	printf("success\nchecking security descriptor (%lu bytes) ... ", GetSecurityDescriptorLength(psd));
	if (IsValidSecurityDescriptor(psd) == 0) cout << "Error" << endl;;
	printf("success\naccessing owner ... ");
	// allocate SID structure ?
	if (GetSecurityDescriptorOwner(psd, &psid, &flag) == 0) cout << "Error" << endl;
	else printf("success\n");
	if (GetSecurityDescriptorDacl(psd, &flag2, &pDacl, &lpbDaclDefaulted) == 0)cout << "Error" << endl;
	else printf("\nSuccess getting Dacl\n");
	printf("DACL Size : %d\n", pDacl->AclSize);
	printf("DACL AceCount : %d\n", pDacl->AceCount);

	PSECURITY_DESCRIPTOR_CONTROL pControl = new SECURITY_DESCRIPTOR_CONTROL;
	DWORD lpdwRevision;
	BOOL x = GetSecurityDescriptorControl(psd, pControl, &lpdwRevision);
	if (x == 0)cout << "Function Failed." << endl;
	cout << "Revision : "<< lpdwRevision << endl;
	// check SID validity with IsValidSid
	BOOL c = IsValidSid(psid);
	LPTSTR StringSid;
	//cout << "C: " << c << endl;
	cout << "=================================================================" << endl;
	cout << "=================================================================" << endl;
	// First call to LookupAccountSid to get the buffer sizes.
	BOOL bRtnBool = LookupAccountSid(
		NULL,           // local computer
		psid,
		account,
		(LPDWORD)&saccount,
		domain,
		(LPDWORD)&sdomain,
		&snu);

	// Reallocate memory for the buffers.
	account = (LPTSTR)GlobalAlloc(
		GMEM_FIXED,
		saccount);

	// Check GetLastError for GlobalAlloc error condition.
	if (account == NULL) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		_tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
		return -1;
	}

	domain = (LPTSTR)GlobalAlloc(
		GMEM_FIXED,
		sdomain);

	// Check GetLastError for GlobalAlloc error condition.
	if (domain == NULL) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();
		printf("GlobalAlloc error = %d\n", dwErrorCode);
		return -1;

	}

	// Second call to LookupAccountSid to get the account name.
	bRtnBool = LookupAccountSid(
		NULL,                   // name of local or remote computer
		psid,              // security identifier
		account,               // account name buffer
		(LPDWORD)&saccount,   // size of account name buffer
		domain,             // domain name
		(LPDWORD)&sdomain, // size of domain name buffer
		&snu);                 // SID type

							   // Check GetLastError for LookupAccountSid error condition.
	if (bRtnBool == FALSE) {
		DWORD dwErrorCode = 0;

		dwErrorCode = GetLastError();

		if (dwErrorCode == ERROR_NONE_MAPPED)
			printf(
			"Account owner not found for specified SID.\n");
		else
			printf("Error in LookupAccountSid.\n");
		return -1;

	}
	else if (bRtnBool == TRUE)

		_tprintf(TEXT("Account owner = %s\n"), account);
	_tprintf(TEXT("Domain owner = %s\n"), domain);

	PEXPLICIT_ACCESS pListOfExplicitEntries = new EXPLICIT_ACCESS;
	ULONG pcCountOfExplicitEntries = 0;
	DWORD ace_entries = 99;
	ace_entries = GetExplicitEntriesFromAcl(
		pDacl,
		&pcCountOfExplicitEntries,
		&pListOfExplicitEntries);
	cout << "Number of ACE Entries found : " << pcCountOfExplicitEntries << endl;
	//cout << sizeof(*pListOfExplicitEntries) << endl;
	if (ace_entries == ERROR_SUCCESS)cout << "Function was successful!" << endl;
	for (UINT i = 0; i < pcCountOfExplicitEntries; i++)
	{
		/*char AccountBuff[80];
		char DomainBuff[80];
		DWORD AccountBufflength = 40;
		DWORD DomainBufflength = 40;*/
		cout << "=================================================================" << endl;
		SID_NAME_USE peUse;
		DWORD saccount = 1;
		LPTSTR account = NULL;
		DWORD sdomain = 1;
		LPTSTR domain = NULL;
		LPTSTR StringSid;
		DWORD access_permissions = pListOfExplicitEntries[i].grfAccessPermissions;
		PTSTR Sid = pListOfExplicitEntries[i].Trustee.ptstrName;
		//_tprintf("Sid : %s\n", Sid);
		BOOL  s_id = ConvertSidToStringSid( Sid, &StringSid);
		printf("Security ID Structure : %s\n",StringSid);
		LookupAccountSid(NULL,
			Sid,
			account,
			&saccount,
			domain,
			&sdomain,
			&peUse
		);
		account = (LPTSTR)GlobalAlloc(
			GMEM_FIXED,
			saccount);

		// Check GetLastError for GlobalAlloc error condition.
		if (account == NULL) {
			DWORD dwErrorCode = 0;

			dwErrorCode = GetLastError();
			_tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
			return -1;
		}

		domain = (LPTSTR)GlobalAlloc(
			GMEM_FIXED,
			sdomain);

		// Check GetLastError for GlobalAlloc error condition.
		if (domain == NULL) {
			DWORD dwErrorCode = 0;

			dwErrorCode = GetLastError();
			_tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
			return -1;

		}

		// Second call to LookupAccountSid to get the account name.
		bRtnBool = LookupAccountSid(
			NULL,                   // name of local or remote computer
			Sid,              // security identifier
			account,               // account name buffer
			(LPDWORD)&saccount,   // size of account name buffer
			domain,             // domain name
			(LPDWORD)&sdomain, // size of domain name buffer
			&snu);
		_tprintf(TEXT("Account = %s\n"), account);
		_tprintf(TEXT("Domain = %s\n\n"), domain);
		cout << "Permissions Granted:\n" << endl;
		if (access_permissions &GENERIC_ALL)cout << "READ\nWRITE\nEXECUTE" << endl;
		else {
			if (access_permissions &STANDARD_RIGHTS_READ)cout << "READ" << endl;
			if (access_permissions &STANDARD_RIGHTS_WRITE)cout << "WRITE" << endl;
			if (access_permissions &STANDARD_RIGHTS_EXECUTE)cout << "EXECUTE" << endl;
		}
		cout << "SACL Access : "; if (access_permissions &ACCESS_SYSTEM_SECURITY)cout << "Granted.." << endl;
		else cout << "Not granted" << endl;
		cout << "DELETE ACCESS : "; if (access_permissions &DELETE)cout << "Granted" << endl; else cout << "Not granted" << endl;
		cout << "READ CONTROL : "; if (access_permissions &READ_CONTROL)cout << "Granted" << endl; else cout << "Not granted" << endl;
		cout << "Write to DACL : "; if (access_permissions &WRITE_DAC)cout << "Granted" << endl; else cout << "Not granted" << endl;
		cout << "Write access to owner : "; if (access_permissions &WRITE_OWNER)cout << "Granted" << endl; else cout << "Not granted" << endl;
		cout << "Synchronize Access : "; if (access_permissions &SYNCHRONIZE)cout << "Granted" << endl; else cout << "Not granted" << endl;
		cout << "=================================================================" << endl;
	}
	cout << "End" << endl;
	return 0;
}
