#pragma once
#include "Client_Defines.h"
#include "C:\Users\soboru\Desktop\Inventory\EngineSDK\Inc\UIObject.h"

#define ItemIconSize 30

#define GameTag_ItemIcon TEXT("Prototype_GameObject_ItemIcon")
#define TextureTag_ItemIcon TEXT("Prototype_Component_Texture_ItemIcon")

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)
class CItemIcon :
    public CUIObject
{
public:
	typedef struct ITEMICON_DESC : public CUIObject::UI_DESC
	{
		_float fOffsetX{}, fOffsetY{};
	};
private:
	CItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemIcon(const CItemIcon& Prototype);
	virtual ~CItemIcon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_ItemIcon(ITEMID Item) { m_Item = Item; }
	void Set_StickToMouse(bool StickToMouse) { m_bStickToMouse = StickToMouse; }

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	static _float s_fPivotX;
	static _float s_fPivotY;

private:
	_float m_fOffsetX;
	_float m_fOffsetY;
	bool m_bStickToMouse;
	ITEMID m_Item = { ITEM_NONE };

private:
	HRESULT Ready_Components();

public:
	static CItemIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END