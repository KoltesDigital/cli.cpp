#include <catch/catch.hpp>

#include <cli.hpp>

TEST_CASE("UTF8")
{
	auto cat = u8"猫";
	const char *argv[] =
	{
		u8"$0",
		u8"-ばか",
		cat,
	};

	int argc = sizeof(argv) / sizeof(char *);
	std::ostringstream stream;
	cli::Parser parser(argc, argv, stream, stream);

	auto idiot = parser.flag(u8"ばか")
		.description("Is idiot")
		.getValue();

	parser.getRemainingArguments(argc, argv);

	CHECK(!parser.hasErrors());
	CHECK(idiot);
	CHECK(argc == 2);
	CHECK(!strcmp(argv[0], u8"$0"));
	CHECK(!strcmp(argv[1], cat));
}
