#include <catch2/catch.hpp>
#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include "arg_provider.hxx"

TEST_CASE("callback", "[ProgramOptions]") {
	po::parser parser;
	parser.silent();

	po::i32_t a_value{};
	po::string_t a_value_str{};
	std::size_t a_counter_1 = 0;
	std::size_t a_counter_2 = 0;
	parser["a"]
		.type(po::i32)
		.callback([&](po::i32_t const& value){ ++a_counter_1; a_value = value; })
		.callback([&](po::string_t const& value){ ++a_counter_2; a_value_str = value; });

	po::i64_t b_value{};
	po::string_t b_value_str{};
	std::size_t b_counter_1 = 0;
	std::size_t b_counter_2 = 0;
	parser["b"]
		.type(po::i64)
		.callback([&](po::i64_t const& value){ ++b_counter_1; b_value = value; })
		.callback([&](po::string_t const& value){ ++b_counter_2; b_value_str = value; });

	po::u32_t c_value{};
	po::string_t c_value_str{};
	std::size_t c_counter_1 = 0;
	std::size_t c_counter_2 = 0;
	parser["c"]
		.type(po::u32)
		.callback([&](po::u32_t const& value){ ++c_counter_1; c_value = value; })
		.callback([&](po::string_t const& value){ ++c_counter_2; c_value_str = value; });

	po::u64_t d_value{};
	po::string_t d_value_str{};
	std::size_t d_counter_1 = 0;
	std::size_t d_counter_2 = 0;
	parser["d"]
		.type(po::u64)
		.callback([&](po::u64_t const& value){ ++d_counter_1; d_value = value; })
		.callback([&](po::string_t const& value){ ++d_counter_2; d_value_str = value; });

	po::f32_t e_value{};
	po::string_t e_value_str{};
	std::size_t e_counter_1 = 0;
	std::size_t e_counter_2 = 0;
	parser["e"]
		.type(po::f32)
		.callback([&](po::f32_t const& value){ ++e_counter_1; e_value = value; })
		.callback([&](po::string_t const& value){ ++e_counter_2; e_value_str = value; });

	po::f64_t f_value{};
	po::string_t f_value_str{};
	std::size_t f_counter_1 = 0;
	std::size_t f_counter_2 = 0;
	parser["f"]
		.type(po::f64)
		.callback([&](po::f64_t const& value){ ++f_counter_1; f_value = value; })
		.callback([&](po::string_t const& value){ ++f_counter_2; f_value_str = value; });

	po::string_t g_value{};
	std::size_t g_counter_1 = 0;
	parser["g"]
		.type(po::string)
		.callback([&](po::string_t const& value){ ++g_counter_1; g_value = value; });

	std::size_t h_counter_1 = 0;
	parser["h"]
		.type(po::void_)
		.callback([&](){ ++h_counter_1; });

	const arg_provider A {
		"/Test",
		"-a",
		"-65",
		"-b+14e12",
		"-c12",
		"-c0000000",
		"-d2",
		"-d4",
		"-d42",
		"-e0",
		"-f7.39",
		"-f",
		"-7.39000",
		"-g",
		"foo",
		"-g",
		"bar",
		"-hhhh"
	};
	CHECK(parser(A.argc, A.argv));

	CHECK(a_value == -65);
	CHECK(a_value_str == "-65");
	CHECK(a_counter_1 == 1);
	CHECK(a_counter_2 == 1);

	CHECK(b_value == 14000000000000);
	CHECK(b_value_str == "+14e12");
	CHECK(b_counter_1 == 1);
	CHECK(b_counter_2 == 1);

	CHECK(c_value == 0);
	CHECK(c_value_str == "0000000");
	CHECK(c_counter_1 == 2);
	CHECK(c_counter_2 == 2);

	CHECK(d_value == 42);
	CHECK(d_value_str == "42");
	CHECK(d_counter_1 == 3);
	CHECK(d_counter_2 == 3);

	CHECK(e_value == 0);
	CHECK(e_value_str == "0");
	CHECK(e_counter_1 == 1);
	CHECK(e_counter_2 == 1);

	CHECK(f_value == Approx(-7.39));
	CHECK(f_value_str == "-7.39000");
	CHECK(f_counter_1 == 2);
	CHECK(f_counter_2 == 2);

	CHECK(g_value == "bar");
	CHECK(g_counter_1 == 2);

	CHECK(h_counter_1 == 4);
}
