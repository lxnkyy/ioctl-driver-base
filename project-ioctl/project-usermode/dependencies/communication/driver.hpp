#pragma once

#include <dependencies/includes.hpp>

namespace memory
{
	class driver
	{

	public:

		HANDLE driver_handle;
		INT32 process_id;
		uintptr_t dtb;
		uintptr_t image_base;

		bool find_driver( ) {
			driver_handle = CreateFileW( ( L"\\\\.\\\IoControl" ), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );

			if ( !driver_handle || ( driver_handle == INVALID_HANDLE_VALUE ) )
				return false;

			return true;
		}

		void read_physical( PVOID address, PVOID buffer, size_t size ) {
			invoke_read arguments = { 0 };
			arguments.directory_table = dtb;
			arguments.virtual_address = ( uint64_t )address;
			arguments.process_id = process_id;
			arguments.buffer = ( PVOID )buffer;
			arguments.size = size;


			if ( DeviceIoControl( driver_handle, code_read, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL ) ) { }

		}

		uintptr_t find_image( ) {
			uintptr_t image_address = { NULL };
			invoke_base arguments = { NULL };

			arguments.process_id = process_id;
			arguments.address = ( ULONGLONG* )&image_address;

			if ( DeviceIoControl( driver_handle, code_base, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL ) ) { }


			return image_address;
		}

		uintptr_t resolve_dtb( ) {
			uintptr_t dtb = { NULL };
			invoke_dtb arguments = { NULL };

			arguments.process_id = process_id;
			arguments.dtb = ( ULONGLONG* )&dtb;

			DeviceIoControl( driver_handle, code_dtb, &arguments, sizeof( arguments ), nullptr, NULL, NULL, NULL );

			return dtb;
		}

		INT32 find_process( LPCTSTR process_name ) {
			PROCESSENTRY32 pt;
			HANDLE hsnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
			pt.dwSize = sizeof( PROCESSENTRY32 );
			if ( Process32First( hsnap, &pt ) ) {
				do {
					if ( !lstrcmpi( pt.szExeFile, process_name ) ) {
						CloseHandle( hsnap );
						process_id = pt.th32ProcessID;
						return pt.th32ProcessID;
					}
				} while ( Process32Next( hsnap, &pt ) );
			}
			CloseHandle( hsnap );

			return { NULL };
		}

		template <typename T>
		T read( uintptr_t address ) {
			T buffer{ };
			read_physical( ( PVOID )address, &buffer, sizeof( T ) );
			return buffer;
		}
	};
}

inline memory::driver device;
