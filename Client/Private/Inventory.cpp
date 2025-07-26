#include "stdafx.h"
#include "Inventory.h"

#include "GameInstance.h"


CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CInventory::CInventory(const CInventory& Prototype)
	:CUIObject{ Prototype }
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
	Desc.fSizeX = 420;
	Desc.fSizeY = 560;

	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	m_Items.resize(INVENTORYSIZE, ITEM_NONE);
	m_SortLock.resize(INVENTORYSIZE, false);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta) {}

void CInventory::Update(_float fTimeDelta) 
{
	Key_Input();
}

void CInventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if(m_bActive == true)
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
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
	if (m_pGameInstance->Get_DIKeyState(DIK_I) & 0x8000)
	{
		if(false == m_KeyDown_I)
			m_bActive = !m_bActive;
		m_KeyDown_I = true;
	}
	else
		m_KeyDown_I = false;
}

void CInventory::Swap_Item(COOR Pick, COOR Drop)
{
	swap(m_Items[Pick.y * INVENTORYSIZEX + Pick.x], m_Items[Pick.y * INVENTORYSIZEX + Pick.x]);
}

bool CInventory::Add_Item(ITEMID Item, int Amount)
{
	if (m_ItemSize[Item] != 0)
	{
		++m_ItemSize[Item];
		return true;
	}

	for (int i = 0; i < INVENTORYSIZE; ++i)
	{
		if (m_Items[i] == ITEM_NONE)
		{
			m_Items[i] == Item;
			++m_ItemSize[Item];
			return true;
		}
	}
	
	return false;
}

bool CInventory::Replace_Item(COOR Pick, int Amount)
{
	ITEMID PickItem = m_Items[Pick.y * INVENTORYSIZEX + Pick.x];
	if (m_ItemSize[PickItem] < Amount)
	{
		return false;
	}

	m_ItemSize[PickItem] -= Amount;
	if (m_ItemSize[PickItem] == 0)
	{
		m_Items[Pick.y * INVENTORYSIZEX + Pick.x] == ITEM_NONE;
	}
	

	return false;
}

void CInventory::Sort_Items()
{
	vector<ITEMID> Unlocked;

	for (int i = 0; i < INVENTORYSIZE; ++i)
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
	for (int i = 0; i < INVENTORYSIZE; ++i)
	{
		if (true == m_SortLock[i])
			continue;
		m_Items[i] = Unlocked[Count];
		++Count;
		if (Count == Unlocked.size())
			return;
	}
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
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
