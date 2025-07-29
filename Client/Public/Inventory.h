#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#define ItemSlotLength 100
#define ItemSlotLengthY 10
#define ItemSlotLengthX 10

#define InventorySizeX 410
#define InventorySizeY 500

#define GameTag_Inventory TEXT("Prototype_GameObject_Inventory")
#define TextureTag_Inventory TEXT("Prototype_Component_Texture_Inventory")


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CInventory :
    public CUIObject
{
private:
    CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CInventory(const CInventory& Prototype);
    virtual ~CInventory() = default;

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

public:
	void Active() { m_bActive = true; }
	void Key_Input();
	void Mouse_Input();
	int MouseCheck(POINT MousePos);

public:
	void Swap_Item(int PickIndex, int DropIndex);
	bool Add_Item(ITEMID Item, int Amount);
	bool Replace_Item(int Index, int Amount);
	void Sort_Items();
	void Set_ItemIcon(ITEMID Item, int Index, int Amount);
	void Syncro_ItemSlot(int SlotIndex);

private:
	int m_iSelectedIndex{ -1 };

private:
	vector<ITEMID> m_Items;
	vector<char> m_SortLock;
	unordered_map<ITEMID, int> m_ItemSize;

private:
	vector<class CItemSlot*> m_ItemSlots;
	vector<class CItemIcon*> m_ItemIcons;
	class CButtonUI* m_pSortButton;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();

public:
	static CInventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END