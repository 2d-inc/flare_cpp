project "flare_skia"
	kind "StaticLib"
	language "C++"

	configuration { "Debug" }
		targetdir "./debug/lib"
		objdir "./debug/obj"

	configuration { "Release" }
		targetdir "./release/lib"
		objdir "./release/obj"

	configuration { }

	includedirs 
	{
		"../include",
		"../../flare_math/include",
		"../../flare/include",
		"../../dependencies/skia/include/core",
		"../../dependencies/skia/include/effects",
		"../../dependencies/skia/include/gpu",
		"../../dependencies/skia/include/config"
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

	targetname "flare_skia"