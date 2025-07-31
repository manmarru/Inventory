#include "stdafx.h"
#include "Inventory.h"
#include "ItemSlot.h"
#include "ItemIcon.h"
#include "ButtonUI.h"
#include <fstream>
#include <string>

#include "GameInstance.h"


CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CInventory::CInventory(const CInventory& Prototype)
	:CUIObject(Prototype)
{
	m_bClone = true;
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

	m_PlayerItems.resize(PlayerItemSlotLength, ITEM_NONE);
	m_PlayerSortLock.resize(PlayerItemSlotLength, false);

	m_PetItems.resize(PetItemSlotLength, ITEM_NONE);
	m_PetSortLock.resize(PetItemSlotLength, false);

	m_Items = &m_PlayerItems;
	m_SortLock = &m_PlayerSortLock;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	Read_ItemData();

	Add_Item(ITEM_POTION_1, 40);
	Add_Item(ITEM_POTION_2, 60);
	Add_Item(ITEM_REDSTONEPOCKET, 10);
	Add_Item(ITEM_BLUESTONEPOCKET, 10);
	Add_Item(ITEM_POTION_AWAKEN, 1);
	Add_Item(ITEM_GRENADE_WIND, 50);
	Add_Item(ITEM_GRENADE_DARK, 50);

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
	else
	{
		if (m_pGameInstance->GetButtonDown(KeyType::I))
		{
			m_pGameInstance->PlayDefault(TEXT("Open.wav"), 1, 1.f, false);
			m_bActive = true;
		}
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
		m_pSortButton->Set_Pivot(m_fX, m_fY);
		m_pPetSlotButton->Set_Pivot(m_fX, m_fY);

		vector<CItemSlot*>& ItemSlots = *m_ItemSlots;
		vector<CItemIcon*>& ItemIcons = *m_ItemIcons;

		int Length = m_bPetActive ? PetItemSlotLength : PlayerItemSlotLength;
		for (int i = 0; i < Length; ++i)
		{
			ItemSlots[i]->Late_Update(fTimeDelta);
			ItemIcons[i]->Late_Update(fTimeDelta);
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)ItemSlots[i]);
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)ItemIcons[i]);
		}

		if (m_iSelectedIndex != -1)
		{
			m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)((*m_ItemIcons)[m_iSelectedIndex]));
		}

		m_pSortButton->Late_Update(fTimeDelta);
		m_pPetSlotButton->Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, (CGameObject*)(m_pSortButton));
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
	}
}

HRESULT CInventory::Render()
{
	if (m_bFontRender == false)
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

		m_bFontRender = true;
		return S_OK;
	}
	else
	{
		vector<CItemIcon*>& ItemIcons = *m_ItemIcons;
		vector<ITEMID>& Items = *m_Items;
		int Length = m_bPetActive ? PetItemSlotLength : PlayerItemSlotLength;
		for(int i = 0; i < Length; ++i)
		{
			int ItemSize = m_ItemSize[Items[i]];
			if (ItemSize == 0)
				continue;
			CItemIcon* Icon = ItemIcons[i];
			
			wstring Amount = to_wstring(ItemSize);
			m_pGameInstance->Render_Text(TEXT("Font_NeoDun"), Amount.c_str(), XMVectorSet(Icon->Get_fX(), Icon->Get_fY(), 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.5f);
		}
		m_bFontRender = false;

		return S_OK;
	}

	return E_FAIL;
}

void CInventory::PetActive()
{
	m_bPetActive = !m_bPetActive;
	if (true == m_bPetActive)
	{
		m_Items = &m_PetItems;
		m_SortLock = &m_PetSortLock;
		m_ItemIcons = &m_PetItemIcons;
		m_ItemSlots = &m_PetItemSlots;
	}
	else
	{
		m_Items = &m_PlayerItems;
		m_SortLock = &m_PlayerSortLock;
		m_ItemIcons = &m_PlayerItemIcons;
		m_ItemSlots = &m_PlayerItemSlots;
	}
}

void CInventory::Key_Input()
{
	if (m_pGameInstance->GetButtonDown(KeyType::I))
	{
		m_bActive = false;
		if (m_bActive == false)
		{
			m_pGameInstance->PlayDefault(TEXT("Close.wav"), 1, 1.f, false);
		}
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_1))
	{
		Add_Item(ITEM_POTION_1, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_2))
	{
		Add_Item(ITEM_POTION_2, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_3))
	{
		Add_Item(ITEM_REDSTONEPOCKET, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_4))
	{
		Add_Item(ITEM_BLUESTONEPOCKET, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_5))
	{
		Add_Item(ITEM_REDSTONE, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_6))
	{
		Add_Item(ITEM_BLUESTONE, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_7))
	{
		Add_Item(ITEM_GRENADE_WIND, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_8))
	{
		Add_Item(ITEM_GRENADE_DARK, 1);
	}

	if (m_pGameInstance->GetButtonDown(KeyType::KEY_9))
	{
		Add_Item(ITEM_POTION_AWAKEN, 1);
	}
}

void CInventory::Mouse_Input()
{
	POINT MousePos = m_pGameInstance->Get_MousePos();

	if (m_pPetSlotButton->MouseOverButton(MousePos) && m_pGameInstance->GetButtonUp(KeyType::LeftMouse) && m_iSelectedIndex != -1)
	{
		if (m_bPetActive == false)
		{
			for (int i = 0; i < PetItemSlotLength; ++i)
			{
				if (m_PetItems[i] == ITEM_NONE)
				{
					m_PetItems[i] = (*m_Items)[m_iSelectedIndex];
					(m_PetItemIcons)[i]->Set_ItemIcon(m_PetItems[i]);
					(*m_Items)[m_iSelectedIndex] = ITEM_NONE;
					(*m_ItemIcons)[m_iSelectedIndex]->Set_ItemIcon(ITEM_NONE);
				}
			}
		}
		else
		{
			for (int i = 0; i < PlayerItemSlotLength; ++i)
			{
				if (m_PlayerItems[i] == ITEM_NONE)
				{
					m_PlayerItems[i] = (*m_Items)[m_iSelectedIndex];
					m_PlayerItemIcons[i]->Set_ItemIcon(m_PlayerItems[i]);
					(*m_Items)[m_iSelectedIndex] = ITEM_NONE;
					(*m_ItemIcons)[m_iSelectedIndex]->Set_ItemIcon(ITEM_NONE);
				}
			}
		}
	}

	if (m_pGameInstance->GetButtonUp(KeyType::LeftMouse) && m_iSelectedIndex != -1) // 아이템 드래그
	{
		int SwapIndex = MouseCheck(MousePos);
		if (SwapIndex != -1)
		{
			Swap_Item(SwapIndex, m_iSelectedIndex);
		}

		(*m_ItemIcons)[m_iSelectedIndex]->Set_StickToMouse(false);
		m_iSelectedIndex = -1;
	}
	
	if (false == MouseOverButton(MousePos))
	{
		return;
	}

	if(m_pSortButton->MouseOverButton(MousePos) && m_pGameInstance->GetButtonDown(KeyType::LeftMouse))
	{
		Sort_Items();
		m_pGameInstance->PlayDefault(TEXT("Button_Select.wav"), 1, 1.f, false);
	}
	if (m_pPetSlotButton->MouseOverButton(MousePos) && m_pGameInstance->GetButtonDown(KeyType::LeftMouse))
	{
		m_pGameInstance->PlayDefault(TEXT("Button_Select.wav"), 1, 1.f, false);
		PetActive();
	}

	if (MousePos.y <= m_fY - InventorySizeY * 0.5f + 25)
	{
		if(m_pGameInstance->GetButton(KeyType::LeftMouse))
		{
			m_fX += m_pGameInstance->Get_DIMouseMove(DIMM_X);
			m_fY += m_pGameInstance->Get_DIMouseMove(DIMM_Y);
		}
	}

	int SelectedSlot = MouseCheck(MousePos);
	if (SelectedSlot == -1)
		return;


	if (m_pGameInstance->GetButtonDown(KeyType::HMouse))
	{
		(*m_SortLock)[SelectedSlot] = !(*m_SortLock)[SelectedSlot];
		(*m_ItemSlots)[SelectedSlot]->Sortlock();
	}
	if (m_pGameInstance->GetButtonDown(KeyType::RightMouse))
	{
		Use_Item(SelectedSlot);
	}
	else if (m_pGameInstance->GetButtonDown(KeyType::LeftMouse))
	{
		m_iSelectedIndex = SelectedSlot;
		(*m_ItemIcons)[SelectedSlot]->Set_StickToMouse(true);
	}
}

int CInventory::MouseCheck(POINT MousePos)
{
	int Result{ -1 };
	int Length = m_bPetActive ? PetItemSlotLength : PlayerItemSlotLength;
	for (int Index = 0; Index < Length; ++Index)
	{
		if ((*m_ItemSlots)[Index]->MouseCheck(MousePos) == true)
			Result = Index;
	}

	return Result;
}

void CInventory::Use_Item(int SlotIndex)
{
	pair<ITEMTYPE, int> ItemData = m_ItemTypes[(*m_Items)[SlotIndex]];
	switch (ItemData.first)
	{
	case ITEM_NORMAL:
		m_pGameInstance->PlayDefault(TEXT("Incorrect.wav"), 1, 1.f, false);
		break;
	case ITEM_BOX:
		Add_Item((ITEMID)ItemData.second, 1);
		Replace_Item(SlotIndex, 1);
		m_pGameInstance->PlayDefault(TEXT("BoxOpen.wav"), 1, 1.f, false);
		break;
	case ITEM_POTION:
		m_pGameInstance->PlayDefault(TEXT("Incorrect.wav"), 1, 1.f, false);
		break;
	case ITEMTYPE_END:
		break;
	default:
		break;
	}
}

void CInventory::Swap_Item(int PickIndex, int DropIndex)
{
	swap((*m_Items)[PickIndex], (*m_Items)[DropIndex]);
	swap((*m_SortLock)[PickIndex], (*m_SortLock)[DropIndex]);
	Syncro_ItemSlot(PickIndex);
	Syncro_ItemSlot(DropIndex);
	(*m_ItemSlots)[PickIndex]->SwapData((*m_ItemSlots)[DropIndex]);
}

bool CInventory::Add_Item(ITEMID Item, int Amount)
{
	if (m_ItemSize[Item] != 0)
	{
		++m_ItemSize[Item];
		return true;
	}

	vector<ITEMID>& Items = *m_Items;
	int Length = m_bPetActive ? PetItemSlotLength : PlayerItemSlotLength;
	for (int i = 0; i < Length; ++i)
	{
		if (Items[i] == ITEM_NONE)
		{
			Items[i] = Item;
			(*m_ItemIcons)[i]->Set_ItemIcon(Item);
			m_ItemSize[Item] += Amount;
			return true;
		}
	}
	
	return false;
}

bool CInventory::Replace_Item(int Index, int Amount)
{
	ITEMID PickItem = (*m_Items)[Index];
	if (m_ItemSize[PickItem] < Amount)
	{
		return false;
	}

	m_ItemSize[PickItem] -= Amount;
	if (m_ItemSize[PickItem] == 0)
	{
		(*m_Items)[Index] = ITEM_NONE;
		(*m_ItemIcons)[Index]->Set_ItemIcon(ITEM_NONE);		
	}

	return true;
}

void CInventory::Sort_Items()
{
	vector<ITEMID> Unlocked;

	vector<ITEMID>& Items = *m_Items;
	vector<char>& SortLock = *m_SortLock;
	int Length = m_bPetActive ? PetItemSlotLength : PlayerItemSlotLength;
	for (int i = 0; i < Length; ++i)
	{
		if (ITEM_NONE == Items[i])
			continue;
		if (SortLock[i] == true)
			continue;
		Unlocked.push_back(Items[i]);
		Items[i] = ITEM_NONE;
	}

	if (Unlocked.empty())
		return;

	sort(Unlocked.begin(), Unlocked.end());

	int Count = 0;
	for (int i = 0; i < Length; ++i)
	{
		if (true == SortLock[i])
			continue;
		Items[i] = Unlocked[Count];
		++Count;
		if (Count == Unlocked.size())
			break;
	}
	for (int i = 0; i < Length; ++i)
	{
		Syncro_ItemSlot(i);
	}
}

void CInventory::Set_ItemIcon(ITEMID Item, int Index, int Amount)
{
	(*m_Items)[Index] = Item;
	(*m_ItemIcons)[Index]->Set_ItemIcon(Item);
}

void CInventory::Syncro_ItemSlot(int SlotIndex)
{
	(*m_ItemIcons)[SlotIndex]->Set_ItemIcon((*m_Items)[SlotIndex]);
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
	//Pleyer
	m_PlayerItemSlots.resize(PlayerItemSlotLength, nullptr);
	m_PlayerItemIcons.resize(PlayerItemSlotLength, nullptr);
	for (int y = 0; y < PlayerItemSlotLengthY; ++y)
	{
		for (int x = 0; x < PlayerItemSlotLengthX; ++x)
		{
			Desc.fOffsetY = ItemSlotSize * y - 165;
			Desc.fOffsetX = ItemSlotSize * x - 177;
			int Index = y * PlayerItemSlotLengthX + x;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_PlayerItemSlots[Index], GameTag_ItemSlot, &Desc)))
				return E_FAIL;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_PlayerItemIcons[Index], GameTag_ItemIcon, &Desc)))
				return E_FAIL;
		}
	}
	//Pet
	m_PetItemSlots.resize(PetItemSlotLength, nullptr);
	m_PetItemIcons.resize(PetItemSlotLength, nullptr);
	for (int y = 0; y < PetItemSlotLengthY; ++y)
	{
		for (int x = 0; x < PlayerItemSlotLengthX; ++x)
		{
			Desc.fOffsetY = ItemSlotSize * y - 165;
			Desc.fOffsetX = ItemSlotSize * x - 177;
			int Index = y * PlayerItemSlotLengthX + x;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_PetItemSlots[Index], GameTag_ItemSlot, &Desc)))
				return E_FAIL;
			if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_PetItemIcons[Index], GameTag_ItemIcon, &Desc)))
				return E_FAIL;
		}
	}

	m_ItemSlots = &m_PlayerItemSlots;
	m_ItemIcons = &m_PlayerItemIcons;

	CButtonUI::BUTTONUI_DESC ButtonDesc;
	ButtonDesc.fOffsetX = -185;
	ButtonDesc.fOffsetY = -205;
	ButtonDesc.fSizeX = 24;
	ButtonDesc.fSizeY = 24;
	ButtonDesc.fX = 0;
	ButtonDesc.fY = 0;
	ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_ButtonUI_Sort");
	if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_pSortButton, GameTag_ButtonUI, &ButtonDesc)))
		return E_FAIL;

	ButtonDesc.fOffsetX = -95.f;
	ButtonDesc.fOffsetY = -206.f;
	ButtonDesc.fSizeX = 43.f;
	ButtonDesc.fSizeY = 28.f;
	ButtonDesc.fX = 0;
	ButtonDesc.fY = 0;
	ButtonDesc.TextureTag = TEXT("Prototype_Component_Texture_ButtonUI_PetSlot");
	if (FAILED(m_pGameInstance->Clone_Prototype((CGameObject**)&m_pPetSlotButton, GameTag_ButtonUI, &ButtonDesc)))
		return E_FAIL;

	return S_OK;
}

void CInventory::Read_ItemData()
{
	ifstream LoadStream("../Bin/Resources/Data/ItemTypes.txt");
	int Input[2];

	for (int i = 0; i < ITEM_END; ++i)
	{
		LoadStream >> Input[0] >> Input[1];
		m_ItemTypes[(ITEMID)i] = { (ITEMTYPE)Input[0], Input[1] };
	}	
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

	if (m_bClone == true)
	{
		for (CItemSlot* pItemSlot : m_PlayerItemSlots)
		{
			Safe_Release(pItemSlot);
		}
		for (CItemSlot* pItemSlot : m_PetItemSlots)
		{
			Safe_Release(pItemSlot);
		}
		for (CItemIcon* pItemIcon : m_PlayerItemIcons)
		{
			Safe_Release(pItemIcon);
		}
		for (CItemIcon* pItemIcon : m_PetItemIcons)
		{
			Safe_Release(pItemIcon);
		}
		Safe_Release(m_pSortButton);
		Safe_Release(m_pPetSlotButton);
	}

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}