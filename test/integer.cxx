#include <catch.hpp>
#define PROGRAMOPTIONS_SILENT
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "integer", "[ProgramOptions]" ) {
	po::parser parser;
	auto&& a = parser[ "a" ]
		.type( po::i64 );
	auto&& b = parser[ "bot" ]
		.abbreviation( 'b' )
		.type( po::i64 )
		.multi();
	auto&& c = parser[ "c" ]
		.type( po::i64 )
		.fallback( 60 );
	auto&& d = parser[ "d" ]
		.type( po::i64 )
		.multi()
		.fallback( -5, 0, "0xAf" );

	REQUIRE( a.count() == 0 );
	REQUIRE( b.count() == 0 );
	REQUIRE( c.count() == 1 );
	REQUIRE( c.get().i64 == 60 );
	REQUIRE( d.count() == 3 );
	REQUIRE( d.get( 0 ).i64 == -5 );
	REQUIRE( d.get( 1 ).i64 == 0 );
	REQUIRE( d.get( 2 ).i64 == 0xAF );

	SECTION( "Scenario 1" ) {
		const arg_provider A {
			"/Test",
			"-a -13",
			"--bot1",
			"--bot -2",
			"-b3",
			"--bot",
			"-4",
			"-c=-10e15",
			"-c=42",
			"-d0",
			"-d",
			"6",
			"-d -10",
			"-d=-5",
			"-d-0"
		};
		REQUIRE( parser( A.argc, A.argv ) );
		REQUIRE( a.count() == 1 );
		CHECK( a.get().i64 == -13 );
		REQUIRE( b.count() == 4 );
		CHECK( b.get( 0 ).i64 == 1 );
		CHECK( b.get( 1 ).i64 == -2 );
		CHECK( b.get( 2 ).i64 == 3 );
		CHECK( b.get( 3 ).i64 == -4 );
		REQUIRE( c.count() == 1 );
		CHECK( c.get().i64 == 42 );
		REQUIRE( d.count() == 5 );
		CHECK( d.get( 0 ).i64 == 0 );
		CHECK( d.get( 1 ).i64 == 6 );
		CHECK( d.get( 2 ).i64 == -10 );
		CHECK( d.get( 3 ).i64 == -5 );
		CHECK( d.get( 4 ).i64 == 0 );
	}
	SECTION( "Scenario 2" ) {
		const arg_provider A {
			"/Test",
			"-a4",
			"-a"
		};
		CHECK( !parser( A.argc, A.argv ) );
		REQUIRE( a.count() == 1 );
		CHECK( a.get().i64 == 4 );
	}
	SECTION( "Scenario 3" ) {
		const arg_provider A {
			"/Test",
			"-d"
		};
		CHECK( !parser( A.argc, A.argv ) );
		REQUIRE( d.count() == 3 );
		CHECK( d.get( 0 ).i64 == -5 );
		CHECK( d.get( 1 ).i64 == 0 );
		CHECK( d.get( 2 ).i64 == 0xAF );
	}
}
