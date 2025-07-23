#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CFixedCamera final : public CCamera
{
private:
	CFixedCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFixedCamera(const CFixedCamera& Prototype);
	virtual ~CFixedCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CFixedCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END