#include <catch.hpp>
#define PROGRAMOPTIONS_SILENT
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "errors", "[ProgramOptions]" ) {
	po::parser parser;
	auto&& sum = parser[ "sum" ]
		.abbreviation( 's' )
		.type( po::i64 )
		.multi();
	auto&& unnamed = parser[ "" ];

	SECTION( "Scenario 1" ) {
		const arg_provider A {
			"/Test",
			"--$"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( !unnamed.was_set() );
	}
	SECTION( "Scenario 2" ) {
		const arg_provider A {
			"/Test",
			"---"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( !unnamed.was_set() );
	}
	SECTION( "Scenario 3" ) {
		const arg_provider A {
			"/Test",
			"-- sum=42"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( !unnamed.was_set() );
	}
}
