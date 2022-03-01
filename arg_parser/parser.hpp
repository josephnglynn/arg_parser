#ifndef ARG_PARSER_ARG_PARSER_HPP
#define  ARG_PARSER_ARG_PARSER_HPP
#include <string>
#include <functional>
#include <iostream>
#include <sstream>

namespace arg_parser
{

	namespace internal
	{
		class base_arg
		{
		public:
			explicit base_arg(std::string name) : m_name(std::move(name))
			{
			}
			virtual ~base_arg() = default;

			[[nodiscard]] inline const std::string& _get_name() const
			{
				return m_name;
			}

			[[nodiscard]] inline const std::string& _get_short_name() const
			{
				return m_short_name;
			}

			[[nodiscard]] inline const std::string& _get_long_name() const
			{
				return m_long_name;
			}

			[[nodiscard]] inline const std::string& _get_description() const
			{
				return m_description;
			}

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

			virtual void set_value(char*) const
			{
				std::cerr << "ERROR: set_value(void* value) not implemented" << std::endl;
				std::exit(-1);
			};
		};
	}

	template<typename T>
	class argument : public internal::base_arg
	{
	public:
		explicit argument(std::string name) : internal::base_arg(std::move(name))
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
		void set_value(char* value) const override
		{
			std::istringstream stream(value);
			stream >> *m_value;
		}

		T* m_value;
	};

	class parser
	{
	public:
		explicit parser(std::string name);

		parser& with_exec(const std::string& exec);
		parser& with_version(const std::string& version);
		parser& with_author(const std::string& author);
		parser& with_about(const std::string& about);

		template<typename T>
		parser& with_arg(const argument<T>& arg)
		{
			m_args.push_back(new argument<T>(arg));
			return *this;
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
}

#endif