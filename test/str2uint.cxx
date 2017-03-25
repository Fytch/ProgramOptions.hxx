#include <catch.hpp>
#define ProgramOptions_silent
#include <ProgramOptions.hxx>
#include <string>
#include <limits>

TEST_CASE( "str2uint", "[ProgramOptions]" ) {
	REQUIRE( po::pow( 10, 0 ) == 1 );
	REQUIRE( po::pow( 10, 1 ) == 10 );
	REQUIRE( po::pow( 10, 2 ) == 100 );
	REQUIRE( po::pow( 10, 3 ) == 1000 );
	REQUIRE( po::pow( 10, 4 ) == 10000 );
	REQUIRE( po::pow( 10, 5 ) == 100000 );

	CHECK( po::str2uint< unsigned >( "+0" ).good() );
	CHECK( po::str2uint< unsigned >( "+0" ) == 0u );
	CHECK( po::str2uint< unsigned >( " 8127  " ) == 8127u );
	CHECK( po::str2uint< unsigned >( "35e2" ) == 3500u );
	CHECK( po::str2uint< unsigned >( "002e+01" ) == 20u );
	CHECK( po::str2uint< unsigned >( "0e1" ).good() );
	CHECK( po::str2uint< unsigned >( "0e1" ) == 0u );
	CHECK( po::str2uint< unsigned >( "43e0" ) == 43u );
	CHECK( po::str2uint< unsigned >( "000000000000000000000001" ) == 1u );
	CHECK( po::str2uint< unsigned long long >( "2e13" ) == 20000000000000u );

	std::string max = std::to_string( std::numeric_limits< unsigned >::max() );
	CHECK( po::str2uint< unsigned >( max ) == std::numeric_limits< unsigned >::max() );
	++max.back();
	CHECK( !po::str2uint< unsigned >( max ) );

	CHECK( !po::str2uint< unsigned >( "" ) );
	CHECK( !po::str2uint< unsigned >( "+" ) );
	CHECK( !po::str2uint< unsigned >( " " ) );
	CHECK( !po::str2uint< unsigned >( "abc" ) );
	CHECK( !po::str2uint< unsigned >( "12ab" ) );
	CHECK( !po::str2uint< unsigned >( "0.94" ) );
	CHECK( !po::str2uint< unsigned >( "++3" ) );
	CHECK( !po::str2uint< unsigned >( "-3" ) );
	CHECK( !po::str2uint< unsigned >( "3 a" ) );
	CHECK( !po::str2uint< unsigned >( "3e" ) );
	CHECK( !po::str2uint< unsigned >( "e4" ) );
	CHECK( !po::str2uint< unsigned >( "5e-4" ) );

	CHECK( po::str2uint< unsigned >( "0xAf" ) == 0xAFu );
	CHECK( po::str2uint< unsigned >( "0x13579BDF" ) == 0x13579BDFu );
	CHECK( po::str2uint< unsigned >( "0xECA86420" ) == 0xECA86420u );
	CHECK( !po::str2uint< unsigned >( "0x" ) );
	CHECK( !po::str2uint< unsigned >( "0x a" ) );
	CHECK( !po::str2uint< unsigned >( "0x0x" ) );
	CHECK( po::str2uint< unsigned >( "0x000000000000000000000000AE03" ) == 0xAE03u );

	std::string hex_max = "0x00";
	hex_max += std::string( std::numeric_limits< unsigned >::digits / 4, 'F' );
	CHECK( po::str2uint< unsigned >( hex_max ) == ~unsigned{} );
	hex_max.push_back( '0' );
	CHECK( !po::str2uint< unsigned >( hex_max ) );

	CHECK( po::str2uint< unsigned >( "0b1111" ) == 15u );
	CHECK( po::str2uint< unsigned >( "0b0" ).good() );
	CHECK( po::str2uint< unsigned >( "0b0" ) == 0u );
	CHECK( !po::str2uint< unsigned >( "0b" ) );
	CHECK( !po::str2uint< unsigned >( "0b 1" ) );
	CHECK( !po::str2uint< unsigned >( "0b1041" ) );
	CHECK( !po::str2uint< unsigned >( "0b10e0" ) );

	std::string bin_max = "0b0";
	bin_max += std::string( std::numeric_limits< unsigned >::digits, '1' );
	CHECK( po::str2uint< unsigned >( bin_max ) == ~unsigned{} );
	bin_max.push_back( '1' );
	CHECK( !po::str2uint< unsigned >( bin_max ) );

	std::string bin2 = "0b01";
	bin2 += std::string( std::numeric_limits< unsigned >::digits - 1, '0' );
	CHECK( po::str2uint< unsigned >( bin2 ) == ~( ~unsigned{} >> 1 ) );
	bin2.push_back( '0' );
	CHECK( !po::str2uint< unsigned >( bin2 ) );
}
