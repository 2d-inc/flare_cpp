solution "example"

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

project "example"
	kind "ConsoleApp"
	language "C++"

	configuration { "Debug" }
		targetdir "./debug/bin"
		objdir "./debug/bin/obj"

	configuration { "Release" }
		targetdir "./release/bin"
		objdir "./release/bin/obj"

	configuration "macosx"
		links { "Cocoa.framework", "IOKit.framework" }
		includedirs { "/usr/local/include" }
		libdirs { "/usr/local/lib" }

	configuration { }

	-- includedirs 
	-- {
	-- 	"../include",
	-- 	"../Nima-Cpp/include",
	-- 	"../Nima-Cpp/Nima-Math-Cpp/include"
	-- }

	buildoptions_cpp 
	{
		"-std=c++11",
		"-fembed-bitcode"
	}
   
	files
	{
		"../example/**.cpp"	
	}

	targetname "example"