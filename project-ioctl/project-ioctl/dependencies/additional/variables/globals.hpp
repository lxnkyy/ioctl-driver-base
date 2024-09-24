#pragma once

namespace globals
{
	inline UNICODE_STRING link;
}

#define code_read CTL_CODE(FILE_DEVICE_UNKNOWN, 0x75, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_dtb CTL_CODE(FILE_DEVICE_UNKNOWN, 0x73, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)


extern "C" NTSTATUS NTAPI IoCreateDriver( PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction );
extern "C" PVOID NTAPI PsGetProcessSectionBaseAddress( PEPROCESS Process );
extern "C" NTSTATUS NTAPI ZwQuerySystemInformation( SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength );


#define PAGE_OFFSET_SIZE 12
static const UINT64 PMASK = ( ~0xfull << 8 ) & 0xfffffffffull;