// blog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "iostream.h"


int main(int argc, char* argv[])
{
  unsigned char* p=(unsigned char*)GetProcAddress(GetModuleHandle("ntdll.dll"),"DbgUiRemoteBreakin");
 
  unsigned long xxx=0;
  VirtualProtect(p,6,PAGE_EXECUTE_READWRITE,&xxx);

 *p=0x68;  //opcodes for push
 *(unsigned long*)(p+1)=(unsigned long)GetProcAddress(GetModuleHandle("kernel32.dll"),"ExitProcess");
 *(p+5)=0xC3;  //opcode for retn
  VirtualProtect(p,6,xxx,&xxx);
 
  int x=0;
  while(1)
  {
  	Sleep(1000);
  	cout<<"walied "<<x++<<endl;
  }
  return 0;
}
