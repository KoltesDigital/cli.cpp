#include <catch/catch.hpp>

#include <cli.hpp>

TEST_CASE("No errors")
{
	const char *argv[] =
	{
		"$0",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		CHECK(!parser.hasErrors());
	}

	CHECK(stream.str().size() == 0);
}

TEST_CASE("Error when required option is missing")
{
	const char *argv[] =
	{
		"$0",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		auto input = parser.option("input")
			.alias("i")
			.description("Input file")
			.required()
			.getValue();

		CHECK(parser.hasErrors());
	}

	CHECK(stream.str().size() > 0);
}

TEST_CASE("Custom error")
{
	const char *argv[] =
	{
		"$0",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		parser.reportError("%s", "foobar");

		CHECK(parser.hasErrors());
	}

	REQUIRE(stream.str().size() > 0);
	CHECK(stream.str().substr(0, 6) == "foobar");
}

TEST_CASE("Does not display errors twice")
{
	const char *argv[] =
	{
		"$0",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;
	std::ostringstream otherStream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		auto input = parser.option("input")
			.alias("i")
			.description("Input file")
			.required()
			.getValue();

		CHECK(parser.hasErrors());

		parser.showErrors(otherStream);
	}

	CHECK(stream.str().size() == 0);
	CHECK(otherStream.str().size() > 0);
}
