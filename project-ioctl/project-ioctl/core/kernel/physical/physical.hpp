#pragma once

#include <dependencies/includes.hpp>

namespace physical
{
	uint64_t get_kernel_directory( );

	NTSTATUS read_physical_address( PVOID, PVOID, SIZE_T, SIZE_T* );
	uint64_t translate_linear_address( uint64_t, uint64_t );
	NTSTATUS write_physical_address( PVOID, PVOID, SIZE_T, SIZE_T* );
}