#include <catch/catch.hpp>

#include <cli.hpp>

TEST_CASE("No help")
{
	const char *argv[] =
	{
		"$0",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		parser.help()
			<< "Usage: <command> [options]";
	}

	CHECK(stream.str().size() == 0);
}

TEST_CASE("Implicit help")
{
	const char *argv[] =
	{
		"$0",
		"-h",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		parser.help()
			<< "Usage: <command> [options]";
	}

	CHECK(stream.str().size() > 0);
}

TEST_CASE("Explicit help")
{
	const char *argv[] =
	{
		"$0",
		"-aide",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		auto help = parser.flag("aide")
			.description("Affiche l'aide");

		parser.help(help)
			<< "Usage: <command> [options]";
	}

	CHECK(stream.str().size() > 0);
}

TEST_CASE("Explicit help overrides implicit help")
{
	const char *argv[] =
	{
		"$0",
		"-h",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		auto help = parser.flag("aide")
			.description("Affiche l'aide");

		parser.help(help)
			<< "Usage: <command> [options]";
	}

	CHECK(stream.str().size() == 0);
}

TEST_CASE("Multiple help messages")
{
	const char *argv[] =
	{
		"$0",
		"-h2",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		parser.help(parser.flag("h1")) << "1";
		parser.help(parser.flag("h2")) << "2";
	}

	REQUIRE(stream.str().size() > 0);
	CHECK(stream.str().at(0) == '2');
}

TEST_CASE("Does not display help twice")
{
	const char *argv[] =
	{
		"$0",
		"-h",
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;
	std::ostringstream specialStream;

	{
		cli::Parser parser(argc, argv, stream, stream);

		parser.help()
			<< "Usage: <command> [options]";
		
		parser.showHelp(specialStream);
	}

	CHECK(stream.str().size() == 0);
	CHECK(specialStream.str().size() > 0);
}
