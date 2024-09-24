#include "base.hpp"

namespace module_base
{
	NTSTATUS get_module_base( pbase data )
	{
		if ( !data->process_id )
			return STATUS_UNSUCCESSFUL;

		PEPROCESS process = NULL;
		PsLookupProcessByProcessId( ( HANDLE )data->process_id, &process );

		if ( !process )
			return STATUS_UNSUCCESSFUL;

		ULONGLONG image_base = ( ULONGLONG )PsGetProcessSectionBaseAddress( process );

		if ( !image_base )
			return STATUS_UNSUCCESSFUL;

		RtlCopyMemory( data->address, &image_base, sizeof( image_base ) ); // can be done without crt. figure out how by yourself / RtlCopyMemory is a definition of memcpy / 
		ObDereferenceObject( process );

		return STATUS_SUCCESS;
	}
}