#include <catch.hpp>
#define PROGRAMOPTIONS_SILENT
#undef NDEBUG
#undef PROGRAMOPTIONS_NO_EXCEPTIONS
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"
#include <stdexcept>

TEST_CASE( "exceptions", "[ProgramOptions]" ) {
	po::parser parser;
	auto&& a = parser[ "a" ]
		.type( po::f32 );
	auto&& bc = parser[ "bc" ]
		.type( po::string )
		.multi();
	SECTION( "designators" ) {
		CHECK_THROWS_AS( parser[ "-foo" ], std::logic_error );
		CHECK_NOTHROW( parser[ "bar32" ] );
		CHECK_THROWS_AS( parser[ "sepa rated" ], std::logic_error );
		CHECK_THROWS_AS( parser[ " " ], std::logic_error );
		CHECK_NOTHROW( parser[ "foo_BAR" ] );
	}
	SECTION( "get()" ) {
		CHECK_THROWS_AS( a.get(), std::logic_error );
		CHECK_THROWS_AS( bc.get(), std::logic_error );
		const arg_provider A {
			"/Test",
			"--bc=foo"
		};
		CHECK( parser( A.argc, A.argv ) );
		CHECK_THROWS_AS( a.get(), std::logic_error );
		CHECK_THROWS_AS( a.get( 0 ), std::logic_error );
		CHECK_NOTHROW( bc.get( 0 ) );
		CHECK_THROWS_AS( bc.get( 1 ), std::logic_error );
	}
	SECTION( "begin()" ) {
		CHECK_NOTHROW( a.begin< po::f32 >() );
		CHECK_NOTHROW( a.begin< po::string >() );
		CHECK_THROWS_AS( a.begin< po::i32 >(), std::logic_error );
		CHECK_NOTHROW( bc.begin< po::string >() );
		CHECK_THROWS_AS( bc.begin< po::i32 >(), std::logic_error );
	}
	SECTION( "abbreviation()" ) {
		CHECK_NOTHROW( a.abbreviation( 'A' ) );
		CHECK_THROWS_AS( a.abbreviation( ' ' ), std::logic_error );
		CHECK_THROWS_AS( bc.abbreviation( '-' ), std::logic_error );
	}
	SECTION( "description()" ) {
		CHECK_THROWS_AS( a.description( "\t" ), std::logic_error );
		CHECK_THROWS_AS( bc.description( "foo\vbar" ), std::logic_error );
	}
	SECTION( "type()" ) {
		CHECK_THROWS_AS( a.type( static_cast< po::value_type >( 123 ) ), std::logic_error );
	}
	SECTION( "fallback()" ) {
		CHECK_NOTHROW( a.fallback( 0 ) );
		CHECK_NOTHROW( a.fallback( 2.3e-3 ) );
		CHECK_NOTHROW( a.fallback( "2.3e-3" ) );
		CHECK_THROWS_AS( a.fallback( "" ), std::logic_error );
		CHECK_THROWS_AS( a.fallback( "unintelligible" ), std::logic_error );
		bc.type( po::i32 );
		CHECK_NOTHROW( bc.fallback( "0" ) );
		CHECK_NOTHROW( bc.fallback( 12, "34", 56, "78", 90 ) );
		CHECK_THROWS_AS( bc.fallback( 2.3e-3 ), std::logic_error );
		CHECK_THROWS_AS( bc.fallback( 12, "unintelligible" ), std::logic_error );
		auto&& v = parser[ "void" ];
		CHECK_THROWS_AS( v.fallback( "" ), std::logic_error );
	}
	SECTION( "callback()" ) {
		CHECK_NOTHROW( a.callback( []{} ) );
		CHECK_NOTHROW( a.callback( []( float ){} ) );
		CHECK_NOTHROW( a.callback( []( std::string ){} ) );
		CHECK_THROWS_AS( a.callback( []( float, float ){} ), std::logic_error );
		CHECK_NOTHROW( bc.callback( []{} ) );
		CHECK_NOTHROW( bc.callback( []( std::string ){} ) );
		CHECK_THROWS_AS( bc.callback( []( std::string, std::string ){} ), std::logic_error );
	}
	SECTION( "option settings order" ) {
#define Agnostic1		.abbreviation( 'X' )
#define Agnostic2		.description( "foo" )
#define FirstStage1		.type( po::i32 )
#define FirstStage2		.multi()
#define SecondStage1	.fallback( "-0xBABE" )
#define SecondStage2	.callback( [ & ]( int ){} )

		auto&& e = parser[ "empty" ];
		SECTION( "valid 1" ) {
			CHECK_NOTHROW( e Agnostic1 Agnostic2 FirstStage1 FirstStage2 SecondStage1 SecondStage2 );
		}
		SECTION( "valid 2" ) {
			CHECK_NOTHROW( e FirstStage2 FirstStage1 SecondStage2 SecondStage1 Agnostic2 Agnostic1 );
		}
		SECTION( "invalid 1" ) {
			CHECK_THROWS_AS( e Agnostic1 Agnostic2 FirstStage1 SecondStage1 SecondStage2 FirstStage2, std::logic_error );
		}
		SECTION( "invalid 2" ) {
			CHECK_THROWS_AS( e SecondStage2 SecondStage1 FirstStage2 FirstStage1 Agnostic2 Agnostic1, std::logic_error );
		}
		SECTION( "invalid 3" ) {
			CHECK_THROWS_AS( e FirstStage2 SecondStage1 SecondStage2 Agnostic1 Agnostic2 FirstStage1, std::logic_error );
		}
		SECTION( "invalid 4" ) {
			CHECK_THROWS_AS( e SecondStage2 SecondStage1 Agnostic2 Agnostic1, std::logic_error );
		}

#undef Agnostic1
#undef Agnostic2
#undef FirstStage1
#undef FirstStage2
#undef SecondStage1
#undef SecondStage2
	}
	SECTION( "reconfiguring options after parsing" ) {
		const arg_provider A {
			"/Test"
		};
		CHECK( parser( A.argc, A.argv ) );
		CHECK_THROWS_AS( a.type( po::void_ ), std::logic_error );
		CHECK_THROWS_AS( a.multi(), std::logic_error );
		CHECK_THROWS_AS( bc.type( po::u64 ), std::logic_error );
		CHECK_THROWS_AS( bc.single(), std::logic_error );
	}
}
