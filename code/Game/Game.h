#pragma once

class Game
{
public:
	static int Main();

	~Game();

	bool Init();
	bool Update(float dt);
	bool Frame();
};