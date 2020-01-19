#include <catch2/catch.hpp>
#include <ProgramOptions.hxx>

TEST_CASE("defaults", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	auto&& _1 = parser["abacus"];
	auto&& _2 = parser["a"];
	auto&& _3 = parser[""];

	CHECK(_1.get_abbreviation() == '\0');
	CHECK(_2.get_abbreviation() == 'a');
	CHECK(_3.get_abbreviation() == '\0');

	CHECK(_1.get_type() == po::void_);
	CHECK(_2.get_type() == po::void_);
	CHECK(_3.get_type() == po::string);

	CHECK(_1.is_single() == true);
	CHECK(_2.is_single() == true);
	CHECK(_3.is_multi() == true);
}
