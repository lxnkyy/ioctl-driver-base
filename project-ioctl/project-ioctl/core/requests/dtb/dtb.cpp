#include "dtb.hpp"

namespace directory_table_base
{
	NTSTATUS resolve_dtb( pdtb data )
	{
		PEPROCESS process = 0;
		if ( PsLookupProcessByProcessId( ( HANDLE )data->process_id, &process ) != STATUS_SUCCESS )
		{
			return STATUS_UNSUCCESSFUL;
		}

		auto process_dtb = pml4::dirbase_from_base_address( ( void* )PsGetProcessSectionBaseAddress( process ) );

		RtlCopyMemory( data->dtb, &process_dtb, sizeof( process_dtb ) );
		ObfDereferenceObject( process );

		return STATUS_SUCCESS;
	}
}