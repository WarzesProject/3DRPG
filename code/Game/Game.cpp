#include "stdafx.h"
#include "Game.h"
//-----------------------------------------------------------------------------
int Game::Main()
{
	Configuration config;

	return Application::Run<Game>(config);
}
//-----------------------------------------------------------------------------
bool Game::Init()
{
	return true;
}
//-----------------------------------------------------------------------------
bool Game::Update()
{
	return true;
}
//-----------------------------------------------------------------------------
bool Game::Frame()
{
	return true;
}
//-----------------------------------------------------------------------------
void Game::Close()
{
}
//-----------------------------------------------------------------------------