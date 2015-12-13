// pxm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C"
{
	int __stdcall MessageBoxA(int,char*,char*,int);
}

struct BLAH_BLAH
{
	int A,B;
	int* pIndex;
	void(__stdcall **f)(void*,int,void*);
	int C,D;
};

void __stdcall clb(void*,int reason,void*)
{
	if(reason==1)  /*  DLL_PROCESS_ATTACH  */
		MessageBoxA(0,"walied","hi",0);
}


int x[2]={1,0};
void(__stdcall *ft[2])(void*,int,void*)={&clb,0};


extern "C"
{
	/* object name must be _tls_used */
	BLAH_BLAH _tls_used={0,0,x,ft,0,0};
}

int main(int argc, char* argv[])
{

	return 0;
}
