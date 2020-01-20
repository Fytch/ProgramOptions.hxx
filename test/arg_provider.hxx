#ifndef arg_provider_hxx_included
#define arg_provider_hxx_included

#define PROGRAMOPTIONS_EXCEPTIONS
#include <ProgramOptions.hxx>
#include <vector>
#include <iterator>

class arg_provider {
	std::vector<std::vector<char>> m_data;
	std::vector<char*> m_args;

public:
	const int argc = static_cast<int>(m_args.size());
	char** const argv = m_args.data();

	template<std::size_t... indices, typename... args_t>
	explicit arg_provider(po::integer_sequence<std::size_t, indices...>, args_t&&... args)
		: m_data{ std::vector<char>(std::begin(args), std::end(args))... }
		, m_args{ m_data[indices].data()... } {
	}
	template<typename... args_t>
	explicit arg_provider(args_t&&... args)
		: arg_provider{ po::make_index_sequence<sizeof...(args_t)>{}, std::forward<args_t>(args)... } {
	}
};

#endif // !arg_provider_hxx_included
