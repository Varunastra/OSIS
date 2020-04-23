#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdint.h>
#include <iostream>
#include <shlwapi.h>

using namespace std;

LONGLONG ClusterNumber;
bool ClusterFoundFlag = false;

SIZE_T *GetFileClusters(
    PCTSTR lpFileName,
    ULONG ClusterSize,
    ULONG *ClCount,
    ULONG *FileSize)
{
    HANDLE hFile;
    ULONG OutSize;
    ULONG Bytes, Cls, CnCount, r;
    SIZE_T *Clusters = NULL;
    BOOLEAN Result = FALSE;
    LARGE_INTEGER PrevVCN, Lcn;
    STARTING_VCN_INPUT_BUFFER InBuf;
    PRETRIEVAL_POINTERS_BUFFER OutBuf;
    hFile = CreateFile(lpFileName, GENERIC_READ, // FILE_READ_ATTRIBUTES
                       FILE_SHARE_READ,          // | FILE_SHARE_WRITE | FILE_SHARE_DELETE
                       NULL, OPEN_EXISTING, 0, 0);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        *FileSize = GetFileSize(hFile, NULL);
        OutSize = sizeof(RETRIEVAL_POINTERS_BUFFER) + (*FileSize / ClusterSize) * sizeof(OutBuf->Extents);
        OutBuf = (PRETRIEVAL_POINTERS_BUFFER)malloc(OutSize);
        InBuf.StartingVcn.QuadPart = 0;
        if (DeviceIoControl(hFile, FSCTL_GET_RETRIEVAL_POINTERS, &InBuf,
                            sizeof(InBuf), OutBuf, OutSize, &Bytes, NULL))
        {
            *ClCount = (*FileSize + ClusterSize - 1) / ClusterSize;
            Clusters = (SIZE_T *)malloc(*ClCount * sizeof(SIZE_T));
            PrevVCN = OutBuf->StartingVcn;
            for (r = 0, Cls = 0; r < OutBuf->ExtentCount; r++)
            {
                Lcn = OutBuf->Extents[r].Lcn;
                for (CnCount = OutBuf->Extents[r].NextVcn.QuadPart - PrevVCN.QuadPart;
                     CnCount;
                     CnCount--, Cls++, Lcn.QuadPart++)
                    Clusters[Cls] = Lcn.QuadPart;
                PrevVCN = OutBuf->Extents[r].NextVcn;
            }
        }
        free(OutBuf);
        CloseHandle(hFile);
    }
    return Clusters;
}

void GetFileAttr(PCTSTR FilePath)
{
    ULONG64 d1 = 0, d2 = 0;
    char DriveName[3] = {0};
    ULONG SecPerCl, BtPerSec, ClusterSize, ClCount, FileSize, i;
    memcpy(DriveName, FilePath, 2);
    GetDiskFreeSpaceA(DriveName, &SecPerCl, &BtPerSec, NULL, NULL);
    ClusterSize = SecPerCl * BtPerSec;
    PSIZE_T x = GetFileClusters(FilePath, ClusterSize, &ClCount, &FileSize);
    if (x != NULL)
    {
        for (i = 0; i < ClCount; i++)
        {
            // d1 = 8 * x[i];
            // d2 = 512 * 8 * x[i];
            // printf("Cluster[%ld]: %ld\tSector: %lLd\tOffset: 0x%llx\n", i, x[i], d1, d2);
            if (x[i] == ClusterNumber)
            {
                cout << "Cluster found in file " << FilePath;
                ClusterFoundFlag = true;
                system("exit");
            }
        }
        free(x);
    }
}

void FindAllFiles(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
{
    TCHAR szFullPattern[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFindFile;
    PathCombine(szFullPattern, lpFolder, _T("*"));
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((FindFileData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY &&
                (FindFileData.cFileName[0] != '.'))
            {
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                // _tprintf("Found %s %s %s \n", _T("Subdirectory" ), szFullPattern, FindFileData.cFileName);
                FindAllFiles(szFullPattern, lpFilePattern);
            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }

    PathCombine(szFullPattern, lpFolder, lpFilePattern);
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                // _tprintf_s("%s\n", szFullPattern);
                GetFileAttr(szFullPattern);
            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
}

int main()
{
    cout << "Write number of cluster: ";
    cin >> ClusterNumber;
    FindAllFiles(_T("E:\\"), _T("*"));
    if (!ClusterFoundFlag)
    {
        cout << "No files found";
    }
}