#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
	po::parser parser;
	parser[ "x" ];				// creates an option with name 'x'

	parser( argc, argv );		// parses the command line arguments

	if( parser[ "x" ].available() )
		std::cout << "flag x set!\n";
}
