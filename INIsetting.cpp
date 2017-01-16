#include "INIsetting.h"


CINIsetting::CINIsetting(void)
{
}


CINIsetting::~CINIsetting(void)
{
}


void CINIsetting::GetINIString(char * szCategori, char * szTitle, char * szVariable, int nVariableSzie)
{
	GetPrivateProfileStringA(szCategori, szTitle,"MSG_ERROR", szVariable , nVariableSzie , currentPath);
}


int CINIsetting::GetINIInt(char * szCategori, char * szTitle, int nSize)
{
	return GetPrivateProfileIntA(szCategori, szTitle, nSize, currentPath);
}


void CINIsetting::CreateINIFile(char * szFileName)
{
	 _getcwd(currentPath, _MAX_PATH);
    printf("%s\n", currentPath);
	strcat(currentPath, "\\");
	strcat(currentPath, szFileName);
}


void CINIsetting::WriteINIString(char * szCategori, char * szTitle, char * szMessage)
{
	WritePrivateProfileStringA(szCategori,szTitle,szMessage,currentPath );
}
