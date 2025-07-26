#pragma once

#include "Client_Defines.h"

#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Layer_Camera();	
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_UI();
	
#ifdef _DEBUG
	HRESULT Ready_Test();
	class CLURD_Test* m_pTestObj;
#endif // _DEBUG

private:

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
