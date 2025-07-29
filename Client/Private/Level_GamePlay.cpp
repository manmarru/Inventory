#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

//#include "FreeCamera.h"
#include "FixedCamera.h"
#include "GameInstance.h"
#include "Inventory.h"
#include "LURD_Test.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;
	if(FAILED(Ready_Layer_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Ready_Test()))
		return E_FAIL;
#endif // _DEBUG

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_NeoDun"), TEXT("../Bin/Resources/Fonts/NeoDun.spritefont"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta) 
{
#ifdef _DEBUG
	m_pTestObj->Update(fTimeDelta);
	m_pTestObj->Late_Update(fTimeDelta);
#endif
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	CCamera::CAMERA_DESC		Desc{};

	Desc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 30.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FixedCamera"), &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_IngameBG"))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Sky"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), GameTag_Inventory)))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG
HRESULT CLevel_GamePlay::Ready_Test()
{
	m_pTestObj = CLURD_Test::Create(m_pDevice, m_pContext);
	return S_OK;
}
#endif // _DEBUG

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pTestObj);
#endif // _DEBUG

	__super::Free();
}
