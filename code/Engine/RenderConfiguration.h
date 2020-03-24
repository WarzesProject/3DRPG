#pragma once

enum class RenderAPI
{
	OpenGL
};

struct RenderConfiguration
{
	RenderAPI api = RenderAPI::OpenGL;
};