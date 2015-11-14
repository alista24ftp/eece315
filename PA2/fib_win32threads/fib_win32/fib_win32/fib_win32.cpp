// fib_win32.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

// data shared by threads
DWORD *fib_seq;

DWORD fib(DWORD);

// thread runs in this separate function
DWORD WINAPI GenerateSeq(LPVOID Param){
//DWORD GenerateSeq(LPVOID Param){
	DWORD seq_len = *(DWORD *)Param;
	DWORD i;
	for(i=0; i<seq_len; i++){
		fib_seq[i] = fib(i);
	}
	return 0;
}

DWORD fib(DWORD n){
	if(n == 0) return 0;
	if(n == 1) return 1;
	return fib(n-1) + fib(n-2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD ThreadId;
	HANDLE ThreadHandle;
	DWORD Param, i;
	
	if(argc != 2){
		fprintf(stderr, "Enter valid integer parameter for sequence length\n");
		return -1;
	}
	
	Param = wcstod(argv[1], _T('\0'));
	if(Param < 1){
		fprintf(stderr, "Enter integer value >= 1 for sequence length\n");
		return -1;
	}
	
	fib_seq = (DWORD *) malloc(Param * sizeof(DWORD));
	
	/* create the thread */
	ThreadHandle = CreateThread(
		NULL, /* default security attributes */
		0, /* default stack size */
		GenerateSeq, /* thread function */
		&Param, /* parameter to thread function */
		0, /* default creation flags */
		&ThreadId /* returns the thread identifier */
	); 
		
	if (ThreadHandle != NULL) {
		/* now wait for the thread to finish */
		WaitForSingleObject(ThreadHandle,INFINITE);
		
		/* close the thread handle */
		CloseHandle(ThreadHandle);
		
		printf("Fibonacci sequence up to %d is: \n", Param);
		for(i=0; i<Param-1; i++){
			printf("%d, ", fib_seq[i]);
		}
		printf("%d\n", fib_seq[i]);
		
		free(fib_seq);
	}
}

