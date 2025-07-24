#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CIngameBG final : public CUIObject
{
private:
    CIngameBG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CIngameBG(const CIngameBG& Prototype);
    virtual ~CIngameBG() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CShader*			m_pShaderCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CIngameBG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END