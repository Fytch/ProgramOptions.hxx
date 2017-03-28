// Demonstrative reimplementation of the example on getopt's website:
// https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
	po::parser parser;

	auto&& a = parser[ "a" ];
	auto&& b = parser[ "b" ];
	auto&& c = parser[ "c" ]
		.type( po::string )
		.fallback( "(null)" );
	auto&& unnamed = parser[ "" ]
		.type( po::string )
		.multi();

	parser( argc, argv );

	std::cout << "aflag = " << a.was_set() << ", ";
	std::cout << "bflag = " << b.was_set() << ", ";
	std::cout << "cflag = " << c.get().string << '\n';
	for( auto&& i : unnamed )
		std::cout << "Non-option argument " << i.string << '\n';
}
