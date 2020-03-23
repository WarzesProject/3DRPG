#pragma once

enum class RenderAPI
{
	OpenGL
};

class RenderConfiguration
{
	RenderAPI api = RenderAPI::OpenGL;
};