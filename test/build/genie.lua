BASE_DIR = path.getabsolute("../..")

solution "test"

	configurations
	{
		"Debug",
		"Release"
	}

configuration "Debug"
	flags
	{
		"Symbols"
	}

location("./");
dofile(path.join(BASE_DIR, "flare_math/build/flare_math.lua"))
location("./");
dofile(path.join(BASE_DIR, "flare/build/flare.lua"))

project "test"
	kind "ConsoleApp"
	language "C++"

	configuration { "Debug" }
		targetdir "./debug/bin"
		objdir "./debug/bin/obj"

	configuration { "Release" }
		targetdir "./release/bin"
		objdir "./release/bin/obj"

	configuration "macosx"
		links { "Cocoa.framework", "IOKit.framework", "flare", "flare_math" }
		includedirs { "/usr/local/include" }
		libdirs { "/usr/local/lib" }

	configuration { }

	includedirs 
	{
		"../../flare/include",
		"../../flare_math/include"
	}

	buildoptions_cpp 
	{
		"-std=c++11",
		"-fembed-bitcode"
	}
   
	files
	{
		"../src/**.cpp"	
	}

	targetname "test"