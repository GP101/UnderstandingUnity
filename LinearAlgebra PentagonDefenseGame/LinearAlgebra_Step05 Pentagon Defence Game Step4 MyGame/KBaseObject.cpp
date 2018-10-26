#include "KBaseObject.h"
#include "KMyGame.h"

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

//void* KBaseObject::operator new(unsigned int size_)
//{
//    void* pbuffer = ::operator new(size_);
//
//    // MUST NOT USE LIKE BELOW! WHY?
//    //KBaseObjectPtr sp;
//    //sp.reset((KBaseObject*)pbuffer);
//    //KMyGame::GetInstance()->AddGameObject(sp);
//    return pbuffer;
//}
