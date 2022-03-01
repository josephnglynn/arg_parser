//
// Created by joseph on 01/03/2022.
//

#include <arg_parser/parser.hpp>

int main(int argc, char* argv[])
{
	int num = 0;
	arg_parser::parser("Super Cool Test Program")
		.with_exec("test_exec")
		.with_version("v0.0.1")
		.with_author("Joseph Glynn <email@email_example.com>")
		.with_about("A simple executable to test the arg_parser library")
		.with_arg(arg_parser::argument<int>("num")
			.with_short("-n")
			.with_long("--number")
			.with_value(num)
			.with_description("A simple number test")
		)
		.parse(argc, argv);

	std::cout << num << std::endl;
}