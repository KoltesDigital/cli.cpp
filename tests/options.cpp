#include <catch/catch.hpp>

#include <cli.hpp>

#define SETUP \
	int argc = sizeof(argv) / sizeof(char *);                    \
											                     \
	std::ostringstream stream;									 \
	cli::Parser parser(argc, argv, stream, stream);              \
											                     \
	auto force = parser.flag("force")		                     \
		.alias("f")							                     \
		.description("Force operation")		                     \
		.getValue();						                     \
											                     \
	auto input = parser.option("input")		                     \
		.alias("i")							                     \
		.description("Input file")			                     \
		.required()							                     \
		.getValue();						                     \
											                     \
	auto output = parser.option("output")	                     \
		.alias("o")							                     \
		.defaultValue(input)						             \
		.description("Output file")			                     \
		.getValue();						                     \
											                     \
	parser.getRemainingArguments(argc, argv)

TEST_CASE("No argument")
{
	const char *argv[] =
	{
		"$0",
	};

	SETUP;

	CHECK(parser.hasErrors());
	CHECK(!force);
	CHECK(!input);
	CHECK(!output);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Flag is parsed")
{
	const char *argv[] =
	{
		"$0",
		"-force",
	};

	SETUP;

	CHECK(parser.hasErrors());
	CHECK(force);
	CHECK(!input);
	CHECK(!output);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}


TEST_CASE("Flag alias is parsed")
{
	const char *argv[] =
	{
		"$0",
		"-f",
	};

	SETUP;

	CHECK(parser.hasErrors());
	CHECK(force);
	CHECK(!input);
	CHECK(!output);
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Option is parsed")
{
	const char *argv[] =
	{
		"$0",
		"-input",
		"in.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in.txt"));
	CHECK(!strcmp(output, input));
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Option alias is parsed")
{
	const char *argv[] =
	{
		"$0",
		"-i",
		"in.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in.txt"));
	CHECK(!strcmp(output, input));
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Multiple options are parsed")
{
	const char *argv[] =
	{
		"$0",
		"-i",
		"in.txt",
		"-o",
		"out.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in.txt"));
	CHECK(!strcmp(output, "out.txt"));
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Option is not parsed twice")
{
	const char *argv[] =
	{
		"$0",
		"-i",
		"in1.txt",
		"-i",
		"in2.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in1.txt"));
	CHECK(!strcmp(output, input));
	CHECK(argc == 3);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "-i"));
	CHECK(!strcmp(argv[2], "in2.txt"));
}

TEST_CASE("Option is not consumed twice")
{
	const char *argv[] =
	{
		"$0",
		"-f",
		"-i",
		"in.txt",
		"foobar",
	};

	int argc = sizeof(argv) / sizeof(char *);

	std::ostringstream stream;
	cli::Parser parser(argc, argv, stream, stream);

	auto force = parser.flag("force")
		.alias("f")
		.description("Force operation");

	auto input = parser.option("input")
		.alias("i")
		.description("Input file")
		.required();

	parser.getRemainingArguments(argc, argv);
	CHECK(argc == 5);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "-f"));
	CHECK(!strcmp(argv[2], "-i"));
	CHECK(!strcmp(argv[3], "in.txt"));
	CHECK(!strcmp(argv[4], "foobar"));

	CHECK(!parser.hasErrors());
	CHECK(force.getValue());
	CHECK(force.getValue());
	CHECK(!strcmp(input.getValue(), "in.txt"));
	CHECK(!strcmp(input.getValue(), "in.txt"));

	parser.getRemainingArguments(argc, argv);
	CHECK(!parser.hasErrors());
	CHECK(argc == 2);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "foobar"));
}

TEST_CASE("Option order is not relevant")
{
	const char *argv[] =
	{
		"$0",
		"-o",
		"out.txt",
		"-i",
		"in.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in.txt"));
	CHECK(!strcmp(output, "out.txt"));
	CHECK(argc == 1);
	CHECK(!strcmp(argv[0], "$0"));
}

TEST_CASE("Options are not parsed as remaining arguments")
{
	const char *argv[] =
	{
		"$0",
		"list",
		"-i",
		"in.txt",
		"--",
		"-o",
		"out.txt",
	};

	SETUP;

	CHECK(!parser.hasErrors());
	CHECK(!force);
	CHECK(!strcmp(input, "in.txt"));
	CHECK(!strcmp(output, input));
	CHECK(argc == 4);
	CHECK(!strcmp(argv[0], "$0"));
	CHECK(!strcmp(argv[1], "list"));
	CHECK(!strcmp(argv[2], "-o"));
	CHECK(!strcmp(argv[3], "out.txt"));
}

TEST_CASE("Options are converted to other types")
{
	const char *argv[] =
	{
		"$0",
		"-n",
		"1337",
	};


	int argc = sizeof(argv) / sizeof(char *);

	std::ostringstream stream;
	cli::Parser parser(argc, argv, stream, stream);

	int n = parser.option("n")
		.defaultValue("42")
		.getValueAs<int>();

	CHECK(!parser.hasErrors());
	CHECK(n == 1337);
}

TEST_CASE("Option default values are converted to other types")
{
	const char *argv[] =
	{
		"$0",
	};


	int argc = sizeof(argv) / sizeof(char *);

	std::ostringstream stream;
	cli::Parser parser(argc, argv, stream, stream);

	int n = parser.option("n")
		.defaultValue("42")
		.getValueAs<int>();

	CHECK(!parser.hasErrors());
	CHECK(n == 42);
}
