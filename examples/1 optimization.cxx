#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
	po::parser parser;
	auto& O = parser["optimization"]	// corresponds to --optimization
		.abbreviation('O')				// corresponds to -O
		.type(po::u32);					// expects an unsigned 32-bit integer

	parser(argc, argv);					// parses the command line arguments

	if(!O.available())
		std::cout << "no optimization level set!\n";
	else
		std::cout << "optimization level set to " << O.get().u32 << '\n';
}
