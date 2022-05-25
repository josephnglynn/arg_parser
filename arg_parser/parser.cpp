//
// Created by joseph on 01/03/2022.
//

#include "parser.hpp"
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
	} // namespace internal

	parser::parser(std::string name)
		: m_name(std::move(name))
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
			for (const auto& arg : m_args)
			{
				if (arg->matches(i, argv)) break;
			}
		}
	}

	void parser::handle_version()
	{
		std::cout << m_exec << " " << m_version << std::endl;
		std::exit(0);
	}

	void parser::handle_help()
	{
		std::cout << m_name << " " << m_version << "\n"
				  << m_author << "\n"
				  << m_about << "\n\nUsage:\n"
				  << ARG_PARSER_SPACE << m_exec << " [OPTIONS]"
				  << "\n\n"
				  << "Flags:\n"
				  << ARG_PARSER_SPACE << "-h, --help   " << ARG_PARSER_SPACE << "Prints help information\n"
				  << ARG_PARSER_SPACE << "-v, --version" << ARG_PARSER_SPACE << "Prints version information\n\n"
				  << "Options:\n";

		long length = 0;
		std::vector<internal::output_info> output = std::vector<internal::output_info>(m_args.size());
		for (size_t i = 0; i < m_args.size(); ++i)
		{
			output[i].stream << ARG_PARSER_SPACE;

			if (!internal::get_short_name(*m_args[i]).empty())
			{
				output[i].stream << internal::get_short_name(*m_args[i]);
				if (!internal::get_long_name(*m_args[i]).empty()) output[i].stream << ", ";
			}
			if (!internal::get_long_name(*m_args[i]).empty()) output[i].stream << internal::get_long_name(*m_args[i]) << " ";
			output[i].stream << "<" << internal::get_name(*m_args[i]) << ">";

			output[i].size = output[i].stream.tellp();
			if (output[i].size > length) length = output[i].size;
		}

		for (size_t i = 0; i < m_args.size(); ++i)
		{
			if (output[i].size != length) output[i].stream << std::string(length - output[i].size, ' ');
			std::cout << output[i].stream.rdbuf() << ARG_PARSER_SPACE << internal::get_description(*m_args[i]) << std::endl;
		}

		std::exit(0);
	}

	parser& parser::check_for_errors()
	{
		std::cout << "Checking parser for errors...\n";

		for (size_t i = 0; i < m_args.size(); ++i)
		{
			const auto& name = internal::get_name(*m_args[i]);
			const auto& short_name = internal::get_short_name(*m_args[i]);
			const auto& long_name = internal::get_long_name(*m_args[i]);
			const auto& description = internal::get_description(*m_args[i]);
			auto value_ptr = internal::get_value_ptr(*m_args[i]);

			std::cout << "\n\nChecking argument " << i;
			std::cout << "\nname: " << name;
			std::cout << "\nvalue_ptr: " << value_ptr;
			std::cout << "\nlong_name: " << long_name;
			std::cout << "\nshort_name: " << short_name;
			std::cout << "\ndescription: " << description;

			int clean[5] = {name.empty(), short_name.empty(), long_name.empty(), description.empty(), value_ptr == nullptr};
			if (!(clean[0] || clean[1] || clean[2] || clean[3] || clean[4])) continue;
			if (clean[0]) std::cout << "\nNote: name should not be empty, how will people know what the flag does";
			if (clean[1] || clean[2]) std::cout << "\nNote: either short_name or long_name should have a value so that this flag is usable";
			if (clean[3]) std::cout << "\nNote: description should not be empty, unless self explanatory";
			if (clean[4])
			{
				std::cout << "\nError: value_ptr should not be a nullptr, this will throw and error later. Therefore I will exit now so you can fix it! - ( hint: you probably forgot to use .with_value(T&) )" << std::endl;
				std::exit(-1);
			}
		}

		std::cout << std::endl; // Flush buffer just in case

		return *this;
	}

} // namespace arg_parser