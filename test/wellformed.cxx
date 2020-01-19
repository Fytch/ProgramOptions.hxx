#include <catch2/catch.hpp>
#include <ProgramOptions.hxx>

TEST_CASE( "wellformed", "[ProgramOptions]" ) {
	po::parser parser;
	CHECK( parser.wellformed() );
	SECTION( "repetitive abbreviations" ) {
		parser[ "aa" ]
			.abbreviation( 'a' );
		parser[ "bb" ]
			.abbreviation( 'b' );
		parser[ "cc" ]
			.abbreviation( 'a' );
		CHECK_FALSE( parser.wellformed() );
	}
	SECTION( "abbreviation for the unnamed parameter" ) {
		parser[ "" ]
			.abbreviation( 'a' );
		CHECK_FALSE( parser.wellformed() );
	}
}
