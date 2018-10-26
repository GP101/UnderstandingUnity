#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <windows.h>

#ifndef CONSTRUCTOR
#define CONSTRUCTOR
#endif 
#ifndef DESTRUCTOR
#define DESTRUCTOR
#endif
#ifndef VIRTUAL 
#define VIRTUAL 
#endif
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

class KBaseObject;
typedef std::shared_ptr<KBaseObject>    KBaseObjectPtr;

class KBaseObject : public std::enable_shared_from_this<KBaseObject>
{
private:
	std::string		    m_name; // name of game object

public:
	CONSTRUCTOR         KBaseObject();
	DESTRUCTOR virtual  ~KBaseObject();
	std::string			GetName();
	void				SetName(const std::string& name_);

    virtual void		OnUpdate(float fElapsedTime_) {}
    virtual void		OnLateUpdate(float fElapsedTime_) {}
	virtual void		OnRender(HDC hdc) {}
    //void*				operator new(unsigned int size_);
};
