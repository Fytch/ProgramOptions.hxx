#include <catch2/catch.hpp>
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE("errors", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	auto&& sum = parser["sum"]
		.abbreviation('s')
		.type(po::i64)
		.multi();
	auto&& unnamed = parser[""];

	SECTION("parsing '--$'") {
		const arg_provider A {
			"/Test",
			"--$"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '---'") {
		const arg_provider A {
			"/Test",
			"---"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '-- sum=42'") {
		const arg_provider A {
			"/Test",
			"-- sum=42"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '--sum42'") {
		const arg_provider A {
			"/Test",
			"--sum42"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '--sumgarbage'") {
		const arg_provider A {
			"/Test",
			"--sumgarbage"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '--sum=garbage'") {
		const arg_provider A {
			"/Test",
			"--sum=garbage"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
	SECTION("parsing '--sum garbage'") {
		const arg_provider A {
			"/Test",
			"--sum",
			"garbage"
		};
		CHECK(!parser(A.argc, A.argv));
		CHECK(!sum.was_set());
		CHECK(!unnamed.was_set());
	}
}
