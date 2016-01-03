#include <iostream>
#include <cli.hpp>

int main(int argc, const char **argv)
{
	cli::Parser parser(argc, argv);

	auto help = parser.defaultHelpFlag();

	parser.help(help)
		<< R"(Usage example
Usage: <command> [options])";

	auto force = parser.flag("force")
		.alias("f")
		.description("Force")
		.getValue();

	auto input = parser.option("input")
		.alias("i")
		.description("Input file")
		.required()
		.getValue();

	parser.command("do")
		.alias("make")
		.description("Do something")
		.execute([&](cli::Parser &parser)
	{
		parser.help(help)
			<< R"(Do something
Usage: do [options])";

		std::cout << "// Command do" << std::endl;

		if (parser.hasErrors())
		{
			std::cout << "// hasError" << std::endl;
			return EXIT_FAILURE;
		}

		std::cout << "// -force: " << (force ? "true" : "false") << std::endl;
		std::cout << "// -input: " << (input ? input : "<missing>") << std::endl;
		parser.getRemainingArguments(argc, argv);
		std::cout << "// Remaining arguments: " << argc << std::endl;
		for (int i = 0; i < argc; ++i)
			std::cout << "// #" << i << ": " << argv[i] << std::endl;
		return EXIT_SUCCESS;
	});

	parser.defaultCommand()
		.alias("pause")
		.description("Don't do anything")
		.execute([&](cli::Parser &parser)
	{
		parser.help(help)
			<< R"(Doesn't do anything
Usage: [pause] [options])";

		auto quiet = parser.flag("quiet")
			.alias("q")
			.description("Quiet")
			.getValue();

		std::cout << "// Default command" << std::endl;

		if (parser.hasErrors())
		{
			std::cout << "// hasError" << std::endl;
			return EXIT_FAILURE;
		}

		std::cout << "// -quiet: " << (quiet ? "true" : "false") << std::endl;
		std::cout << "// -force: " << (force ? "true" : "false") << std::endl;
		std::cout << "// -input: " << (input ? input : "<missing>") << std::endl;
		parser.getRemainingArguments(argc, argv);
		std::cout << "// Remaining arguments: " << argc << std::endl;
		for (int i = 0; i < argc; ++i)
			std::cout << "// #" << i << ": " << argv[i] << std::endl;
		return EXIT_SUCCESS;
	});

	int result;
	bool executed = parser.executeCommand(result);

	std::cout << "// Outside of commands" << std::endl;

	if (parser.hasErrors())
	{
		std::cout << "// hasError" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "// -force: " << (force ? "true" : "false") << std::endl;
	std::cout << "// -input: " << (input ? input : "<missing>") << std::endl;
	std::cout << "// executeCommand: " << (executed ? "true" : "false") << std::endl;
	if (executed)
		std::cout << "// Execution result: " << result << std::endl;
	parser.getRemainingArguments(argc, argv);
	std::cout << "// Remaining arguments: " << argc << std::endl;
	for (int i = 0; i < argc; ++i)
		std::cout << "// #" << i << ": " << argv[i] << std::endl;
}
