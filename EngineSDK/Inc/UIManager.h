#pragma once
#include "Base.h"

BEGIN(Engine)
class CUIManager :
    public CBase
{
private:
    CUIManager();
    virtual ~CUIManager() = default;

public:
    HRESULT Initialize();
    HRESULT Add_RenderObejct(class CGameObject* pRenderObject);
    HRESULT Render();


private:
    list<class CGameObject*> m_Uis;
    list<class CGameObject*> m_RenderUIs;

public:
    static CUIManager* Create();
    virtual void Free();
};

END