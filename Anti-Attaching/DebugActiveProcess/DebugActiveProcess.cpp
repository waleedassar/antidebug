// block2.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"
#include "windows.h"
#include "iostream.h"
 

char* GetDebugEventName(unsigned long DebugEventCode)
{
	if(DebugEventCode == 1) return "EXCEPTION_DEBUG_EVENT";
	else if(DebugEventCode == 2) return "CREATE_THREAD_DEBUG_EVENT";
	else if(DebugEventCode == 3) return "CREATE_PROCESS_DEBUG_EVENT";
	else if(DebugEventCode == 4) return "EXIT_THREAD_DEBUG_EVENT";
	else if(DebugEventCode == 5) return "EXIT_PROCESS_DEBUG_EVENT";
	else if(DebugEventCode == 6) return "LOAD_DLL_DEBUG_EVENT";
	else if(DebugEventCode == 7) return "UNLOAD_DLL_DEBUG_EVENT";
	else if(DebugEventCode == 8) return "OUTPUT_DEBUG_STRING_EVENT";
	else if(DebugEventCode == 9) return "RIP_EVENT";
	
	return "UNKNOWN";
}


//This demo spawns calc.exe in debugged mode. Any attempt to debug calc.exe will fail.

int main(int argc, char* argv[])
{
    
    //cout<<"Enter process id (PID) ";
    //unsigned char x[6]={0};
    //cin>>x;
 
    //long xx=atol((char*)x);
 
    STARTUPINFO SI={sizeof(STARTUPINFO)};
    PROCESS_INFORMATION PI;
    CreateProcess(0,"calc.exe",0,0,FALSE,0,0,0,&SI,&PI);
 
    if(DebugActiveProcess(PI.dwProcessId))
    {
   	 bool ss=true;
   	 DEBUG_EVENT DE={0};
   	 while(ss)
   	 {
   		 WaitForDebugEvent(&DE,INFINITE);
   		 cout<<"Debug Event: "<<GetDebugEventName(DE.dwDebugEventCode)<<endl;
   		 switch(DE.dwDebugEventCode)
   		 {
   		 case EXIT_PROCESS_DEBUG_EVENT:
   			 ss=false;
   			 break;
		 case EXCEPTION_DEBUG_EVENT:
			 ContinueDebugEvent(DE.dwProcessId,DE.dwThreadId,DBG_EXCEPTION_NOT_HANDLED);
   		 }
   		 ContinueDebugEvent(DE.dwProcessId,DE.dwThreadId,DBG_CONTINUE);
   	 }
    }
 
    MessageBox(0,"Process terminated!","walied Debugger",0);
    return 0;
}
