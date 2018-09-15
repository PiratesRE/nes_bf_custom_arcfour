# include "FileSystem.h"
# include <stdio.h>

bool FileSystem::_ReadFile(HANDLE hFile, void *pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead, HANDLE hOverlappedEvent) {
	bool bRetVal = false;
	OVERLAPPED ov;

	ZeroMemory(&ov, sizeof(OVERLAPPED));
	ov.hEvent = hOverlappedEvent;

	if(ReadFile(hFile, pBuffer, dwNumberOfBytesToRead, pdwNumberOfBytesRead, &ov)) { bRetVal = true; }
	else {
		DWORD le = GetLastError();
		if (le == ERROR_HANDLE_EOF) {
			*pdwNumberOfBytesRead = 0;
			bRetVal = true;
		}
		else if (le == ERROR_IO_PENDING) {
			if (GetOverlappedResult(hFile, &ov, pdwNumberOfBytesRead, TRUE)) { bRetVal = true; }
			else if (GetLastError() == ERROR_HANDLE_EOF) {
				*pdwNumberOfBytesRead = 0;
				bRetVal = true;
			}
		}
	}
	return(bRetVal);
}

DWORD FileSystem::_ReadFileToBuffer(const LPSTR fileName, void *buffer, DWORD bufferSize) {
	DWORD retVal = (DWORD)-1;
	HANDLE file  = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (file != INVALID_HANDLE_VALUE) {
		if(ReadFile(file, buffer, bufferSize, &bufferSize, NULL) != FALSE) { retVal = bufferSize; }
		CloseHandle(file);
	}
	return(retVal);
}

bool FileSystem::_WriteFile(HANDLE hFile, void *pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten, HANDLE hOverlappedEvent) {
	bool bRetVal = false;
	OVERLAPPED ov;

	ZeroMemory(&ov, sizeof(OVERLAPPED));
	ov.hEvent = hOverlappedEvent;

	if (WriteFile(hFile, pBuffer, dwNumberOfBytesToWrite, pdwNumberOfBytesWritten, &ov)) { bRetVal = true; }
	else if(GetLastError() == ERROR_IO_PENDING) {
		if (GetOverlappedResult(hFile, &ov, pdwNumberOfBytesWritten, TRUE)) { bRetVal = true; }
	}
	return(bRetVal);
}