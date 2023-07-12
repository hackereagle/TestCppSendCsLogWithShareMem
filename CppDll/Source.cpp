#include "Source.h"
#include <tchar.h>
#include <iomanip>

void WriteErrorToFile(const char* message)
{
    FILE* fp = 0;
    fopen_s(&fp, "./debug.txt", "a+");
    if (fp != 0)
    {
        fprintf(fp, "%s", message);
        fclose(fp);
    }
}

void CreatePipeStream(const char* szPipeUpdate)
{
    HANDLE hFile;
    BOOL flg;
    DWORD dwWrite;
    //char szPipeUpdate[200];
    //hFile = CreateFile(L"\\\\.\\pipe\\BvrPipe", GENERIC_WRITE,
    hFile = CreateFile((LPCSTR)"\\\\.\\pipe\\BvrPipe", GENERIC_WRITE,
        0, NULL, OPEN_ALWAYS,
        0, NULL);

    //strcpy(szPipeUpdate, "Data from Named Pipe client for createnamedpipe");
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DWORD dw = GetLastError();
        WriteErrorToFile("CreateFile failed for Named Pipe client\n:");
    }
    else
    {
        flg = WriteFile(hFile, szPipeUpdate, strlen(szPipeUpdate), &dwWrite, NULL);
        if (FALSE == flg)
        {
            WriteErrorToFile("WriteFile failed for Named Pipe client\n");
        }
        else
        {
            WriteErrorToFile("WriteFile succeeded for Named Pipe client\n");
        }
        CloseHandle(hFile);
    }
}

int AddTwoNum(int a, int b)
{
	int sum = 0;
    CreatePipeStream("test");
	sum = a + b;
    CreatePipeStream("yo");
	return sum;
}