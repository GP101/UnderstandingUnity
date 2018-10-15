#include "KBaseObject.h"

KBaseObject::KBaseObject()
{
}

KBaseObject::~KBaseObject()
{
}

std::string KBaseObject::GetName()
{
	return m_name;
}

void KBaseObject::SetName(const std::string& name_)
{
	m_name = name_;
}
