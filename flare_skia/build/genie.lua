solution "flare_skia"

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

dofile("flare_skia.lua")