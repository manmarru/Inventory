#pragma once

#include "Client_Defines.h"
#include "C:\Users\soboru\Desktop\Inventory\EngineSDK\Inc\UIObject.h"

#define GameTag_ButtonUI TEXT("Prototype_GameObject_ButtonUI")
#define TextureTag_ButtonUI_Sort TEXT("Prototype_Component_Texture_ButtonUI_Sort")

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CButtonUI :
    public CUIObject
{
public:
	typedef struct BUTTONUI_DESC : public CUIObject::UI_DESC
	{
		wstring TextureTag;
		_float fOffsetX{}, fOffsetY{};
	};

private:
	CButtonUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButtonUI(const CButtonUI& Prototype);
	virtual ~CButtonUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pivot(int x, int y) { m_fPivotX = x; m_fPivotY = y; }

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_float m_fPivotX;
	_float m_fPivotY;

public:
	_float m_fOffsetX;
	_float m_fOffsetY;

private:
	HRESULT Ready_Components(_wstring TextureTag);

public:
	static CButtonUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END