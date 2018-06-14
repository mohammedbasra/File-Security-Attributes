#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>


using namespace std;

SYSTEMTIME stUTC,stLocal;

FILETIME stCreate,stAccess,stWrite;

LPTSTR lpszString;

DWORD dwSize=30;

void traverse(char *Dir)
{
    WIN32_FIND_DATA foundFile;
    
	HANDLE hFind = NULL;
    
	char Path[500];
    
	sprintf(Path, "%s\\*.*", Dir);
	
	hFind = FindFirstFile(Path, &foundFile);
    
	if(hFind == INVALID_HANDLE_VALUE)
    {
        cout << "Path Not Found " <<endl;
        return;
    }

    do
    {
       
        if(strcmp(foundFile.cFileName, ".") != 0
                && strcmp(foundFile.cFileName, "..") != 0)
        {
            sprintf(Path, "%s\\%s", Dir, foundFile.cFileName);

            //folder

            if(foundFile.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)

            {

                traverse(Path);

            }

            //file

            else{
            	
				GetFileTime(hFind,&stCreate,&stAccess,&stWrite);
            	
				FileTimeToSystemTime(&stCreate, &stUTC);
            	
				SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
            	printf("%02d/%02d/%d  %02d:%02d",stLocal.wMonth,stLocal.wDay,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
				 		/*StringCchPrintf(lpszString, dwSize, 
        				
						TEXT("%02d/%02d/%d  %02d:%02d"),
        			
						stLocal.wMonth, stLocal.wDay, stLocal.wYear,
        			
						stLocal.wHour, stLocal.wMinute);*/

                printf("File: %s\n", Path);

            }

        }
        
    }

    while(FindNextFile(hFind, &foundFile)); 
    
}

int main(){

	char path[256];

	cout << "Enter the Path:" << endl;

	scanf("%s",path);

	traverse(path);

return 0;

}
