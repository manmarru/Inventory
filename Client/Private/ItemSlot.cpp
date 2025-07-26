#include "stdafx.h"
#include "ItemSlot.h"

#include "GameInstance.h"

CItemSlot::CItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CItemSlot::CItemSlot(const CItemSlot& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CItemSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemSlot::Initialize(void* pArg)
{
	UI_DESC Desc = { };
	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = 30;
	Desc.fSizeY = 30;

	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CItemSlot::Priority_Update(_float fTimeDelta)
{
}

void CItemSlot::Update(_float fTimeDelta)
{
}

void CItemSlot::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItemSlot::Render()
{
	return S_OK;
}

HRESULT CItemSlot::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TextureTag_ItemSlot,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}


CItemSlot* CItemSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemSlot* pInstance = new CItemSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItemSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemSlot::Clone(void* pArg)
{
	CItemSlot* pInstance = new CItemSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItemSlot"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemSlot::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
