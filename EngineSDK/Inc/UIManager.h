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

public:
    static CUIManager* Create();
    virtual void Free();
};

END