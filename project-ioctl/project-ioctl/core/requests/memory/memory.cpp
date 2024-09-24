#include "memory.hpp"

auto find_min( INT32 g, SIZE_T f ) -> ULONG64
{
	INT32 h = ( INT32 )f;
	ULONG64 result = 0;

	result = ( ( ( g ) < ( h ) ) ? ( g ) : ( h ) );

	return result;
}

namespace memory
{
	NTSTATUS read( pread data ) {
		size_t bytes = 0;
		NTSTATUS status = STATUS_UNSUCCESSFUL;

		PEPROCESS process = 0;
		if ( !NT_SUCCESS( PsLookupProcessByProcessId( reinterpret_cast< HANDLE >( data->process_id ), &process ) ) ) {
			return STATUS_UNSUCCESSFUL;
		}

		auto physical_address = physical::translate_linear_address( data->directory_table, data->virtual_address );

		if ( !physical_address ) {
			return STATUS_UNSUCCESSFUL;
		}

		auto final_size = find_min( PAGE_SIZE - ( physical_address & 0xFFF ), data->size );


		if ( !NT_SUCCESS( physical::read_physical_address( ( PVOID )physical_address, reinterpret_cast< void* >( ( reinterpret_cast< ULONG64 >( data->buffer ) ) ), final_size, &bytes ) ) )
		{
			ObfDereferenceObject( process );
			return STATUS_UNSUCCESSFUL;
		}

		ObfDereferenceObject( process );
		return STATUS_SUCCESS;
	}
}