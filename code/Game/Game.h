#pragma once

class Game
{
public:
	static int Main();

	bool Init();
	bool Update();
	bool Frame();
	void Close();
};