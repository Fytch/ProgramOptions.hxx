#define PROGRAMOPTIONS_NO_EXCEPTIONS
#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
	po::parser parser;
	parser["help"]
		.abbreviation('?')
		.description("print this help screen")
		.callback([&]{ std::cout << parser << '\n'; });

	parser["PLACEHOLDER"]
		.abbreviation('PLACEHOLDER')
		.description("PLACEHOLDER")
		.type(po::PLACEHOLDER)
		.multi()
		.fallback(PLACEHOLDER)
		.callback([&]{ PLACEHOLDER; });

	if(!parser(argc, argv)) {
		// error
		return 1;
	}

	if(parser["PLACEHOLDER"].available()) {
		// do something
	}
}
