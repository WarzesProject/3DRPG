#pragma once

#include "Engine/RenderConfiguration.h"
#include "Engine/Subsystem.h"

class RenderSystem : public Subsystem<RenderSystem>
{
public:
	RenderSystem(RenderConfiguration &config);
	~RenderSystem();

	void BeginFrame();
	void EndFrame();

private:
	RenderConfiguration &m_config;
};