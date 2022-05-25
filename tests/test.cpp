//
// Created by joseph on 01/03/2022.
//

#include <arg_parser/parser.hpp>
#include <iostream>
#include <string>

template <typename... T>
void log(T... t)
{
	((std::cout << t << " "), ...);
	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	int num = 0;
	std::string str;
	arg_parser::parser("Super Cool Test Program")
		.with_exec("test_exec")
		.with_version("v0.0.1")
		.with_author("Joseph Glynn <email@email_example.com>")
		.with_about("A simple executable to test the arg_parser library")
		.with_arg(arg_parser::argument<int>("num")
					  .with_short("-n")
					  .with_long("--number")
					  .with_value(num)
					  .with_description("A simple number argument"))
		.with_arg(arg_parser::argument<std::string>("str")
					  .with_short("-s")
					  .with_long("--string")
					  .with_description("A simple string argument")
					  .with_value(str))
		.check_for_errors_debug_only()
		.parse(argc, argv);

	log("Num is", num);
	log("String is", str);
}