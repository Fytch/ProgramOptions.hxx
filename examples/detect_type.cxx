#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
	po::parser parser;
	auto&& o = parser[ "" ];
	parser( argc, argv );

	po::value_type testing_order[] = { po::void_, po::u32, po::u64, po::i32, po::i64, po::f32, po::f64, po::string };
	// iterate over std::strings
	for( auto iter = o.begin< po::string >(); iter != o.end< po::string >(); ++iter ) {
		std::cout << '\'' << *iter << "\' has type ";
		for( auto&& type : testing_order )
			// we try to parse the string with an option of the corresponding type
			if( po::option{}.type( type ).parse( *iter ) == po::error_code::none ) {
				// if the conversion succeeds, print the type
				std::cout << vt2str( type );
				break;
			}
		std::cout << '\n';
	}
}
