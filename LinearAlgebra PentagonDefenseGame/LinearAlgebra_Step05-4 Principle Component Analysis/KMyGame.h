#pragma once
#include "KBaseObject.h"
#include <memory>
#include <list>
#include <vector>

class KMyGame : public std::enable_shared_from_this<KMyGame>
{
private:
    static std::shared_ptr<KMyGame>     instance;
public:
	template<typename T>
	static std::shared_ptr<T>			CreateInstance()
	{
		if (instance == nullptr)
			instance.reset(new T());
		return std::static_pointer_cast<T>( instance );
	}
    static std::shared_ptr<KMyGame>     GetInstance()
    {
        return instance;
    }

private:
    std::vector<KBaseObjectPtr>
                        m_objectManager;

private:
    void                _AddGameObject(KBaseObjectPtr sp);

public:
    CONSTRUCTOR         KMyGame();
    DESTRUCTOR          ~KMyGame();

    template<typename T>
    std::shared_ptr<T>  CreateObject() // Factory Method
                        {
                            std::shared_ptr<T> pb;
                            pb.reset(new T());
                            _AddGameObject(pb);
                            return pb;
                        }

    int                 Run(HINSTANCE hInstance, int nCmdShow);

    void                Initialize();
    void                Finalize();
    void                Update(float fElaspedTime_);
    void                Render();
	void                LButtonDown(int x, int y);
    void                ProcessCommand(int commandId);

    virtual void        OnInitialize(){}
    virtual void        OnFinalize(){}
    virtual void        OnLButtonDown(int x, int y){}
    virtual void        OnUpdate(float fElapsedTime) {}
    virtual void        OnRender(HDC hdc) {}
    virtual void        OnCommand(int commandId) {}
};
