struct CLIENT_ID
{
     unsigned long UniqueProcess;
     unsigned long UniqueThread;
};

struct UNICODE_STRING
{
 unsigned short Length;
 unsigned short MaximumLength;
 wchar_t*  Buffer;
};

struct VM_COUNTERS
{
    unsigned long PeakVirtualSize;
    unsigned long VirtualSize;
    unsigned long PageFaultCount;
    unsigned long PeakWorkingSetSize;
    unsigned long WorkingSetSize;
    unsigned long QuotaPeakPagedPoolUsage;
    unsigned long QuotaPagedPoolUsage;
    unsigned long QuotaPeakNonPagedPoolUsage;
    unsigned long QuotaNonPagedPoolUsage;
    unsigned long PagefileUsage;
    unsigned long PeakPagefileUsage;
};

struct IO_COUNTERS
{
  ULONGLONG ReadOperationCount;
  ULONGLONG WriteOperationCount;
  ULONGLONG OtherOperationCount;
  ULONGLONG ReadTransferCount;
  ULONGLONG WriteTransferCount;
  ULONGLONG OtherTransferCount;
};

struct SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    unsigned long WaitTime;
    unsigned long StartAddress;
    CLIENT_ID ClientId;
    long Priority;
    long BasePriority;
    unsigned long ContextSwitchCount;
    long State;
    long WaitReason;
};


struct SYSTEM_PROCESS_INFORMATION {
unsigned long NextEntryDelta;
unsigned long ThreadCount;
unsigned long Reserved1[6];
LARGE_INTEGER CreateTime;
LARGE_INTEGER UserTime;
LARGE_INTEGER KernelTime;
UNICODE_STRING ProcessName;
long BasePriority;
unsigned long ProcessId;
unsigned long InheritedFromProcessId;
unsigned long HandleCount;
unsigned long Reserved2[2];
VM_COUNTERS VmCounters;
IO_COUNTERS IoCounters;
SYSTEM_THREAD_INFORMATION Threads[5]; //Here, 5 is a random number
};