#include <catch2/catch.hpp>
#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE("bind", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	std::string a;
	auto&& a_opt = parser["a"].bind(a);
	REQUIRE(a_opt.is_single());
	REQUIRE(a_opt.get_type() == po::string);

	std::int32_t b{};
	auto&& b_opt = parser["b"].bind(b);
	REQUIRE(b_opt.is_single());
	REQUIRE(b_opt.get_type() == po::i32);

	std::int64_t c{};
	auto&& c_opt = parser["c"].bind(c);
	REQUIRE(c_opt.is_single());
	REQUIRE(c_opt.get_type() == po::i64);

	std::uint32_t d{};
	auto&& d_opt = parser["d"].bind(d);
	REQUIRE(d_opt.is_single());
	REQUIRE(d_opt.get_type() == po::u32);

	std::uint64_t e{};
	auto&& e_opt = parser["e"].bind(e);
	REQUIRE(e_opt.is_single());
	REQUIRE(e_opt.get_type() == po::u64);

	// float f{};
	// auto&& f_opt = parser["f"].bind(f);
	// REQUIRE(f_opt.is_single());
	// REQUIRE(f_opt.get_type() == po::f32);

	// double g{};
	// auto&& g_opt = parser["g"].bind(g);
	// REQUIRE(g_opt.is_single());
	// REQUIRE(g_opt.get_type() == po::f64);

	std::vector<std::int32_t> h;
	auto&& h_opt = parser["h"].bind(h);
	REQUIRE(h_opt.is_multi());
	REQUIRE(h_opt.get_type() == po::i32);

	std::deque<std::string> i;
	auto&& i_opt = parser["i"].bind(i);
	REQUIRE(i_opt.is_multi());
	REQUIRE(i_opt.get_type() == po::string);

	std::list<std::int32_t> j;
	auto&& j_opt = parser["j"].bind(j);
	REQUIRE(j_opt.is_multi());
	REQUIRE(j_opt.get_type() == po::i32);

	std::stack<std::int64_t> k;
	auto&& k_opt = parser["k"].bind(k);
	REQUIRE(k_opt.is_multi());
	REQUIRE(k_opt.get_type() == po::i64);

	std::queue<std::uint32_t> l;
	auto&& l_opt = parser["l"].bind(l);
	REQUIRE(l_opt.is_multi());
	REQUIRE(l_opt.get_type() == po::u32);

	std::priority_queue<std::uint64_t> m;
	auto&& m_opt = parser["m"].bind(m);
	REQUIRE(m_opt.is_multi());
	REQUIRE(m_opt.get_type() == po::u64);

	SECTION("good scenario") {
		const arg_provider A {
			"/Test",
			"-a" "hello",
			"-h 36",
			"-h 42",
			"-i" "foo",
			"-i" "bar",
			"-k 2",
			"-k 7",
			"-b 3",
			"-b 4",
		};

		CHECK(parser(A.argc, A.argv));

		REQUIRE(a_opt.count() == 1);
		CHECK(a == "hello");

		REQUIRE(b_opt.count() == 1);
		CHECK(b == 4);

		REQUIRE(h_opt.count() == 2);
		REQUIRE(h.size() == 2);
		CHECK(h[0] == 36);
		CHECK(h[1] == 42);
		CHECK(h == h_opt.to_vector<po::i32>());

		REQUIRE(i_opt.count() == 2);
		REQUIRE(i.size() == 2);
		CHECK(i[0] == "foo");
		CHECK(i[1] == "bar");

		REQUIRE(k_opt.count() == 2);
		REQUIRE(k.size() == 2);
		CHECK(k.top() == 7);
	}
}
