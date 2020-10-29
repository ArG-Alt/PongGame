#include "Game.hpp"

Game::Game()
	: m_Window{ nullptr }, m_Renderer{ nullptr }, m_IsRunning{ true }
		, m_BallPos{ 1024/2.0, 768/2.0 }, m_BatPos{ 0.0, 768/2.0 }
	    , m_TicksCount{ 0 }, m_BatDir{ 0 }, m_BallVelocity{ -200.0f, 235.0f }
{

}

bool Game::initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}

	m_Window = SDL_CreateWindow(
		"Pong Game",
		100,
		100,
		1024,
		768,
		0
	);

	
	if (m_Window == nullptr)
	{
		SDL_Log("Unable to create a window: %s", SDL_GetError());
		return false;
	}

	m_Renderer = SDL_CreateRenderer(
		m_Window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (m_Renderer == nullptr)
	{
		SDL_Log("Unable to create a renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::shutDown()
{	
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Game::runLoop()
{
	while (m_IsRunning)
	{
		processInput();
		updateGame();
		generateOutput();
	}
}

void Game::processInput()
{
	SDL_Event event;
	
	// Handling all window related Events
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:			// Close button
			m_IsRunning = false;
			break;
		}
	}

	// Handling KeyPresses
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		m_IsRunning = false;
	}
	else if (state[SDL_SCANCODE_W])
	{
		m_BatDir = -1;
	}
	else if (state[SDL_SCANCODE_S])
	{
		m_BatDir = 1;
	}

}

void Game::updateGame()
{
	// Frame Limiting
	// Wait until 16ms has passed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), (m_TicksCount + 16)));

	float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

	m_TicksCount = SDL_GetTicks();

	// Update the game object as a function of delta time
	if (m_BatDir != 0)
	{
		m_BatPos.y += m_BatDir * 300.0f * deltaTime;

		if (m_BatPos.y < 45.0)
		{	
			m_BatPos.y = 45.0;
		}
		else if (m_BatPos.y > (768 - 15 - 30))
		{
			m_BatPos.y = 768 - 15 - 30;
		}
	}

	// checking collision with the walls
	if (m_BallPos.y <= 15 && m_BallVelocity.y < 0.0f)		// top wall
	{
		m_BallVelocity.y *= -1;
	}
	else if (m_BallPos.y >= (768 - 15) && m_BallVelocity.y > 0.0f)	// bottom wall
	{
		m_BallVelocity.y *= -1;
	}
	else if (m_BallPos.x >= (1024 - 15) && m_BallVelocity.x > 0.0f)		// Right wall
	{
		m_BallVelocity.x *= -1;
	}

	// checking collision with bat
	float diff = (m_BallPos.y - m_BatPos.y);
	float absDiff = diff > 0 ? diff : -1*diff;

	if (
		absDiff <= 30 &&
		m_BallPos.x <= 25.0f && m_BallPos.x >= 20.0f &&
		m_BallVelocity.x < 0.0f
		)
	{
		m_BallVelocity.x *= -1;
	}

	m_BallPos.x += m_BallVelocity.x * deltaTime;
	m_BallPos.y += m_BallVelocity.y * deltaTime;

}

void Game::generateOutput()
{

	constexpr int wallThickness = 15;

	constexpr int batHeight = 60;
	constexpr int batThickness = 10;
	constexpr int ballDimension = 10;

	SDL_Rect WallTop{
		0,
		0,
		1024,
		wallThickness
	};
	SDL_Rect WallBottom{
		0,
		768 - wallThickness,
		1024,
		wallThickness
	};
	SDL_Rect WallLeft{
		1024 - wallThickness,
		0,
		wallThickness,
		768
	};

	SDL_Rect Bat{
		static_cast<int>(m_BatPos.x + batThickness / 2.0),
		static_cast<int>(m_BatPos.y - batHeight / 2.0),
		batThickness,
		batHeight
	};

	SDL_Rect Ball{
		static_cast<int>(m_BallPos.x - ballDimension / 2.0),
		static_cast<int>(m_BallPos.y - ballDimension / 2.0),
		ballDimension,
		ballDimension
	};


	SDL_SetRenderDrawColor(
		m_Renderer,
		51,
		52,
		86,
		255
	);

	SDL_RenderClear(m_Renderer);

	SDL_SetRenderDrawColor(
		m_Renderer,
		255,
		255,
		255,
		255
	);



	SDL_RenderFillRect(m_Renderer, &WallTop);
	SDL_RenderFillRect(m_Renderer, &WallBottom);
	SDL_RenderFillRect(m_Renderer, &WallLeft);
	SDL_RenderFillRect(m_Renderer, &Bat);
	SDL_RenderFillRect(m_Renderer, &Ball);

	// Swaps the front and back buffer
	SDL_RenderPresent(m_Renderer);
}

// TODO: Add a bat on the right side also, making it multiplayer