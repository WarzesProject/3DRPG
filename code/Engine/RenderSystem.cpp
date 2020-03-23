#include "stdafx.h"
#include "RenderSystem.h"
//-----------------------------------------------------------------------------
RenderSystem::RenderSystem(RenderConfiguration &config)
	: m_config(config)
{
	setValid(true);
}
//-----------------------------------------------------------------------------