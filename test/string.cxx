#include <catch2/catch.hpp>
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE("string", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	auto&& a = parser["a"]
		.type(po::string);
	auto&& b = parser["bot"]
		.abbreviation('b')
		.type(po::string)
		.multi();
	auto&& c = parser["c"]
		.type(po::string)
		.fallback("test");
	auto&& f = parser["f"]
		.type(po::string)
		.multi()
		.fallback(3, "no-exceptions");
	auto&& unnamed = parser[""];

	REQUIRE(a.count() == 0);
	REQUIRE(b.count() == 0);
	REQUIRE(c.count() == 1);
	REQUIRE(c.get().string == "test");
	REQUIRE(f.count() == 2);
	REQUIRE(f.get(0).string == "3");
	REQUIRE(f.get(1).string == "no-exceptions");
	REQUIRE(unnamed.count() == 0);

	SECTION("good scenario 1") {
		const arg_provider A {
			"/Test",
			"-a text",
			"-a/path/to/nowhere",
			"--bot=net",
			"--bot",
			" text",
			"-beast",
			"--bot",
			"whatever",
			"-c=/usr/bin",
			"-f",
			"-f",
			"down here",
			"unnamed arg",
			"-flto",
			"-fmax-errors=25",
			"-fno-rtti"
		};
		CHECK(parser(A.argc, A.argv));
		REQUIRE(a.count() == 1);
		CHECK(a.get().string == "/path/to/nowhere");
		REQUIRE(b.count() == 4);
		CHECK(b.get(0).string == "net");
		CHECK(b.get(1).string == " text");
		CHECK(b.get(2).string == "east");
		CHECK(b.get(3).string == "whatever");
		REQUIRE(c.count() == 1);
		CHECK(c.get().string == "/usr/bin");
		REQUIRE(f.count() == 5);
		CHECK(f.get(0).string == "");
		CHECK(f.get(1).string == "down here");
		CHECK(f.get(2).string == "lto");
		CHECK(f.get(3).string == "max-errors=25");
		CHECK(f.get(4).string == "no-rtti");
		REQUIRE(unnamed.count() == 1);
		CHECK(unnamed.get().string == "unnamed arg");
	}
	SECTION("good scenario 2") {
		const arg_provider A {
			"/Test",
			"-a",
			"foo",
			"-",
			"bar",
			"-foo",
			"--",
			"-flto",
			"--",
			"-fmax-errors=25"
		};
		CHECK(parser(A.argc, A.argv));
		REQUIRE(a.count() == 1);
		CHECK(a.get().string == "foo");
		REQUIRE(b.count() == 0);
		REQUIRE(c.count() == 1);
		CHECK(c.get().string == "test");
		REQUIRE(f.count() == 1);
		CHECK(f.get().string == "oo");
		REQUIRE(unnamed.count() == 5);
		CHECK(unnamed.get(0).string == "-");
		CHECK(unnamed.get(1).string == "bar");
		CHECK(unnamed.get(2).string == "-flto");
		CHECK(unnamed.get(3).string == "--");
		CHECK(unnamed.get(4).string == "-fmax-errors=25");
	}
	SECTION("invalid long option syntax") {
		const arg_provider A {
			"/Test",
			"--bot123",
			"--bot/usr/bin",
			"--bot=/usr/lib"
		};
		CHECK(!parser(A.argc, A.argv));
		REQUIRE(a.count() == 0);
		REQUIRE(b.count() == 1);
		CHECK(b.get(0).string == "/usr/lib");
		REQUIRE(c.count() == 1);
		CHECK(c.get().string == "test");
		REQUIRE(f.count() == 2);
		CHECK(f.get(0).string == "3");
		CHECK(f.get(1).string == "no-exceptions");
		REQUIRE(unnamed.count() == 0);
	}
}
