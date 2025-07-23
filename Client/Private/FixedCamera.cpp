#include "stdafx.h"
#include "..\Public\FixedCamera.h"

#include "GameInstance.h"

CFixedCamera::CFixedCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera { pDevice, pContext }
{
}

CFixedCamera::CFixedCamera(const CFixedCamera & Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CFixedCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFixedCamera::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CFixedCamera::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CFixedCamera::Update(_float fTimeDelta)
{
}

void CFixedCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CFixedCamera::Render()
{
	return S_OK;
}

CFixedCamera * CFixedCamera::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFixedCamera*		pInstance = new CFixedCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFixedCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CFixedCamera::Clone(void * pArg)
{
	CFixedCamera*		pInstance = new CFixedCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFixedCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFixedCamera::Free()
{
	__super::Free();

}
