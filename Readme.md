# cli.cpp

C++11 header-only command-line argument parser.

* Handles key-value options (`Option`) and boolean key-only options (`Flag`)
* Automatically generates help messages or error reports
* Can handle commands with specific help messages and options
* Encoding-agnostic, e.g. supports UTF8, but you have to use another library to handle the actual strings
* Exhaustively unit tested

## Usage

```cpp
#include <cli.hpp>

int main(int argc, const char **argv)
{
    cli::Parser parser(argc, argv);

    parser.help()
        << R"(Awesome CLI tool
Usage: <command> [options] filenames...)";

    auto output = parser.option("output")
        .alias("o")
        .description("Output file")
        .required()
        .getValue();

    parser.command("parse")
        .alias("p")
        .description("Parse file")
        .execute([&](cli::Parser &parser)
        {
            auto strict = parser.flag("strict")
                .alias("s")
                .description("Enforce strict parsing")
                .getValue();

            parser.getRemainingArguments(argc, argv);
            for (int i = 1; i < argc; ++i)
            {
            	std::ifstream file(argv[i]);
            	if (!file.is_open())
            		continue;
                // ...
            }
        });

    // in case -output is missing
    if (parser.hasErrors())
        return EXIT_FAILURE;

    // in case the command is missing
    if (!parser.executeCommand())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
```

Flags and options are set in the command-line when they are prefixed with at least one dash. The number of dashes is not relevant. Commands are not prefixed with dashes. Parsing also stops when an argument with only dashes is encountered.

```console
program parse file1.txt file2.txt -output output.txt
program p file1.txt file2.txt -o output.txt
```

See the `usage` and `tests` directories for additional examples.

## API

### Command

Allows to execute different functions depending on the leading argument, hence named command. This happens when `Parser::executeCommand` is called.

Because each command is given a new parser instance, you may define specific flags, options, help messages... per command. It is constructed with the original parser's remaining arguments, as well as its error messages. The original parser then looses its error messages.

Your CLI may not define commands: in this case, simply do not call `Parser::executeCommand`.

#### .alias(name) -> itself

Defines a new alias.

#### .description(message) -> itself

Defines the description (default: `nullptr`).

#### .execute(callback)

Defines the function to be executed for this command. This is mandatory.

`callback` has type `void(cli::Parser &)`.

### Flag

Boolean option: it's either here or not.

#### .alias(name) -> itself

Defines a new alias.

#### .description(message) -> itself

Defines the description (default: `nullptr`).

#### .getValue() -> bool

Returns whether the flag is set.

### Option

Expects a value right after the option.

#### .alias(name) -> itself

Defines a new alias.

#### .defaultValue(value) -> itself

Defines the default value (default: `nullptr`).

#### .description(message) -> itself

Defines the description (default: `nullptr`).

#### .getValue() -> const char *

Returns the value after the option if set, or the default value.

Beware of this case:

    std::string arg = parser.option("arg").getValue();

If `-arg` is not set, `getValue()` returns `nullptr`, and `std::string::operator=` throws an exception. Note that this still happens if the option is `required`, because a missing requirement only generates an error message but does not break the function's flow.

#### .required() -> itself

Defines that the option is mandatory.

### Parser

Library's entry point.

Actually `Parser` is a typedef to `BasicParser<std::allocator>`, so you may use another allocator.

#### Parser(argc, argv, helpStream = std::cout, errorStream = std::cerr)

`argc` and `argv` should be directly `main`'s arguments.

`helpStream` and `errorStream` are the default streams on which respectively the help message and errors are written.

#### .command(name) -> Command

Creates a new command with one alias and returns it.

#### .defaultCommand() -> Command

Creates a new command and returns it. This command will be executed when no command is given, however you may still define aliases.

#### .defaultHelpFlag() -> Flag

Creates a new flag and returns it. The flag has the aliases `help`, `h`, `?`.

#### .executeCommand(int &result) -> bool

Considers the first non-parsed argument as a command name and executes the corresponding callback. If a default command is defined and no command argument is given, calls the default command's callback. Returns whether a callback was called. If true, then `result` contains the command's exit code.

Your CLI may not define commands: in this case, simply do not call this function.

#### .flag(name) -> Flag

Creates a new flag with one alias and returns it.

#### .getRemainingArguments(&argc, &argv)

Retrieves the arguments which were not parsed because no corresponding flag/option exists, or because an argument with only dashes has been encountered.

`argv` should be at least as large as the original `argv` given to the constructor, unless you are sure that flags/options have been parsed and therefore remove from these remaining arguments. In practice, you can safely use the same `argv` if you don't mind loosing access to the original arguments.

#### .hasErrors() -> bool

Returns whether parsing errors occurred.

#### .help(bool) -> std::ostringstream

If the argument is `true`, returns the help message's string stream.

You may define more than one help message, in which case you should bind them to different flags.

#### .help(Flag) -> std::ostringstream

Calls `.help(bool)` with the flag's value.

#### .help() -> std::ostringstream

Calls `.help(Flag)` with a default help flag.

#### .option(name) -> Option

Creates a new option with one alias and returns it.

#### .showErrors(stream = constructor's errorStream)

Writes the parsing errors on `stream` and clear them.

If not explicitly called, is called automatically at `Parser`'s destruction if errors happened.

#### .showHelp(stream = constructor's helpStream)

Writes the help messages on `stream` and clear them.

If not explicitly called, is called automatically at `Parser`'s destruction if at least one help flag is set.

## Build usage and tests executables

The library is header-only: you don't need to build anything to use it, just include `cli.hpp` in your projects.

Building this library actually means building the *usage* and *tests* projects. The *usage* project allows you to evaluate the library. The *tests* project contains unit tests: if you change this library, always ensure that they still pass after your changes.

The build system is [Premake 5](https://premake.github.io/).

## License

Copyright (c) 2016 Jonathan Giroux "Bloutiouf"

[MIT License](https://opensource.org/licenses/MIT)
