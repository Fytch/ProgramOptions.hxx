#include <catch2/catch.hpp>
#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE("integer", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	auto&& a = parser["a"]
		.type(po::i64);
	auto&& b = parser["bot"]
		.abbreviation('b')
		.type(po::i64)
		.multi();
	auto&& c = parser["c"]
		.type(po::i64)
		.fallback(60);
	auto&& d = parser["d"]
		.type(po::i64)
		.multi()
		.fallback(-5, 0, "0xAf");
	auto&& one = parser["one"]
		.type(po::i32)
		.abbreviation('1');

	REQUIRE(a.count() == 0);
	REQUIRE(b.count() == 0);
	REQUIRE(c.count() == 1);
	REQUIRE(c.get().i64 == 60);
	REQUIRE(d.count() == 3);
	REQUIRE(d.get(0).i64 == -5);
	REQUIRE(d.get(1).i64 == 0);
	REQUIRE(d.get(2).i64 == 0xAF);
	REQUIRE(one.count() == 0);

	SECTION("good scenario") {
		const arg_provider A {
			"/Test",
			"-a -13",
			"--bot=1",
			"-b=-2",
			"-b3",
			"--bot",
			"-4",
			"-c=-10e15",
			"-c=42",
			"-d0",
			"-d",
			"6",
			"-d -10",
			"-d=-5",
			"-d-0",
			"-1",
			"-2"
		};
		CHECK(parser(A.argc, A.argv));
		REQUIRE(a.count() == 1);
		CHECK(a.get().i64 == -13);
		REQUIRE(b.count() == 4);
		CHECK(b.get(0).i64 == 1);
		CHECK(b.get(1).i64 == -2);
		CHECK(b.get(2).i64 == 3);
		CHECK(b.get(3).i64 == -4);
		REQUIRE(c.count() == 1);
		CHECK(c.get().i64 == 42);
		REQUIRE(d.count() == 5);
		CHECK(d.get(0).i64 == 0);
		CHECK(d.get(1).i64 == 6);
		CHECK(d.get(2).i64 == -10);
		CHECK(d.get(3).i64 == -5);
		CHECK(d.get(4).i64 == 0);
		REQUIRE(one.count() == 1);
		CHECK(one.get().i32 == -2);
	}
	SECTION("missing obligatory argument") {
		const arg_provider A {
			"/Test",
			"-a4",
			"-a"
		};
		CHECK(!parser(A.argc, A.argv));
		REQUIRE(a.count() == 1);
		CHECK(a.get().i64 == 4);
	}
	SECTION("missing obligatory argument") {
		const arg_provider A {
			"/Test",
			"-d"
		};
		CHECK(!parser(A.argc, A.argv));
		REQUIRE(d.count() == 3);
		CHECK(d.get(0).i64 == -5);
		CHECK(d.get(1).i64 == 0);
		CHECK(d.get(2).i64 == 0xAF);
	}
	SECTION("invalid long option syntax") {
		const arg_provider A {
			"/Test",
			"--bot40"
		};
		CHECK(!parser(A.argc, A.argv));
		REQUIRE(b.count() == 0);
	}
	SECTION("invalid use of an option as an argument") {
		const arg_provider A {
			"/Test",
			"-1",
			"-4",
			"-1",
			"-1"
		};
		CHECK(!parser(A.argc, A.argv));
		REQUIRE(one.count() == 1);
		CHECK(one.get().i32 == -4);
	}
}
