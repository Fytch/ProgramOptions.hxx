#include <catch.hpp>
#define ProgramOptions_silent
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "option_packs", "[ProgramOptions]" ) {
	po::parser parser;
	auto&& g = parser[ "g" ];
	auto&& h = parser[ "home" ]
		.abbreviation( 'h' )
		.multi();
	auto&& i = parser[ "i" ]
		.multi();
	auto&& j = parser[ "j" ]
		.multi();
	auto&& k = parser[ "k" ]
		.type( po::string );

	REQUIRE( g.count() == 0 );
	REQUIRE( h.count() == 0 );
	REQUIRE( i.count() == 0 );
	REQUIRE( j.count() == 0 );
	REQUIRE( k.count() == 0 );

	SECTION( "Scenario 1" ) {
		const arg_provider A {
			"/Test",
			"-ghhi",
			"-ihhg",
		};
		REQUIRE( parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 4 );
		CHECK( i.count() == 2 );
		CHECK( j.count() == 0 );
		CHECK( k.count() == 0 );
	}
	SECTION( "Scenario 2" ) {
		const arg_provider A {
			"/Test",
			"-ghijk"
		};
		REQUIRE_FALSE( parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 1 );
		CHECK( i.count() == 1 );
		CHECK( j.count() == 1 );
		CHECK( k.count() == 0 );
	}
	SECTION( "Scenario 3" ) {
		const arg_provider A {
			"/Test",
			"-kjihg"
		};
		REQUIRE( parser( A.argc, A.argv ) );
		CHECK( g.count() == 0 );
		CHECK( h.count() == 0 );
		CHECK( i.count() == 0 );
		CHECK( j.count() == 0 );
		CHECK( k.count() == 1 );
	}
	SECTION( "Scenario 4" ) {
		const arg_provider A {
			"/Test",
			"-gkhij"
		};
		REQUIRE_FALSE( parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 1 );
		CHECK( i.count() == 1 );
		CHECK( j.count() == 1 );
		CHECK( k.count() == 0 );
	}
}
