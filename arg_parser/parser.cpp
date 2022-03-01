//
// Created by joseph on 01/03/2022.
//

#include  "parser.hpp"
#include <cstring>
#include <utility>
#define ARG_PARSER_SPACE "    "

namespace arg_parser
{

	namespace internal
	{
		struct output_info
		{
			long size;
			std::stringstream stream;
		};
	}

	parser::parser(std::string name) : m_name(std::move(name))
	{

	}

	parser& parser::with_exec(const std::string& exec)
	{
		m_exec = exec;
		return *this;
	}

	parser& parser::with_version(const std::string& version)
	{
		m_version = version;
		return *this;
	}

	parser& parser::with_author(const std::string& author)
	{
		m_author = author;
		return *this;
	}

	parser& parser::with_about(const std::string& about)
	{
		m_about = about;
		return *this;
	}

	void parser::parse(int argc, char* argv[])
	{
		for (int i = 0; i < argc; ++i)
		{
			if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) handle_help();
			if (std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--version") == 0) handle_version();
			for (const auto& arg : m_args) {
				if (arg->matches(i, argv)) break;
			}
		}
	}

	void parser::handle_version()
	{
		std::cout << m_name << " " << m_version << std::endl;
		std::exit(0);
	}


	void parser::handle_help()
	{
		std::cout << m_name << " " << m_version << "\n"
				  << m_author << "\n"
				  << m_about << "\n\nUsage:\n"
				  << ARG_PARSER_SPACE << m_exec << " [OPTIONS]"<< "\n\n"
				  << "Flags:\n"
				  << ARG_PARSER_SPACE << "-h, --help   " << ARG_PARSER_SPACE << "Prints help information\n"
				  << ARG_PARSER_SPACE << "-v, --version" << ARG_PARSER_SPACE << "Prints version information\n\n"
				  << "Options:\n";

		long length = 0;
		std::vector<internal::output_info> output = std::vector<internal::output_info>(m_args.size());
		for (size_t i = 0; i < m_args.size(); ++i)
		{
			output[i].stream << ARG_PARSER_SPACE;
			if (!m_args[i]->_get_short_name().empty())
			{
				output[i].stream << m_args[i]->_get_short_name();
				if (!m_args[i]->_get_long_name().empty()) output[i].stream << ", ";
			}
			if (!m_args[i]->_get_long_name().empty()) output[i].stream << m_args[i]->_get_long_name() << " ";
			output[i].stream << "<" << m_args[i]->_get_name() << ">";

			output[i].size = output[i].stream.tellp();
			if (output[i].size > length) length = output[i].size;
		}

		for (size_t i = 0; i < m_args.size(); ++i)
		{
			if (output[i].size != length) output[i].stream << std::string(length - output[i].size, ' ');
			std::cout << output[i].stream.rdbuf() << ARG_PARSER_SPACE << m_args[i]->_get_description() << std::endl;
		}

		std::exit(0);
	}

}