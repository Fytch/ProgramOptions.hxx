#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
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
}
