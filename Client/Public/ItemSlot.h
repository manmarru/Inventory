#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

#define ItemSlotSize 40

#define GameTag_ItemSlot TEXT("Prototype_GameObject_ItemSlot")
#define TextureTag_ItemSlot TEXT("Prototype_Component_Texture_ItemSlot")

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CItemSlot :
    public CUIObject
{
public:
	typedef struct ITEMSLOT_DESC : public CUIObject::UI_DESC
	{
		_float fOffsetX{}, fOffsetY{};
	};
private:
    CItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CItemSlot(const CItemSlot& Prototype);
    virtual ~CItemSlot() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	bool MouseCheck(POINT MousePos);

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	static _float s_fPivotX;
	static _float s_fPivotY;

private:
	_float m_fOffsetX;
	_float m_fOffsetY;
	_uint m_iRenderTexture{ 0 };

private:
	HRESULT Ready_Components();

public:
	static CItemSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END