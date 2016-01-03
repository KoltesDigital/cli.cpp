#include <catch/catch.hpp>

#include <cli.hpp>

#define SETUP \
	int argc = sizeof(argv) / sizeof(char *);                    \
											                     \
	std::ostringstream stream;									 \
	cli::Parser parser(argc, argv, stream, stream);              \
											                     \
	auto global = parser.flag("global")		                     \
		.alias("g")							                     \
		.description("Global")		                             \
		.getValue();						                     \
											                     \
	bool local = false;											 \
	parser.command("do")	                                     \
		.alias("make")						                     \
		.description("Do something")		                     \
		.execute([&](cli::Parser &parser)	                     \
	{									                    	 \
		local = parser.flag("local")         	     			 \
			.alias("l")											 \
			.description("Local")								 \
			.getValue();										 \
																 \
		parser.getRemainingArguments(argc, argv);				 \
		return EXIT_SUCCESS;									 \
	})

TEST_CASE("No commands executed")
{
	const char *argv[] =
	{
		"$0",
		"-l",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 2);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "-l"));
}

TEST_CASE("No commands given")
{
	const char *argv[] =
	{
		"$0",
		"-l",
	};

	SETUP;

	int result;
	CHECK(!parser.executeCommand(result));
	CHECK(parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 2);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "-l"));
}

TEST_CASE("Command is parsed")
{
	const char *argv[] =
	{
		"$0",
		"do",
	};

	SETUP;

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Command alias is parsed")
{
	const char *argv[] =
	{
		"$0",
		"make",
	};

	SETUP;

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Unknown command")
{
	const char *argv[] =
	{
		"$0",
		"drink",
	};

	SETUP;

	int result;
	CHECK(!parser.executeCommand(result));
	CHECK(parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 2);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "drink"));
}

TEST_CASE("Command is parsed with global options")
{
	const char *argv[] =
	{
		"$0",
		"do",
		"-g",
	};

	SETUP;

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(global);
	CHECK(!local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Command is parsed with local options")
{
	const char *argv[] =
	{
		"$0",
		"do",
		"-l",
	};

	SETUP;

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Command does not need to be the first argument")
{
	const char *argv[] =
	{
		"$0",
		"-l",
		"do",
	};

	SETUP;

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Default command is parsed")
{
	const char *argv[] =
	{
		"$0",
	};

	SETUP;

	bool pause = false;
	parser.defaultCommand()
		.alias("pause")
		.description("Don't do anything")
		.execute([&](cli::Parser &parser)
	{
		pause = true;

		parser.getRemainingArguments(argc, argv);
		return EXIT_SUCCESS;
	});

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(pause);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Default command alias is parsed")
{
	const char *argv[] =
	{
		"$0",
		"pause",
	};

	SETUP;

	bool pause = false;
	parser.defaultCommand()
		.alias("pause")
		.description("Don't do anything")
		.execute([&](cli::Parser &parser)
	{
		pause = true;

		parser.getRemainingArguments(argc, argv);
		return EXIT_SUCCESS;
	});

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == EXIT_SUCCESS);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(pause);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Command result code is retrieved")
{
	const char *argv[] =
	{
		"$0",
	};

	SETUP;

	parser.defaultCommand()
		.alias("pause")
		.description("Don't do anything")
		.execute([&](cli::Parser &parser)
	{
		return 1337;
	});

	int result;
	CHECK(parser.executeCommand(result));
	CHECK(result == 1337);
	CHECK(!parser.hasErrors());
	CHECK(!global);
	CHECK(!local);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}
