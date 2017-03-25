#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
	po::parser parser;
	parser[ "optimization" ]	// corresponds to --optimization
		.abbreviation( 'O' )	// corresponds to -O
		.description( "set the optimization level (default: -O0)" )
								// the description to be shown when printing the parser
		.type( po::u32 )		// expects an unsigned 32-bit integer
		.fallback( 0 );			// if --optimization is not explicitly specified, assume 0

	parser[ "include-path" ]	// corresponds to --include-path
		.abbreviation( 'I' )	// corresponds to -I
		.description( "add an include path" )
		.type( po::string )		// expects a string
		.multi();				// allows multiple arguments for the same option

	parser[ "help" ]			// corresponds to --help
		.abbreviation( '?' )	// corresponds to -?
		.description( "print this help screen" )
		// .type( po::void_ )	// redundant; default for named parameters
		// .single()			// redundant; default for named parameters
		.callback( [ & ]{ std::cout << parser << '\n'; } );
								// callbacks get invoked when the option occurs

	parser[ "" ]				// the unnamed parameter is used for direct arguments, i.e. gcc a.c b.c
		// .type( po::string )	// redundant; default for the unnamed parameter
		// .multi()				// redundant; default for the unnamed parameter
		.callback( [ & ]( std::string const& x ){ std::cout << "processed \'" << x << "\' successfully!\n"; } );
								// as .get_type() == po::string, the callback may either take a std::string

	// parsing returns false if at least one error has occurred
	if( !parser( argc, argv ) ) {
		std::cerr << "errors occurred; aborting\n";
		return -1;
	}
	// we don't want to print anything else if the help screen has been displayed
	if( parser[ "help" ].size() )
		return 0;

	std::cout << "processed files: " << parser[ "" ].size() << '\n';

	auto&& O = parser[ "optimization" ];
	// .was_set() reports whether the option was specified by the user or relied on the predefined fallback value.
	std::cout << "optimization level (" << ( O.was_set() ? "manual" : "auto" ) << ") = " << O.get().u32 << '\n';

	auto&& I = parser[ "include-path" ];
	// .size() and .count() return the number of given arguments. Without .multi(), their return value is always <= 1.
	std::cout << "included paths (" << I.size() << "):\n";
	// Here, the non-template .begin() / .end() methods were used. Their value type is
	// po::value, which is not a value in itself but contains the desired values as members, i.e. i.string.
	for( auto&& i : I )
		std::cout << '\t' << i.string << '\n';
}
