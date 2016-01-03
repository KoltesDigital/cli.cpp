configurations {
	"Default",
}

flags {
	"FatalWarnings",
	"MultiProcessorCompile",
	"NoPCH",
	"Symbols",
	"Unicode",
}

workspace "cli"
	language "C++"
	location "build"

project "tests"
	files {
		"tests/**",
		"include/**",
	}
	includedirs {
		"include",
		"dep/include",
	}
	location "build"
	kind "ConsoleApp"
	targetdir "bin"

project "usage"
	files {
		"usage/**",
		"include/**",
	}
	includedirs {
		"include",
		"dep/include",
	}
	location "build"
	kind "ConsoleApp"
	targetdir "bin"
