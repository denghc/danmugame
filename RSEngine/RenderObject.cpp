#include "RenderObject.h"


RenderObject::RenderObject(void)
{
}


RenderObject::~RenderObject(void)
{
}

void RenderObject::SetVisible(bool v)
{
	m_visible = v;
}

bool RenderObject::IsVisible()
{
	return m_visible;
}

bool RenderObject::Is2D()
{
	return m_isOrtho;
}