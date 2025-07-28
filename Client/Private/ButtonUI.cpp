#include "stdafx.h"

#include "ButtonUI.h"
#include "GameInstance.h"


CButtonUI::CButtonUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CButtonUI::CButtonUI(const CButtonUI& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CButtonUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButtonUI::Initialize(void* pArg)
{
	BUTTONUI_DESC* Desc = static_cast<BUTTONUI_DESC*>(pArg);
	Desc->fX = 0; Desc->fY = 0;
	Desc->fSpeedPerSec = 0.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.f);
	m_fOffsetX = Desc->fOffsetX;
	m_fOffsetY = Desc->fOffsetY;

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components(Desc->TextureTag)))
		return E_FAIL;

	return S_OK;
}

void CButtonUI::Priority_Update(_float fTimeDelta)
{
}

void CButtonUI::Update(_float fTimeDelta)
{
}

void CButtonUI::Late_Update(_float fTimeDelta)
{
	m_fX = m_fPivotX + m_fOffsetX;
	m_fY = m_fPivotY + m_fOffsetY;

	__super::Late_Update(fTimeDelta);
}

HRESULT CButtonUI::Render()
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

HRESULT CButtonUI::Ready_Components(_wstring TextureTag)
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CButtonUI* CButtonUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CButtonUI* pInstance = new CButtonUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CButtonUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CButtonUI::Clone(void* pArg)
{
	CButtonUI* pInstance = new CButtonUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CButtonUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButtonUI::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}