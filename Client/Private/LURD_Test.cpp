#include "stdafx.h"
#include "LURD_Test.h"

#include "GameInstance.h"

CLURD_Test::CLURD_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice, pContext }
{
}

CLURD_Test::CLURD_Test(const CLURD_Test& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CLURD_Test::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLURD_Test::Initialize(void* pArg)
{
	UI_DESC Desc;
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

	m_pGameInstance->Add_RenderObject(CRenderer::RG_TEST, this);

	return S_OK;
}

void CLURD_Test::Priority_Update(_float fTimeDelta)
{
}

void CLURD_Test::Update(_float fTimeDelta)
{
	if (m_pGameInstance->GetButton(KeyType::KEY_1))
	{
		m_fSizeX += 1;
	}
	else 	if (m_pGameInstance->GetButton(KeyType::KEY_2))
	{
		m_fSizeX -= 1;
	}

	if (m_pGameInstance->GetButton(KeyType::KEY_3))
	{
		m_fSizeY += 1;
	}
	else if (m_pGameInstance->GetButton(KeyType::KEY_4))
	{
		m_fSizeY -= 1;
	}

	if (m_pGameInstance->GetButton(KeyType::J))
	{
		m_fX -= 1;
	}
	else if (m_pGameInstance->GetButton(KeyType::L))
	{
		m_fX += 1;
	}

	if (m_pGameInstance->GetButton(KeyType::I))
	{
		m_fY -= 1;
	}
	else if (m_pGameInstance->GetButton(KeyType::K))
	{
		m_fY += 1;
	}

}

void CLURD_Test::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLURD_Test::Render()
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

HRESULT CLURD_Test::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_LURD"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CLURD_Test* CLURD_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLURD_Test* pInstance = new CLURD_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLURD_Test"));
		Safe_Release(pInstance);
	}

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX(TEXT("Failed to Initialized : CLURD_TEST"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLURD_Test::Clone(void* pArg)
{
	CLURD_Test* pInstance = new CLURD_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLURD_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLURD_Test::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
