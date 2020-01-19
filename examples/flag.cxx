#include <ProgramOptions.hxx>
#include <iostream>

int main(int argc, char** argv) {
	po::parser parser;
	auto&& x = parser["x"];	// creates an option with name 'x'

	parser(argc, argv);		// parses the command line arguments

	if(x.available())
		std::cout << "flag x set!\n";
}
