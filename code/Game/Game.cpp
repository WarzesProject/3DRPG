#include "stdafx.h"
#include "Game.h"
//-----------------------------------------------------------------------------
int Game::Main()
{
	Configuration config;

	return Application::Run<Game>(config);
}
//-----------------------------------------------------------------------------
Game::~Game()
{	
}
//-----------------------------------------------------------------------------
bool Game::Init()
{
	return true;
}
//-----------------------------------------------------------------------------
bool Game::Update(float dt)
{
	return true;
}
//-----------------------------------------------------------------------------
bool Game::Frame()
{
	return true;
}
//-----------------------------------------------------------------------------