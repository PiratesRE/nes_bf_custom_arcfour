# pragma once
# include <Windows.h>


namespace FileSystem {
	bool _ReadFile(HANDLE hFile, void *pBuffer, DWORD dwNumberOfBytesToRead, LPDWORD pdwNumberOfBytesRead, HANDLE hOverlappedEvent);
	DWORD _ReadFileToBuffer(const LPSTR fileName, void *buffer, DWORD bufferSize);
	bool _WriteFile(HANDLE hFile, void *pBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD pdwNumberOfBytesWritten, HANDLE hOverlappedEvent);
};