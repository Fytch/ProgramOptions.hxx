#include <ProgramOptions.hxx>
#include <exception>
#include <iostream>

int main( int argc, char** argv )
try {
	po::parser parser;
	parser[ "help" ]
		.abbreviation( '?' )
		.description( "print this help screen" )
		.callback( [ & ]{ std::cout << parser << '\n'; } );

	parser[ "PLACEHOLDER" ]
		.abbreviation( 'PLACEHOLDER' )
		.description( "PLACEHOLDER" )
		.type( po::PLACEHOLDER )
		.multi()
		.fallback( PLACEHOLDER )
		.callback( [ & ]{ PLACEHOLDER; } );

	if( !parser( argc, argv ) ) {
		// error
		return 1;
	}

	if( parser[ "PLACEHOLDER" ].available() ) {
		// do something
	}
} catch( std::exception const& e ) {
	std::cerr << "uncaught exception: " << e.what() << '\n';
	std::cerr << "terminating application\n";
	return -1;
}
