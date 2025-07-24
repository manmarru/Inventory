#pragma once
#include "Base.h"

BEGIN(Engine)
class CUIHandler :
    public CBase
{
private:
    CUIHandler();
    virtual ~CUIHandler() = default;

public:
    HRESULT Initialize();

public:
    CUIHandler* Create();
    virtual void Free();
};

END