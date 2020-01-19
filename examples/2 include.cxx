#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
	po::parser parser;
	parser["optimization"]	// corresponds to --optimization
		.abbreviation('O')	// corresponds to -O
		.type(po::u32)		// expects an unsigned 32-bit integer
		.fallback(0);		// if --optimization is not explicitly specified, assume 0

	parser["include-path"]	// corresponds to --include-path
		.abbreviation('I')	// corresponds to -I
		.type(po::string)	// expects a string
		.multi();			// allows multiple arguments for the same option

	parser(argc, argv);		// parses the command line arguments

	auto&& O = parser["optimization"];
	// .was_set() reports whether the option was specified by the user or relied on the predefined fallback value.
	std::cout << "optimization level (" << (O.was_set() ? "manual" : "auto") << ") = " << O.get().u32 << '\n';

	auto&& I = parser["include-path"];
	// .size() and .count() return the number of given arguments. Without .multi(), their return value is always <= 1.
	std::cout << "include paths (" << I.size() << "):\n";
	// Here, the non-template .begin() / .end() methods were used. Their value type is po::value,
	// which is not a value in itself but contains the desired values as members, i.e. i.string.
	for(auto&& i : I)
		std::cout << '\t' << i.string << '\n';
}
