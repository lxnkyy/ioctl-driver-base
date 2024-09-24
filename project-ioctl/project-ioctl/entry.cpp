#include <dependencies/includes.hpp>

NTSTATUS io_controller( PDEVICE_OBJECT device_obj, PIRP irp ) {
	UNREFERENCED_PARAMETER( device_obj );

	NTSTATUS status = { };
	ULONG bytes = { };
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation( irp );

	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
	ULONG size = stack->Parameters.DeviceIoControl.InputBufferLength;


	if ( code == code_base ) {
		if ( size == sizeof( invoke_base ) ) {
			pbase req = ( pbase )( irp->AssociatedIrp.SystemBuffer );

			status = module_base::get_module_base( req );
			bytes = sizeof( invoke_base );
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if ( code == code_read ) {

		if ( size == sizeof( invoke_read ) ) {
			pread req = ( pread )( irp->AssociatedIrp.SystemBuffer );

			status = memory::read( req );
			bytes = sizeof( invoke_read );
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if ( code == code_dtb ) {
		if ( size == sizeof( invoke_dtb ) ) {
			pdtb req = ( pdtb )( irp->AssociatedIrp.SystemBuffer );

			status = directory_table_base::resolve_dtb( req );
			bytes = sizeof( invoke_dtb );
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}


	irp->IoStatus.Status = status;
	irp->IoStatus.Information = bytes;
	IoCompleteRequest( irp, IO_NO_INCREMENT );

	return status;
}

NTSTATUS unsupported_dispatch( PDEVICE_OBJECT device_obj, PIRP irp ) {
	UNREFERENCED_PARAMETER( device_obj );

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest( irp, IO_NO_INCREMENT );

	return irp->IoStatus.Status;
}

NTSTATUS dispatch_handler( PDEVICE_OBJECT device_obj, PIRP irp ) {
	UNREFERENCED_PARAMETER( device_obj );

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation( irp );

	switch ( stack->MajorFunction ) {
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE:
		break;
	default:
		break;
	}

	IoCompleteRequest( irp, IO_NO_INCREMENT );
	return irp->IoStatus.Status;
}

void unload_drv( PDRIVER_OBJECT drv_obj ) {
	NTSTATUS status = { };

	status = IoDeleteSymbolicLink( &globals::link );

	if ( !NT_SUCCESS( status ) )
		return;

	IoDeleteDevice( drv_obj->DeviceObject );
}

NTSTATUS initialize_driver( PDRIVER_OBJECT drv_obj, PUNICODE_STRING path ) {
	UNREFERENCED_PARAMETER( path );

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT device_obj = NULL;

	UNICODE_STRING name, link;
	RtlInitUnicodeString( &name, L"\\Device\\IoControlDevice" );
	RtlInitUnicodeString( &link, L"\\DosDevices\\IoControl" );

	// Create the device
	status = IoCreateDevice( drv_obj, 0, &name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_obj );
	if ( !NT_SUCCESS( status ) ) {
		return status;
	}

	// Create a symbolic link
	status = IoCreateSymbolicLink( &link, &name );
	if ( !NT_SUCCESS( status ) ) {
		IoDeleteDevice( device_obj );
		return status;
	}

	// Set up IRP dispatch functions
	for ( int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++ ) {
		drv_obj->MajorFunction[ i ] = &unsupported_dispatch;
	}

	drv_obj->MajorFunction[ IRP_MJ_CREATE ] = &dispatch_handler;
	drv_obj->MajorFunction[ IRP_MJ_CLOSE ] = &dispatch_handler;
	drv_obj->MajorFunction[ IRP_MJ_DEVICE_CONTROL ] = &io_controller;
	drv_obj->DriverUnload = &unload_drv;

	// Configure device flags
	device_obj->Flags |= DO_BUFFERED_IO;
	device_obj->Flags &= ~DO_DEVICE_INITIALIZING;

	return status;
}


NTSTATUS entry( PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath ) {
	UNREFERENCED_PARAMETER( DriverObject );
	UNREFERENCED_PARAMETER( RegistryPath );

	return IoCreateDriver( NULL, &initialize_driver );
}
