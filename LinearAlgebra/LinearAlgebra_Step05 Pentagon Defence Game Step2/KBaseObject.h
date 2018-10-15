#pragma once
#include <string>
#include <iostream>

class KBaseObject
{
private:
	std::string		m_name; // name of game object
public:
	KBaseObject();
	virtual ~KBaseObject();
	std::string GetName();
	void SetName(const std::string& name_);

	virtual void OnUpdate( float fElapsedTime_ ) {}
	virtual void OnRender() {}
};

