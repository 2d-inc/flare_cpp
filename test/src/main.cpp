#include "flare/actor.hpp"
#include "flare/exceptions/overflow_exception.hpp"
#include "flare/exceptions/unsupported_version_exception.hpp"
#include <stdio.h>

int main()
{
   flare::Actor* actor = new flare::Actor();

	try
	{
		actor->load("assets/Filip.flr");
	}
	catch (flare::OverflowException ex)
	{
		printf("Bad data, got an overflow. %s\n", ex.message().c_str());
	}
	catch (flare::UnsupportedVersionException ex)
	{
		printf("Unsupported version. %d %d\n", ex.versionFound(), ex.versionRequired());
	}
	printf("Loaded ok.\n");
   return 0;
}