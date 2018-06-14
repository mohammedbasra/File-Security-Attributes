#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>


using namespace std;

SYSTEMTIME stUTC,stLocal;

FILETIME stCreate,stAccess,stWrite;

LPTSTR lpszString;

DWORD dwSize=30;

int c=0;

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
            sprintf(Path, "%s\\%s", Dir , foundFile.cFileName);

            //folder

            if(foundFile.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)

            {
				
                traverse(Path);

            }

            //file

            else{
            	
            	c++;
				/*GetFileTime(hFind,&stCreate,&stAccess,&stWrite);
            	
				FileTimeToSystemTime(&stCreate, &stUTC);
            	
				SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
            	printf("%02d/%02d/%d  %02d:%02d",stLocal.wMonth,stLocal.wDay,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
				 		StringCchPrintf(lpszString, dwSize, 
        				
						TEXT("%02d/%02d/%d  %02d:%02d"),
        			
						stLocal.wMonth, stLocal.wDay, stLocal.wYear,
        			
						stLocal.wHour, stLocal.wMinute);*/

                

            }

        }
        
    }

    while(FindNextFile(hFind, &foundFile)); 
    
}

void DisplayVolumePaths(
         PCHAR VolumeName
        )
{
    DWORD  CharCount = MAX_PATH + 1;
    PCHAR Names     = NULL;
    PCHAR NameIdx   = NULL;
    BOOL   Success   = FALSE;

    for (;;) 
    {
        //
        //  Allocate a buffer to hold the paths.
        Names = (PCHAR) new BYTE [CharCount * sizeof(CHAR)];

        if ( !Names ) 
        {
            //
            //  If memory can't be allocated, return.
            return;
        }

        //
        //  Obtain all of the paths
        //  for this volume.
        Success = GetVolumePathNamesForVolumeName(
            VolumeName, Names, CharCount, &CharCount
            );

        if ( Success ) 
        {
            break;
        }

        if ( GetLastError() != ERROR_MORE_DATA ) 
        {
            break;
        }

        //
        //  Try again with the
        //  new suggested size.
        delete [] Names;
        Names = NULL;
    }

    if ( Success )
    {
        
        for ( NameIdx = Names; 
              NameIdx[0] != '\0'; 
              NameIdx += strlen(NameIdx) + 1 ) 
        {
            printf("  %s", NameIdx);
            traverse(NameIdx);
            printf("Files in %s : %d\n", NameIdx , c );
        }
        printf("\n");
    }

    if ( Names != NULL ) 
    {
        delete [] Names;
        Names = NULL;
    }

    return;
}
void __cdecl getvol(void)
{
    DWORD  CharCount            = 0;
    char  DeviceName[MAX_PATH] = "";
    DWORD  Error                = ERROR_SUCCESS;
    HANDLE FindHandle           = INVALID_HANDLE_VALUE;
    BOOL   Found                = FALSE;
    size_t Index                = 0;
    BOOL   Success              = FALSE;
    char  VolumeName[MAX_PATH] = "";

    //
    //  Enumerate all volumes in the system.
    FindHandle = FindFirstVolume(VolumeName, ARRAYSIZE(VolumeName));

    if (FindHandle == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        printf("FindFirstVolumeW failed with error code %d\n", Error);
        return;
    }

    for (;;)
    {
        
        Index = strlen(VolumeName) - 1;

        if (VolumeName[0]     != '\\' ||
            VolumeName[1]     != '\\' ||
            VolumeName[2]     != '?'  ||
            VolumeName[3]     != '\\' ||
            VolumeName[Index] != '\\') 
        {
            Error = ERROR_BAD_PATHNAME;
            printf("FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", VolumeName);
            break;
        }

        //
        //  QueryDosDeviceW does not allow a trailing backslash,
        //  so temporarily remove it.
        VolumeName[Index] = '\0';

        CharCount = QueryDosDevice(&VolumeName[4], DeviceName, ARRAYSIZE(DeviceName)); 

        VolumeName[Index] = L'\\';

        if ( CharCount == 0 ) 
        {
            Error = GetLastError();
            printf("QueryDosDeviceW failed with error code %d\n", Error);
            break;
        }

        printf("\nFound a device:\n %s", DeviceName);
        printf("\nVolume name: %s", VolumeName);
        printf("\nPaths:");
        DisplayVolumePaths(VolumeName);

        //
        //  Move on to the next volume.
        Success = FindNextVolume(FindHandle, VolumeName, ARRAYSIZE(VolumeName));

        if ( !Success ) 
        {
            Error = GetLastError();

            if (Error != ERROR_NO_MORE_FILES) 
            {
                printf("FindNextVolumeW failed with error code %d\n", Error);
                break;
            }

            //
            //  Finished iterating
            //  through all the volumes.
            Error = ERROR_SUCCESS;
            break;
        }
    }

    FindVolumeClose(FindHandle);
    FindHandle = INVALID_HANDLE_VALUE;

    return;
}

int main(){

	char path[256];

	//cout << "Enter the Path:" << endl;

//	scanf("%s",path);

//	traverse(path);
getvol();

return 0;

}
