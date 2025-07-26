#pragma once
#include "Client_Defines.h"
#include "UIObject.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBUFFER_RECT;
END

BEGIN(Client)
class CItemIcon :
    public CUIObject
{
	typedef struct ITEMICON_DESC : public CUIObject::UI_DESC
	{
		_float fOffsetX{}, fOffsetY{};
	};
private:
    CItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext pContext);
    CItemIcon(const CItemIcon& Prototype);
    ~CItemIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	static _float s_fPivotX;
	static _float s_fPivotY;

private:
	_float m_fOffsetX;
	_float m_fOffsetY;

private:
	HRESULT Ready_Components();

public:
	static CItemIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END