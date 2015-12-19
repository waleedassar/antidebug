// PEBLoop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"


int main(int argc, char* argv[])
{
	unsigned long start = 0;
	unsigned long imagebase = 0;

	__asm
	{
		mov eax,dword ptr fs:[0x30]
		mov eax,dword ptr [eax+0xC]
		add eax,0x14
		mov dword ptr [ebp-0x4],eax
		mov eax,dword ptr[eax]
here:
		mov ebx,dword ptr[eax+0x10]
		mov dword ptr[ebp-0x8],ebx
		mov ecx,eax
		mov eax,dword ptr[eax]
		cmp eax,dword ptr[ebp-0x4]
		jnz here
	}
	while(9)
	{
		Sleep(1000);
	}
	return 0;
}
