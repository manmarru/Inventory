#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)
class CPlayer : public CBase
{
private:
    CPlayer();
    virtual ~CPlayer() = default;


public:
    virtual void Free() override;
};
END