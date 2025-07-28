#include "stdafx.h"
#include "Inventory.h"
#include "ItemSlot.h"
#include "ItemIcon.h"

#include "GameInstance.h"


CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CInventory::CInventory(const CInventory& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	UI_DESC Desc{};
	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = InventorySizeX;
	Desc.fSizeY = InventorySizeY;

	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	m_Items.resize(ItemSlotLength, ITEM_NONE);
	m_SortLock.resize(ItemSlotLength, false);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta) {}

void CInventory::Update(_float fTimeDelta) 
{
	if (m_bActive == true)
	{
		Key_Input();

		Mouse_Input();
	}
}

void CInventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_bActive == true)
	{
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

		CItemSlot::s_fPivotY = m_fY;
		CItemSlot::s_fPivotX = m_fX;
		CItemIcon::s_fPivotY = m_fY;
		CItemIcon::s_fPivotX = m_fX;
		for (int i = 0; i < ItemSlotLength; ++i)
		{
			m_ItemSlots[i]->Late_Update(fTimeDelta);
			m_ItemIcons[i]->Late_Update(fTimeDelta);
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)m_ItemSlots[i]);
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)m_ItemIcons[i]);
		}

		if (m_iSelectedIndex != -1)
		{
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)(m_ItemIcons[m_iSelectedIndex]));
		}
	}
}

HRESULT CInventory::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CInventory::Key_Input()
{
	if (m_pGameInstance->GetButtonDown(KeyType::I))
	{
		m_bActive = !m_bActive;
	}

	if (m_pGameInstance->GetButtonDown(KeyType::Q))
	{
		Add_Item(ITEM_POTION_1, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::E))
	{
		Add_Item(ITEM_POTION_2, 1);
	}
}

void CInventory::Mouse_Input()
{
	POINT MousePos = m_pGameInstance->Get_MousePos();

	if (m_pGameInstance->GetButtonUp(KeyType::LeftMouse) && m_iSelectedIndex != -1)
	{
		int SwapIndex = MouseCheck(MousePos);
		if (SwapIndex != -1)
		{
			Swap_Item(SwapIndex, m_iSelectedIndex);
		}

		m_ItemIcons[m_iSelectedIndex]->Set_StickToMouse(false);
		m_iSelectedIndex = -1;
	}

	if (false == MouseOverButton(MousePos))
	{
		return;
	}

	if (MousePos.y <= m_fY - InventorySizeY * 0.5f + 25)
	{
		if(m_pGameInstance->GetButtonDown(KeyType::LeftMouse))
		{
			m_fX += m_pGameInstance->Get_DIMouseMove(DIMM_X);
			m_fY += m_pGameInstance->Get_DIMouseMove(DIMM_Y);
		}
	}

	int SelectedSlot = MouseCheck(MousePos);
	if (SelectedSlot == -1)
		return;

	if (m_pGameInstance->GetButtonDown(KeyType::RightMouse))
	{
		Replace_Item(SelectedSlot, 1);
	}
	else if (m_pGameInstance->GetButtonDown(KeyType::LeftMouse))
	{
		m_iSelectedIndex = SelectedSlot;
		m_ItemIcons[SelectedSlot]->Set_StickToMouse(true);
	}
}

int CInventory::MouseCheck(POINT MousePos)
{
	int Result{ -1 };
	for (int Index = 0; Index < ItemSlotLength; ++Index)
	{
		if (m_ItemSlots[Index]->MouseCheck(&MousePos) == true)
			Result = Index;
	}

	return Result;
}

bool CInventory::MouseOverButton(POINT pMouse)
{
	if (pMouse.x < m_fX - InventorySizeX * 0.5f
		|| pMouse.x > m_fX + InventorySizeX * 0.5f
		|| pMouse.y < m_fY - InventorySizeY * 0.5f
		|| pMouse.y > m_fY + InventorySizeY * 0.5f)
	{
		return false;
	}

	return true;
}

void CInventory::Swap_Item(int PickIndex, int DropIndex)
{
	swap(m_Items[PickIndex], m_Items[DropIndex]);
	Syncro_ItemSlot(PickIndex);
	Syncro_ItemSlot(DropIndex);
}

bool CInventory::Add_Item(ITEMID Item, int Amount)
{
	if (m_ItemSize[Item] != 0)
	{
		++m_ItemSize[Item];
		return true;
	}

	for (int i = 0; i < ItemSlotLength; ++i)
	{
		if (m_Items[i] == ITEM_NONE)
		{
			m_Items[i] = Item;
			m_ItemIcons[i]->Set_ItemIcon(Item);
			++m_ItemSize[Item];
			return true;
		}
	}
	
	return false;
}

bool CInventory::Replace_Item(int Index, int Amount)
{
	ITEMID PickItem = m_Items[Index];
	if (m_ItemSize[PickItem] < Amount)
	{
		return false;
	}

	m_ItemSize[PickItem] -= Amount;
	if (m_ItemSize[PickItem] == 0)
	{
		m_Items[Index] = ITEM_NONE;
		m_ItemIcons[Index]->Set_ItemIcon(ITEM_NONE);
	}

	return true;
}

void CInventory::Sort_Items()
{
	vector<ITEMID> Unlocked;

	for (int i = 0; i < ItemSlotLength; ++i)
	{
		if (ITEM_NONE == m_Items[i])
			continue;
		if (m_SortLock[i] == true)
			continue;

		Unlocked.push_back(m_Items[i]);
	}

	if (Unlocked.empty())
		return;

	sort(Unlocked.begin(), Unlocked.end());

	int Count = 0;
	for (int i = 0; i < ItemSlotLength; ++i)
	{
		if (true == m_SortLock[i])
			continue;
		m_Items[i] = Unlocked[Count];
		++Count;
		if (Count == Unlocked.size())
			return;
	}
}

void CInventory::Set_ItemIcon(ITEMID Item, int Index, int Amount)
{
	m_Items[Index] = Item;
	m_ItemIcons[Index]->Set_ItemIcon(Item);
}

void CInventory::Syncro_ItemSlot(int SlotIndex)
{
	m_ItemIcons[SlotIndex]->Set_ItemIcon(m_Items[SlotIndex]);
}

HRESULT CInventory::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TextureTag_Inventory,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CInventory::Ready_Parts()
{
	CItemSlot::ITEMSLOT_DESC Desc;
	m_ItemSlots.resize(ItemSlotLength, nullptr);
	m_ItemIcons.resize(ItemSlotLength, nullptr);
	for (int y = 0; y < ItemSlotLengthY; ++y)
	{
		for (int x = 0; x < ItemSlotLengthX; ++x)
		{
			Desc.fOffsetY = ItemSlotSize * y - 165;
			Desc.fOffsetX = ItemSlotSize * x - 180;
			int Index = y * ItemSlotLengthX + x;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_ItemSlots[Index], GameTag_ItemSlot, &Desc)))
				return E_FAIL;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_ItemIcons[Index], GameTag_ItemIcon, &Desc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CInventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CInventory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	
	for(CItemSlot* pItemSlot : m_ItemSlots)
	{
		Safe_Release(pItemSlot);
	}
	for (CItemIcon* pItemIcon : m_ItemIcons)
	{
		Safe_Release(pItemIcon);
	}

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}