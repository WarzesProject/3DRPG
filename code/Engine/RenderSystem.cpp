#include "stdafx.h"
#include "RenderSystem.h"
//-----------------------------------------------------------------------------
RenderSystem::RenderSystem(RenderConfiguration &config)
	: m_config(config)
{
	setValid(true);
}
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
}
//-----------------------------------------------------------------------------
void RenderSystem::BeginFrame()
{
}
//-----------------------------------------------------------------------------
void RenderSystem::EndFrame()
{
}
//-----------------------------------------------------------------------------