//http://waleedassar.blogspot.com - (@waleedassar)
//This code shows how to hook the "ZwContinue" function in order to detect debuggers trying to attach.
//Tested with Windows XP SP2/SP3 and Windows 7 SP1.
#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
 
unsigned long relativeOffset(unsigned long from,unsigned long to)
{
        unsigned long diff=0;
        if(from>to)
        {
			//5-byte jump instruction
			diff=((unsigned long)from+5)-(unsigned long)to;
			diff=-diff;
        }
        else
        {
			diff=(unsigned long)to-((unsigned long)from+5);
        }
        return diff;
}
 
struct OSVERSIONINFOEx
{
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  TCHAR szCSDVersion[128];
  WORD  wServicePackMajor;
  WORD  wServicePackMinor;
  WORD  wSuiteMask;
  BYTE  wProductType;
  BYTE  wReserved;
};
 
unsigned long ver;   //0 if XP, 1 if win7, -1 otherwise.
unsigned char* p;  //holds address of ZwContinue
unsigned long addr_Dbg;   //holds address of DbgUiRemoteBreakin
unsigned long addr_DbgPnt;   //holds address of DbgBreakPoint
unsigned long addr_RtlUserThreadStart; ////holds address of RtlUserThreadStart
 
char* backup;    //here we backup the first 5 bytes of ZwContinue.
 
void HookFunc(char* pCOntext)
{
           unsigned long eip_=*(unsigned long*)(pCOntext+0xB8);   //StartAddress
           unsigned long eax_=*(unsigned long*)(pCOntext+0xB0);   //Win32StartAddress
           if(ver==0)  //xp
           {
                   //Since the attaching thread is created via RtlCreateUserThread, then eip should be DbgUiRemoteBreakin and eax should be zero
                   if(eip_==addr_Dbg) ExitProcess(0);
                   //Heuristics (Smart guy may use DbgBreakPoint instead of DbgUiRemoteBreakin
                   if(eip_==addr_DbgPnt) ExitProcess(0);
                   //Smarter guy may manipulate DebugActiveProcess and replace RtlCreateUserThread with CreateRemoteThread
                   if(eax_==addr_Dbg || eax_==addr_DbgPnt) ExitProcess(0);
           }
           else if(ver==1)  //Win7
           {
                   //No matter whether RtlCreateUserThread or CreateRemoteThread is used, the eip is ntdll.RtlUserThreadStart and eax is thread entrypoint
                   if(eip_==addr_RtlUserThreadStart)
                   {
                           if(eax_==addr_Dbg || eax_==addr_DbgPnt) ExitProcess(0);
                   }
           }
 
           //For this to work, plz don't optimize
           __asm
           {
                   leave
                   jmp dword ptr[backup]
           }
}
int main(int argc, char* argv[])
{
        //---------------version info---------------------------------------
        OSVERSIONINFOEx OSI={sizeof(OSI)};
        GetVersionEx((OSVERSIONINFO*)&OSI);
        if(OSI.dwMajorVersion==5 && OSI.dwMinorVersion==1) ver=0;
        else if(OSI.dwMajorVersion==6 && OSI.dwMinorVersion==1) ver=1;
        else ver=-1;
        //------------------------------------------------------------------
        addr_RtlUserThreadStart=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"RtlUserThreadStart");
        addr_Dbg=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"DbgUiRemoteBreakin");
        addr_DbgPnt=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"DbgBreakPoint");
        p=(unsigned char*)GetProcAddress(GetModuleHandle("ntdll.dll"),"ZwContinue");
        unsigned long old;
 
        backup=(char*)VirtualAlloc(0,0x1000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
        memcpy(backup,p,0x5);
        backup[0x5]=0x68;  //push
        *(unsigned long*)(&backup[0x6])=(unsigned long)(p+0x5);
        backup[0xA]=0xC3;  //retn
 
        VirtualProtect(p,5,PAGE_EXECUTE_READWRITE,&old);
        *p=0xE9;  //opcode for jmp
        *(unsigned long*)(p+1)=relativeOffset((unsigned long)p,(unsigned long)&HookFunc);
 
        VirtualProtect(p,5,old,&old);
        //----------------------Shitty stuff------------------------------------
        int x=0;
        while(1)
        {
			Sleep(1000);
			printf("%s %d\r\n","walied",x);
            x++;
        }
		return 0;
}