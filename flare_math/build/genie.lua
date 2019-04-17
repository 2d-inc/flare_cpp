solution "flare_math"

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

dofile("flare_math.lua")