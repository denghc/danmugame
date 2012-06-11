////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include "Structures.h"

ModelClass::ModelClass()
{
}

ModelClass::ModelClass(const ModelClass& other)
{
	m_visible = false;
}


ModelClass::~ModelClass()
{
}


bool ModelClass::IsTextured()
{
	return hasTexture;
}

