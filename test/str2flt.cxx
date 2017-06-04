#include <catch.hpp>
#include <ProgramOptions.hxx>
#include <string>
#include <limits>
#include <cmath>

TEST_CASE( "str2flt", "[ProgramOptions]" ) {
	CHECK( po::str2flt< double >( "+0" ).good() );
	CHECK( po::str2flt< double >( "+0" ) == 0.0 );
	CHECK( po::str2flt< double >( "-0" ).good() );
	CHECK( po::str2flt< double >( "-0" ) == -0.0 );
	CHECK( po::str2flt< double >( "123.456" ) == Approx( 123.456 ) );
	CHECK( po::str2flt< double >( "-42.36" ) == Approx( -42.36 ) );
	CHECK( po::str2flt< double >( ".53" ) == Approx( 0.53 ) );
	CHECK( po::str2flt< double >( "21." ) == Approx( 21.0 ) );

	CHECK( po::str2flt< double >( "123.456e-12" ) == Approx( 123.456e-12 ) );
	CHECK( po::str2flt< double >( "-42.36E8" ) == Approx( -42.36e+8 ) );
	CHECK( po::str2flt< double >( ".53e0" ) == Approx( 0.53 ) );
	CHECK( po::str2flt< double >( "21.e+3" ) == Approx( 21.0e+3 ) );
	CHECK( std::isinf( po::str2flt< double >( "15.3e+806" ) ) );
	CHECK( po::str2flt< double >( "15.3e-806" ) == Approx( 0.0 ) );

	CHECK( !po::str2flt< double >( "." ).good() );
	CHECK( !po::str2flt< double >( "-." ).good() );
	CHECK( !po::str2flt< double >( "++0" ).good() );
	CHECK( !po::str2flt< double >( "+-0" ).good() );
	CHECK( !po::str2flt< double >( "-+0" ).good() );
	CHECK( !po::str2flt< double >( "--0" ).good() );
	CHECK( !po::str2flt< double >( "4.a" ).good() );
	CHECK( !po::str2flt< double >( "2.5 a" ).good() );
	CHECK( !po::str2flt< double >( "e3" ).good() );
	CHECK( !po::str2flt< double >( "1e" ).good() );
	CHECK( !po::str2flt< double >( "1e+" ).good() );
	CHECK( !po::str2flt< double >( "1e+2.0" ).good() );

	CHECK( !po::str2flt< double >( "n" ).good() );
	CHECK( !po::str2flt< double >( "na" ).good() );
	CHECK(  po::str2flt< double >( "nan" ).good() );
	CHECK( !po::str2flt< double >( "nann" ).good() );
	CHECK( !po::str2flt< double >( "nanE0" ).good() );
	CHECK( std::isnan( po::str2flt< double >( "nan" ) ) );
	CHECK( std::isnan( po::str2flt< double >( "+nan" ) ) );
	CHECK( std::isnan( po::str2flt< double >( "-nan" ) ) );

	CHECK( !po::str2flt< double >( "i" ).good() );
	CHECK( !po::str2flt< double >( "in" ).good() );
	CHECK(  po::str2flt< double >( "inf" ).good() );
	CHECK( !po::str2flt< double >( "infi" ).good() );
	CHECK( !po::str2flt< double >( "infin" ).good() );
	CHECK( !po::str2flt< double >( "infini" ).good() );
	CHECK( !po::str2flt< double >( "infinit" ).good() );
	CHECK(  po::str2flt< double >( "infinity" ).good() );
	CHECK( !po::str2flt< double >( "infinityy" ).good() );
	CHECK( !po::str2flt< double >( "infinityE0" ).good() );
	CHECK( std::isinf( po::str2flt< double >( "inf" ) ) );
	CHECK( std::isinf( po::str2flt< double >( "+inf" ) ) );
	CHECK( std::isinf( po::str2flt< double >( "-inf" ) ) );
	CHECK( std::isinf( po::str2flt< double >( "infinity" ) ) );
	CHECK( std::isinf( po::str2flt< double >( "+infinity" ) ) );
	CHECK( std::isinf( po::str2flt< double >( "-infinity" ) ) );
}
