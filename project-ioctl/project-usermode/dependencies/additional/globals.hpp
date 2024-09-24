#pragma once

#define code_read CTL_CODE(FILE_DEVICE_UNKNOWN, 0x75, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_dtb CTL_CODE(FILE_DEVICE_UNKNOWN, 0x73, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// driver structures
typedef struct invoke_read {
	uint64_t directory_table;
	uint64_t virtual_address;
	uint32_t process_id;
	void* buffer;
	size_t size;
} read, * pread;

typedef struct invoke_base {
	INT32 process_id;
	ULONGLONG* address;
} base, * pbase;

typedef struct invoke_dtb {
	uint32_t process_id;
	ULONGLONG* dtb;
} dtb, * pdtb;
