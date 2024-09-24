#pragma once

#include <dependencies/includes.hpp>

namespace pml4
{
	PVOID split_memory( PVOID SearchBase, SIZE_T SearchSize, const void* Pattern, SIZE_T PatternSize );
	static NTSTATUS InitializeMmPfnDatabase( );
	uintptr_t dirbase_from_base_address( void* base );
}