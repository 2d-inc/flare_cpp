project "flare_math"
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
		"../include"
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

	targetname "flare_math"