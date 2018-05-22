#include <ProgramOptions.hxx>
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <iostream>

int main( int argc, char** argv ) {
	po::parser parser;

	std::uint32_t optimization = 0;	// the value we set here acts as an implicit fallback
	parser[ "optimization" ]		// corresponds to --optimization
		.abbreviation( 'O' )		// corresponds to -O
		.description( "set the optimization level (default: -O0)" )
		.bind( optimization );		// write the parsed value to the variable 'optimization'
									// .bind( optimization ) automatically calls .type( po::u32 ) and .single()

	std::vector< std::string > include_paths;
	parser[ "include-path" ]		// corresponds to --include-path
		.abbreviation( 'I' )		// corresponds to -I
		.description( "add an include path" )
		.bind( include_paths );		// append paths to the vector 'include_paths'

	parser[ "help" ]				// corresponds to --help
		.abbreviation( '?' )		// corresponds to -?
		.description( "print this help screen" );

	std::deque< std::string > files;
	parser[ "" ]					// the unnamed parameter is used for non-option arguments, i.e. gcc a.c b.c
		.bind( files );				// append paths to the deque 'include_paths

	// parsing returns false if at least one error has occurred
	if( !parser( argc, argv ) )
		return -1;

	// we don't want to print anything else if the help screen has been displayed
	if( parser[ "help" ].was_set() ) {
		std::cout << parser << '\n';
		return 0;
	}

	// print the parsed values
	// note that we don't need to access parser anymore; all data is stored in the bound variables
	std::cout << "optimization level = " << optimization << '\n';

	std::cout << "include files (" << files.size() << "):\n";
	for( auto&& i : files )
		std::cout << '\t' << i << '\n';

	std::cout << "include paths (" << include_paths.size() << "):\n";
	for( auto&& i : include_paths )
		std::cout << '\t' << i << '\n';
}
