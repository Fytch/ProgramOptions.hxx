#include <catch.hpp>
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE( "bind", "[ProgramOptions]" ) {
	po::parser parser;
	parser.silent();

	std::string a;
	std::int32_t b{};
	std::int64_t c{};
	std::uint32_t d{};
	std::uint64_t e{};
	// float f{};
	// double g{};
	std::vector< std::int32_t > h;
	std::deque< std::string > i;

	auto&& a_opt = parser[ "a" ].bind( a );
	auto&& b_opt = parser[ "b" ].bind( b );
	auto&& c_opt = parser[ "c" ].bind( c );
	auto&& d_opt = parser[ "d" ].bind( d );
	auto&& e_opt = parser[ "e" ].bind( e );
	// auto&& f_opt = parser[ "f" ].bind( f );
	// auto&& g_opt = parser[ "g" ].bind( g );
	auto&& h_opt = parser[ "h" ].bind( h );
	auto&& i_opt = parser[ "i" ].bind( i );

	REQUIRE( a_opt.is_single() );
	REQUIRE( a_opt.get_type() == po::string );
	REQUIRE( b_opt.is_single() );
	REQUIRE( b_opt.get_type() == po::i32 );
	REQUIRE( c_opt.is_single() );
	REQUIRE( c_opt.get_type() == po::i64 );
	REQUIRE( d_opt.is_single() );
	REQUIRE( d_opt.get_type() == po::u32 );
	REQUIRE( e_opt.is_single() );
	REQUIRE( e_opt.get_type() == po::u64 );
	// REQUIRE( f_opt.is_single() );
	// REQUIRE( f_opt.get_type() == po::f32 );
	// REQUIRE( g_opt.is_single() );
	// REQUIRE( g_opt.get_type() == po::f64 );
	REQUIRE( h_opt.is_multi() );
	REQUIRE( h_opt.get_type() == po::i32 );
	REQUIRE( i_opt.is_multi() );
	REQUIRE( i_opt.get_type() == po::string );

	SECTION( "good scenario" ) {
		const arg_provider A {
			"/Test",
			"-a" "hello",
			"-h 36",
			"-h 42",
			"-i" "foo",
			"-i" "bar",
			"-b 36",
			"-b 42",
		};

		CHECK( parser( A.argc, A.argv ) );

		REQUIRE( a_opt.count() == 1 );
		CHECK( a == "hello" );

		REQUIRE( b_opt.count() == 1 );
		CHECK( b == 42 );

		REQUIRE( h_opt.count() == 2 );
		REQUIRE( h.size() == 2 );
		CHECK( h[ 0 ] == 36 );
		CHECK( h[ 1 ] == 42 );
		CHECK( h == h_opt.to_vector< po::i32 >() );

		REQUIRE( i_opt.count() == 2 );
		REQUIRE( i.size() == 2 );
		CHECK( i[ 0 ] == "foo" );
		CHECK( i[ 1 ] == "bar" );
	}
}
