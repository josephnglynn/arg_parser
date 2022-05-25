#ifndef ARG_PARSER_ARG_PARSER_HPP
#define ARG_PARSER_ARG_PARSER_HPP
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

namespace arg_parser
{

	namespace internal
	{
		class base_arg;
		const std::string& get_name(const base_arg& arg);
		const std::string& get_short_name(const base_arg& arg);
		const std::string& get_long_name(const base_arg& arg);
		const std::string& get_description(const base_arg& arg);
		const void* get_value_ptr(const base_arg& arg);

		class base_arg
		{
		public:
			explicit base_arg(std::string name)
				: m_name(std::move(name))
			{
			}

			virtual ~base_arg() = default;

			[[nodiscard]] inline friend const std::string& get_name(const base_arg& arg) { return arg.m_name; }
			[[nodiscard]] inline friend const std::string& get_short_name(const base_arg& arg) { return arg.m_short_name; }
			[[nodiscard]] inline friend const std::string& get_long_name(const base_arg& arg) { return arg.m_long_name; }
			[[nodiscard]] inline friend const void* get_value_ptr(const base_arg& arg) { return arg.get_arg_value_ptr(); }
			[[nodiscard]] inline friend const std::string& get_description(const base_arg& arg) { return arg.m_description; }

			bool matches(int& index, char* argv[]) const
			{
				if (m_short_name == argv[index] || m_long_name == argv[index])
				{
					set_value(argv[++index]);
					return true;
				}
				return false;
			}

		protected:
			const std::string m_name;
			std::string m_short_name;
			std::string m_long_name;
			std::string m_description;

			[[nodiscard]] virtual const void* get_arg_value_ptr() const
			{
				std::cerr << "ERROR: is_value_null() not implemented" << std::endl;
				std::exit(-1);
			}

			virtual void set_value(const char*) const
			{
				std::cerr << "ERROR: set_value(void* value) not implemented" << std::endl;
				std::exit(-1);
			};
		};
	} // namespace internal

	template <typename T>
	class argument : public internal::base_arg
	{
	public:
		explicit argument(std::string name)
			: internal::base_arg(std::move(name)), m_value(nullptr)
		{
		}

		argument<T>& with_short(const std::string& name)
		{
			m_short_name = name;
			return *this;
		}

		argument<T>& with_long(const std::string& name)
		{
			m_long_name = name;
			return *this;
		}

		argument<T>& with_description(const std::string& description)
		{
			m_description = description;
			return *this;
		}

		argument<T>& with_value(T& value)
		{
			m_value = &value;
			return *this;
		}

	private:
		[[nodiscard]] const void* get_arg_value_ptr() const override { return m_value; }

		void set_value(const char* value) const override
		{
			std::istringstream stream(value);
			stream >> *m_value;
		}

		T* m_value;
	};

	template <>
	void argument<std::string>::set_value(const char* value) const
	{
		*m_value = value;
	}

	class parser
	{
	public:
		explicit parser(std::string name);

		parser& with_exec(const std::string& exec);
		parser& with_version(const std::string& version);
		parser& with_author(const std::string& author);
		parser& with_about(const std::string& about);

		template <typename T>
		parser& with_arg(const argument<T>& arg)
		{
			m_args.push_back(new argument<T>(arg));
			return *this;
		}

		/*
		 * THROWABLE
		 */
		parser& check_for_errors();

		inline parser& check_for_errors_debug_only()
		{
#ifdef DEBUG
			return check_for_errors();
#else
			return *this;
#endif
		}

		void parse(int argc, char* argv[]);

	private:
		void handle_version();
		void handle_help();

		const std::string m_name;
		std::string m_exec;
		std::string m_version;
		std::string m_author;
		std::string m_about;

		std::vector<internal::base_arg*> m_args;
	};
} // namespace arg_parser

#endif