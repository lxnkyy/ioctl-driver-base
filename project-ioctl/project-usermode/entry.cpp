#include <dependencies/includes.hpp>

bool test_offsets( )
{
	__int64 va_text;
	for ( int i = 0; i < 25; i++ )
	{
		if ( device.read<__int32>( device.image_base + ( i * 0x1000 ) + 0x250 ) == 0x260E020B )
		{
			va_text = device.image_base + ( ( i + 1 ) * 0x1000 );
		}
	}

	if ( !va_text )
	{
		e_log( "Failed to get va_text." );

		return 0;
	}
	
	e_log( "Got va_text: 0x%llx", va_text );

	auto uworld = device.read<uintptr_t>( va_text + 0x1178F4B8 );

	if ( !uworld )
	{
		e_log( "Failed to get UWorld." );

		return 0;
	}

	e_log( "Got UWorld: 0x%llx", uworld );

	auto gameinstance = device.read<uintptr_t>( uworld + 0x1d8 );

	if ( !gameinstance )
	{
		e_log( "Failed to get GameInstance." );

		return 0;
	}

	e_log( "Got GameInstance: 0x%llx", gameinstance );

	return 1;
}

bool initialise_driver( )
{
	if ( !device.find_driver( ) )
	{
		e_log( "Failed to find driver's handle." );
		return 0;
	}

	device.process_id = device.find_process( "FortniteClient-Win64-Shipping.exe" );

	if ( !device.process_id )
	{
		e_log( "Failed to target process ID." );
		return 0;
	}

	e_log( "Got target process ID: %d", device.process_id );
	
	device.image_base = device.find_image( );

	if ( !device.image_base )
	{
		e_log( "Failed to target process image base." );
		return 0;
	}

	e_log( "Got target image base: 0x%llx", device.image_base );

	device.dtb = device.resolve_dtb( );

	if ( !device.dtb )
	{
		e_log( "Failed to target process directory table base." );
		return 0;
	}

	e_log( "Got target directory table base: 0x%llx", device.dtb );

	std::cout << std::endl;

	return 1;
}

void entry( )
{
	SetConsoleTitleA( "discord.gg/enigmacheat" );

	initialise_driver( );

	test_offsets( );
	
	std::cin.get( );
}