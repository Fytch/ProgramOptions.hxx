#include <ProgramOptions.hxx>
#include <iostream>
#include <numeric>

int main(int argc, char** argv) {
	po::parser parser;

	auto&& x = parser[""]			// the unnamed parameter
		.type(po::f64)			// expects 64-bit floating point numbers
		.multi()					// allows multiple arguments
		.fallback(-8, "+.5e2")	// if no arguments were provided, assume these as default
		.callback([&]{ std::cout << "successfully parsed "; })
		.callback([&](std::string const& x){ std::cout << x; })
		.callback([&]{ std::cout << " which equals "; })
		.callback([&](po::f64_t x){ std::cout << x << '\n'; });

	parser(argc, argv);

	std::cout << "(+ ";
	for(auto&& i : x.to_vector<po::f64>())	// unnecessary copy; for demonstration purposes only
		std::cout << i << ' ';
	std::cout << ") = " << std::accumulate(x.begin<po::f64>(), x.end<po::f64>(), po::f64_t{}) << '\n';
}
