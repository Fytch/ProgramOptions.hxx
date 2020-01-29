#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
try {
	po::parser parser;

	auto& PLACEHOLDER = parser["PLACEHOLDER"]
		.abbreviation('PLACEHOLDER')
		.description("PLACEHOLDER")
		.type(po::PLACEHOLDER)
		.multi()
		.fallback(PLACEHOLDER)
		.callback([&]{ PLACEHOLDER; });

	std::uint32_t PLACEHOLDER;
	auto& PLACEHOLDER = parser["PLACEHOLDER"]
		.abbreviation('PLACEHOLDER')
		.description("PLACEHOLDER")
		.bind(PLACEHOLDER);

	auto& help = parser["help"]
		.abbreviation('?')
		.description("print this help screen");

	if(!parser(argc, argv)) {
		// error
		return 1;
	}

	if(help.was_set()) {
		std::cout << parser << '\n';
		return 0;
	}

	if(PLACEHOLDER.available()) {
		// do something
	}
} catch(std::exception const& e) {
	std::cerr << "uncaught exception: " << e.what() << '\n';
	std::cerr << "terminating application\n";
	return -1;
}
