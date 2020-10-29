#pragma once
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

struct Vector2
{
	float x;
	float y;
};


class Game
{
private:
	// Window created by SDL
	SDL_Window* m_Window;
	bool m_IsRunning;
	SDL_Renderer* m_Renderer;
	Uint32 m_TicksCount;
	int m_BatDir;
	Vector2 m_BallVelocity;

	// Game Objects
	Vector2 m_BatPos;
	Vector2 m_BallPos;


	// Helper functions
	void processInput();
	void updateGame();
	void generateOutput();

public:
	Game();

	bool initialize();

	void runLoop();

	void shutDown();
};