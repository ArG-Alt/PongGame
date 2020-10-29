#include <iostream>

#include "Game.hpp"


int main()
{
	Game pong;

	if (pong.initialize())
	{
		pong.runLoop();
	}

	pong.shutDown();

	return 0;
}