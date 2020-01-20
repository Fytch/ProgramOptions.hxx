#include <catch2/catch.hpp>
#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include <limits>
#include <string>
#include <cmath>

TEST_CASE("str2flt", "[ProgramOptions]") {
	const double inf = std::numeric_limits<double>::infinity();

	CHECK(po::str2flt<double>("").error == po::error_code::conversion_error);

	CHECK(po::str2flt<double>("+0").good());
	CHECK(po::str2flt<double>("+0") == 0.0);
	CHECK(po::str2flt<double>("-0").good());
	CHECK(po::str2flt<double>("-0") == -0.0);
	CHECK(po::str2flt<double>("123.456") == Approx(123.456));
	CHECK(po::str2flt<double>("-42.36") == Approx(-42.36));
	CHECK(po::str2flt<double>(".53") == Approx(0.53));
	CHECK(po::str2flt<double>("21.") == Approx(21.0));
	CHECK(po::str2flt<double>("-.07") == Approx(-0.07));
	CHECK(po::str2flt<double>("-07.") == Approx(-7.0));

	CHECK(po::str2flt<double>("123.456e-12") == Approx(123.456e-12));
	CHECK(po::str2flt<double>("-42.36E8") == Approx(-42.36e+8));
	CHECK(po::str2flt<double>(".53e0") == Approx(0.53));
	CHECK(po::str2flt<double>("21.e+3") == Approx(21.0e+3));

	CHECK(po::str2flt<double>("15.3e+806").error == po::error_code::out_of_range);
	CHECK(po::str2flt<double>("1e9999999999999999999999999999").error == po::error_code::out_of_range);
	std::string big(500, '9');
	CHECK(po::str2flt<double>(big.begin(), big.end()).error == po::error_code::out_of_range);
	CHECK(po::str2flt<double>("15.3e-806") == Approx(0.0));
	CHECK(po::str2flt<double>("1e-9999999999999999999999999999") == Approx(0.0));

	CHECK(po::str2flt<double>(".").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("-.").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("++0").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("+-0").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("-+0").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("--0").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("4.a").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("2.5 a").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("e3").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("1e").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("1e+").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("1e+2.0").error == po::error_code::conversion_error);

	CHECK(po::str2flt<double>("n").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("na").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("nan").good());
	CHECK(po::str2flt<double>("nann").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("nanE0").error == po::error_code::conversion_error);
	CHECK(std::isnan(po::str2flt<double>("nan").get()));
	CHECK(std::isnan(po::str2flt<double>("+nan").get()));
	CHECK(std::isnan(po::str2flt<double>("-nan").get()));

	CHECK(po::str2flt<double>("i").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("in").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("inf").good());
	CHECK(po::str2flt<double>("infi").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("infin").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("infini").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("infinit").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("infinity").good());
	CHECK(po::str2flt<double>("infinityy").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("infinityE0").error == po::error_code::conversion_error);
	CHECK(po::str2flt<double>("inf") == inf);
	CHECK(po::str2flt<double>("+inf") == inf);
	CHECK(po::str2flt<double>("-inf") == -inf);
	CHECK(po::str2flt<double>("infinity") == inf);
	CHECK(po::str2flt<double>("+infinity") == inf);
	CHECK(po::str2flt<double>("-infinity") == -inf);
}
