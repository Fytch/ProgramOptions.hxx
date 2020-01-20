#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
	po::parser parser;
	auto& O = parser["optimization"]	// corresponds to --optimization
		.abbreviation('O')				// corresponds to -O
		.description("set the optimization level (default: -O0)")
										// the description to be shown when printing the parser
		.type(po::u32)					// expects an unsigned 32-bit integer
		.fallback(0);					// if --optimization is not explicitly specified, assume 0

	auto& I = parser["include-path"]	// corresponds to --include-path
		.abbreviation('I')				// corresponds to -I
		.description("add an include path")
										// the description to be shown when printing the parser
		.type(po::string)				// expects a string
		.multi();						// allows multiple arguments for the same option

	auto& help = parser["help"]			// corresponds to --help
		.abbreviation('?')				// corresponds to -?
		.description("print this help screen")
										// the description to be shown when printing the parser
		// .type(po::void_)				// redundant; default for named parameters
		// .single()					// redundant; default for named parameters
		.callback([&]{ std::cout << parser << '\n'; });
										// callbacks get invoked when the option occurs

	auto& files = parser[""]			// the unnamed parameter is used for non-option arguments as in: gcc a.c b.c
		// .type(po::string)			// redundant; default for the unnamed parameter
		// .multi()						// redundant; default for the unnamed parameter
		.callback([&](std::string const& x){ std::cout << "processed \'" << x << "\' successfully!\n"; });
										// as .get_type() == po::string, the callback may take an std::string

	// parsing returns false if at least one error has occurred
	if(!parser(argc, argv)) {
		std::cerr << "errors occurred; aborting\n";
		return -1;
	}
	// we don't want to print anything else if the help screen has been displayed
	if(help.was_set())
		return 0;

	std::cout << "processed files: " << files.size() << '\n';

	// .was_set() reports whether the option was specified by the user or relied on the predefined fallback value.
	std::cout << "optimization level (" << (O.was_set() ? "manual" : "auto") << ") = " << O.get().u32 << '\n';

	// .size() and .count() return the number of given arguments. Without .multi(), their return value is always <= 1.
	std::cout << "include paths (" << I.size() << "):\n";

	// Here, the non-template .begin() / .end() methods were used. Their value type is
	// po::value, which is not a value in itself but contains the desired values as members, i.e. i.string.
	for(auto&& i : I)
		std::cout << '\t' << i.string << '\n';
}
