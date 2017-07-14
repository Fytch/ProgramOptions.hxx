#include <catch.hpp>
#include <ProgramOptions.hxx>
#include <string>
#include <limits>

TEST_CASE( "str2int", "[ProgramOptions]" ) {
	CHECK( po::str2int< int >( "+0" ).good() );
	CHECK( po::str2int< int >( "+0" ) == 0 );
	CHECK( po::str2int< int >( "-0" ).good() );
	CHECK( po::str2int< int >( "-0" ) == 0 );
	CHECK( po::str2int< int >( " -8127  " ) == -8127 );
	CHECK( po::str2int< int >( "-35e2" ) == -3500 );
	CHECK( po::str2int< int >( "-002e+01" ) == -20 );
	CHECK( po::str2int< int >( "-0e1" ).good() );
	CHECK( po::str2int< int >( "-0e1" ) == 0 );
	CHECK( po::str2int< int >( "-43e0" ) == -43 );
	CHECK( po::str2int< int >( "-000000000000000000000001" ) == -1 );
	CHECK( po::str2int< long long >( "-2e13" ) == -20000000000000 );

	std::string min = std::to_string( std::numeric_limits< int >::min() );
	CHECK( po::str2int< int >( min ) == std::numeric_limits< int >::min() );
	++min.back();
	CHECK( !po::str2int< int >( min ) );

	std::string max = std::to_string( std::numeric_limits< int >::max() );
	CHECK( po::str2int< int >( max ) == std::numeric_limits< int >::max() );
	++max.back();
	CHECK( !po::str2int< int >( max ) );

	CHECK( !po::str2int< int >( "" ) );
	CHECK( !po::str2int< int >( "+" ) );
	CHECK( !po::str2int< int >( "-" ) );
	CHECK( !po::str2int< int >( " " ) );
	CHECK( !po::str2int< int >( "-abc" ) );
	CHECK( !po::str2int< int >( "-12ab" ) );
	CHECK( !po::str2int< int >( "0.94" ) );
	CHECK( !po::str2int< int >( "++3" ) );
	CHECK( !po::str2int< int >( "+-3" ) );
	CHECK( !po::str2int< int >( "-+3" ) );
	CHECK( !po::str2int< int >( "--3" ) );
	CHECK( !po::str2int< int >( "3 a" ) );
	CHECK( !po::str2int< int >( "3e" ) );
	CHECK( !po::str2int< int >( "e4" ) );
	CHECK( !po::str2int< int >( "5e-4" ) );

	CHECK( po::str2int< int >( "-0xAf" ) == -0xAF );
	CHECK( po::str2int< int >( "-0x13579BDF" ) == -0x13579BDF );
	CHECK( po::str2int< int >( "-0x02468ACE" ) == -0x02468ACE );
	CHECK( !po::str2int< int >( "0x" ) );
	CHECK( !po::str2int< int >( "0x a" ) );
	CHECK( !po::str2int< int >( "0x0x" ) );
	CHECK( po::str2int< int >( "0x000000000000000000000000AE03" ) == 0xAE03 );
}
