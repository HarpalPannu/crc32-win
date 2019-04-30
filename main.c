#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "crc.h"
char *readable_fs(double size, char *buf)
{
    int i = 0;
    const char *units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024){
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f %s", i, size, units[i]);
    return buf;
}
double precent = 0.0;
int isDone = 1;
DWORD WINAPI myThread(LPVOID lpParameter)
{
	  while(isDone){
          printf("%0.2f%c \r", precent, '%');
      }
      return 0;
}
int main(void)
{
    char *filenames[5] =
        {
            "E:\\New folder\\1922.2017.720p.NF.WEBRip.x264.650MB-Pahe.in.mkv",
            "Manzil.mp3"};
    int sel = 0;
    char buf[10];
    crc_t crc;
    clock_t start, end;
    start = clock();
    crc = crc_init();
    FILE *file = NULL;

    
    

    file = fopen(filenames[sel], "rb");
    if (file == NULL)
    {
        printf("Error! Unable to read file\n");
        return -1;
    }
    char buffer[1024];
    DWORD bytesRead = 0;
    HANDLE hFile = CreateFile(TEXT("1922.2017.720p.NF.WEBRip.x264.650MB-Pahe.in.mkv"),
                     GENERIC_READ,
                     0,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);
    if(hFile == INVALID_HANDLE_VALUE){
        printf("Error hFile");
    }
    double filesize = (double)GetFileSize(hFile,NULL);
    double processed = 0;
    printf("Name: %s \nSize: %s (%d Bytes)\n", filenames[sel],readable_fs(filesize, buf), (int)filesize);

	unsigned int myCounter = 0;
	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, myThread, &myCounter, 0, &myThreadID);

    while (ReadFile(hFile, buffer, 1024, &bytesRead, NULL))
    {
        crc = crc_update(crc, buffer, bytesRead);
    
        processed += bytesRead;
        precent = (processed / filesize) * 100.0;
       // printf("%d\r",(int)bytesRead);;
       // printf("%0.2f%c \r", precent, '%');
       // fflush(stdout);
        if(bytesRead < 1024){
             isDone = 0;
            break;
           
        }
    }

    crc = crc_finalize(crc);

    printf("CRC32 Hexadecimal: 0x%lX\n", (unsigned long)crc);
    printf("CRC32 Decimal: %lu\n", (unsigned long)crc);
    end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f s\n", cpu_time_used);
	WaitForSingleObject(myHandle,INFINITE);
	CloseHandle(myHandle);
    return 0;
}
