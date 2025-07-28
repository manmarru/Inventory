#include "stdafx.h"

#include "ItemIcon.h"
#include "GameInstance.h"

_float CItemIcon::s_fPivotX = 0;
_float CItemIcon::s_fPivotY = 0;


CItemIcon::CItemIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CItemIcon::CItemIcon(const CItemIcon& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CItemIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemIcon::Initialize(void* pArg)
{
	ITEMICON_DESC* Desc = static_cast<ITEMICON_DESC*>(pArg);
	Desc->fX = g_iWinSizeX >> 1;
	Desc->fY = g_iWinSizeY >> 1;
	Desc->fSizeX = ItemIconSize;
	Desc->fSizeY = ItemIconSize;

	Desc->fSpeedPerSec = 0.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.f);

	m_fOffsetX = Desc->fOffsetX;
	m_fOffsetY = Desc->fOffsetY;

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CItemIcon::Priority_Update(_float fTimeDelta)
{
}

void CItemIcon::Update(_float fTimeDelta)
{
}

void CItemIcon::Late_Update(_float fTimeDelta)
{
	m_fX = s_fPivotX + m_fOffsetX;
	m_fY = s_fPivotY + m_fOffsetY;

	__super::Late_Update(fTimeDelta);
}

HRESULT CItemIcon::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", m_Item)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemIcon::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TextureTag_ItemIcon,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CItemIcon* CItemIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemIcon* pInstance = new CItemIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItemIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemIcon::Clone(void* pArg)
{
	CItemIcon* pInstance = new CItemIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItemIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemIcon::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}