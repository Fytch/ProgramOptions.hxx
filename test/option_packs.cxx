#include <catch2/catch.hpp>
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "option_packs", "[ProgramOptions]" ) {
	po::parser parser;
	parser.silent();

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
	auto&& l = parser[ "l" ]
		.type( po::i32 );

	REQUIRE( g.count() == 0 );
	REQUIRE( h.count() == 0 );
	REQUIRE( i.count() == 0 );
	REQUIRE( j.count() == 0 );
	REQUIRE( k.count() == 0 );
	REQUIRE( l.count() == 0 );

	SECTION( "Scenario 1" ) {
		const arg_provider A {
			"/Test",
			"-ghhi",
			"-ihhg",
		};
		CHECK( parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 4 );
		CHECK( i.count() == 2 );
		CHECK( j.count() == 0 );
		CHECK( k.count() == 0 );
		CHECK( l.count() == 0 );
	}
	SECTION( "Scenario 2" ) {
		const arg_provider A {
			"/Test",
			"-ghijk"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 1 );
		CHECK( i.count() == 1 );
		CHECK( j.count() == 1 );
		CHECK( k.count() == 0 );
		CHECK( l.count() == 0 );
	}
	SECTION( "Scenario 3" ) {
		const arg_provider A {
			"/Test",
			"-kjihg"
		};
		CHECK( parser( A.argc, A.argv ) );
		CHECK( g.count() == 0 );
		CHECK( h.count() == 0 );
		CHECK( i.count() == 0 );
		CHECK( j.count() == 0 );
		CHECK( k.count() == 1 );
		CHECK( l.count() == 0 );
	}
	SECTION( "Scenario 4" ) {
		const arg_provider A {
			"/Test",
			"-gkhij"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 1 );
		CHECK( i.count() == 1 );
		CHECK( j.count() == 1 );
		CHECK( k.count() == 0 );
	}
	SECTION( "Scenario 5" ) {
		const arg_provider A {
			"/Test",
			"-gl3"
		};
		CHECK( !parser( A.argc, A.argv ) );
		CHECK( g.count() == 1 );
		CHECK( h.count() == 0 );
		CHECK( i.count() == 0 );
		CHECK( j.count() == 0 );
		CHECK( k.count() == 0 );
		CHECK( l.count() == 0 );
	}
}
