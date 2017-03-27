#include <iostream>
#include <ProgramOptions.hxx>

int main() {
	std::cout << po::black		<< "black      " << static_cast< int >( po::black		) << '\n';
	std::cout << po::maroon		<< "maroon     " << static_cast< int >( po::maroon		) << '\n';
	std::cout << po::green		<< "green      " << static_cast< int >( po::green		) << '\n';
	std::cout << po::brown		<< "brown      " << static_cast< int >( po::brown		) << '\n';
	std::cout << po::navy		<< "navy       " << static_cast< int >( po::navy		) << '\n';
	std::cout << po::purple		<< "purple     " << static_cast< int >( po::purple		) << '\n';
	std::cout << po::teal		<< "teal       " << static_cast< int >( po::teal		) << '\n';
	std::cout << po::light_gray	<< "light_gray " << static_cast< int >( po::light_gray	) << '\n';
	std::cout << po::dark_gray	<< "dark_gray  " << static_cast< int >( po::dark_gray	) << '\n';
	std::cout << po::red		<< "red        " << static_cast< int >( po::red			) << '\n';
	std::cout << po::lime		<< "lime       " << static_cast< int >( po::lime		) << '\n';
	std::cout << po::yellow		<< "yellow     " << static_cast< int >( po::yellow		) << '\n';
	std::cout << po::blue		<< "blue       " << static_cast< int >( po::blue		) << '\n';
	std::cout << po::fuchsia	<< "fuchsia    " << static_cast< int >( po::fuchsia		) << '\n';
	std::cout << po::cyan		<< "cyan       " << static_cast< int >( po::cyan		) << '\n';
	std::cout << po::white		<< "white      " << static_cast< int >( po::white		) << '\n';
}
