#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "structs.h"

#pragma comment(lib,"ntdll.lib")
#pragma comment(lib,"msvcrt.lib")
#pragma comment(linker,"/entry:main")


#define SystemProcessesAndThreadsInformation 0x5
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004


extern "C"
{
   int __stdcall ZwQuerySystemInformation(int,void*,int,void*);
}


//---------prototypes-----------------
unsigned long addr;
void __stdcall clb(void*,int,void*);
//------------------------------------
//This is the TLS structure.
struct BLAH_BLAH
{
	int A,B;
	int* pIndex;
	void(__stdcall **f)(void*,int,void*);
	int C,D;
};
//-------------------------------------------
int x[2]={1,0};
void(__stdcall *ft[3])(void*,int,void*)={&clb,0,0};
//-------------------------------------------

unsigned long y=0;
void __stdcall clb(void*,int reason,void*)
{
  if(reason==DLL_THREAD_ATTACH)
  {
	MessageBox(0,"walied","assar",0);
	unsigned long cbBuffer=0x5000;
	void* Buffer=(void*)LocalAlloc(0,cbBuffer);
	if(!Buffer) return;
	bool x=false;
	while(x==false)
	{
	    int ret=ZwQuerySystemInformation(SystemProcessesAndThreadsInformation,Buffer,cbBuffer,0);
		if(ret<0)
		{
			if(ret==STATUS_INFO_LENGTH_MISMATCH)
			{
				cbBuffer=cbBuffer+cbBuffer;
				LocalFree(Buffer);
				Buffer=(void*)LocalAlloc(0,cbBuffer);
				if(!Buffer) return;
				x=false;
			}
			else return;
		}
		else x=true;
	}
	SYSTEM_PROCESS_INFORMATION* p=(SYSTEM_PROCESS_INFORMATION*)Buffer;	
	while(1)
	{
			if(p->ProcessId==GetCurrentProcessId())
			{
				for(int i=0;i<p->ThreadCount;i++)
				{
					if(GetCurrentThreadId()==p->Threads[i].ClientId .UniqueThread)
					{
						if(addr==p->Threads[i].StartAddress)
						{
							MessageBox(0,"A7a, are you trying to attach to me","walied",0);
						    ExitProcess(10);
						}
					}
				}
			}
			if(p->NextEntryDelta==0) break;
			p=(SYSTEM_PROCESS_INFORMATION*)((unsigned char*)p+(p->NextEntryDelta));
		}
  }
}
//----------------------------------------------------------
extern "C"
{
	/* object name must be _tls_used */
	BLAH_BLAH _tls_used={0,0,x,ft,0,0};
}

int main(int argc, char* argv[])
{
	addr=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"DbgUiRemoteBreakin");

	int ssss=0;
	while(1)
	{
		Sleep(1000);
		printf("walied %X\r\n",ssss++);
	}
	return 0;
}
