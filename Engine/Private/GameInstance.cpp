#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"
#include "Input_Device.h"
#include "UIManager.h"
#include "Font_Manager.h"
#include "SoundManager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const ENGINE_DESC& EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* �׷��� ī�带 �ʱ�ȭ�Ѵ�. */
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowsed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pSound_Manager = CSoundManager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;
	m_pSound_Manager->Initialize();

	/* �Է���ġ�� �ʱ�ȭ�Ѵ�. */
	m_pKeyMgr = KeyMgr::Create();
	if (nullptr == m_pKeyMgr)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(hInst, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	/* �������� �Ŵ����� �ʱ�ȭ�Ѵ�. */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	m_pUIManager = CUIManager::Create();
	if (nullptr == m_pUIManager)
		return E_FAIL;	

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return S_OK;
}



void CGameInstance::Update_Engine(_float fTimeDelta)
{
	/* ���� Ű����� ���콺�� ���¸� �޾ƿò���. */
	m_pInput_Device->Update();

	m_pKeyMgr->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);	

	m_pPipeLine->Update();

	m_pObject_Manager->Update(fTimeDelta);
	
	m_pObject_Manager->Late_Update(fTimeDelta);
	
	m_pLevel_Manager->Update(fTimeDelta);		

	m_pSound_Manager->Update();
}

HRESULT CGameInstance::Draw_Engine()
{
	
	m_pRenderer->Draw();

	return m_pLevel_Manager->Render();
}

HRESULT CGameInstance::Clear(_uint iLevelIndex)
{
	/* Ư�� ������ ���� �����ϴ� �ڿ����� �ش� ������ ������ ��������. */
	m_pObject_Manager->Clear(iLevelIndex);

	/* ������Ʈ �����鵵 �������� �����߾���. */
	// m_pComponent_Manager->Clear(iLevelIndex);

	return S_OK;
}

void CGameInstance::Render_Begin()
{
	/*m_pGraphic_Device->Render_Begin();*/
	m_pGraphic_Device->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGraphic_Device->Clear_DepthStencil_View();
	

}

void CGameInstance::Render_End()
{
	/*m_pGraphic_Device->Render_End(hWnd);*/

	m_pGraphic_Device->Present();
	
	
}
#pragma region INPUT_DEVICE
_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);	
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

POINT CGameInstance::Get_MousePos()
{
	return m_pInput_Device->Get_MousePos();
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel * pNextLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNextLevel);	
}

#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT CGameInstance::Add_Prototype(const _wstring & strPrototypeTag, CGameObject * pPrototype)
{
	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strPrototypeTag, void * pArg)
{
	return m_pObject_Manager->Add_CloneObject_ToLayer(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Clone_Prototype(CGameObject** pClone, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Manager->Clone_Prototype(pClone, strPrototypeTag, pArg);
}

CComponent * CGameInstance::Find_Component(_uint iLevelIndex, const _wstring & strLayerTag, const _wstring & strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);	
}

#pragma endregion

#pragma region COMPONENT_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CComponent * pPrototype)
{
	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region TIMER_MANAGER

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

_float CGameInstance::Compute_TimeDelta(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion


#pragma region RENDERER

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroupID, CGameObject * pRenderObject)
{
	return m_pRenderer->Add_RenderObject(eRenderGroupID, pRenderObject);
}
#pragma endregion

#pragma region PIPELINE
void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}
_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}
_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}
_float4x4 CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}
_matrix CGameInstance::Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

_float4 CGameInstance::Get_CamPosition_Float4() const
{
	return m_pPipeLine->Get_CamPosition_Float4();
}
_vector CGameInstance::Get_CamPosition_Vector() const
{
	return m_pPipeLine->Get_CamPosition_Vector();
}
#pragma endregion

#pragma region KEYMGR
//������ ���� ��
bool CGameInstance::GetButton(KeyType key)
{
	return m_pKeyMgr->GetButton(key);
}
//�� ó�� ������ ��
bool CGameInstance::GetButtonDown(KeyType key)
{
	return m_pKeyMgr->GetButtonDown(key);
}
//�� ó�� �����ٰ� �� ��
bool CGameInstance::GetButtonUp(KeyType key)
{
	return m_pKeyMgr->GetButtonUp(key);
}
#pragma endregion

#pragma region FONT_MANAGER 

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
HRESULT CGameInstance::Render_Text(const _wstring& strFontTag, const _tchar* pText, _fvector vPosition, _fvector vColor, _float fRadian, _fvector vPivot, _float fScale)
{
	return m_pFont_Manager->Render(strFontTag, pText, vPosition, vColor, fRadian, vPivot, fScale);
}

#pragma endregion

#pragma region SOUND_MANAGER

void CGameInstance::Update_Sound()
{
	m_pSound_Manager->Update();
}

HRESULT CGameInstance::PlayDefault(TCHAR* _TagSound, _int _Channel, _float _fVloume, _bool _bLoop)
{
	return m_pSound_Manager->PlayDefault(_TagSound, CSoundManager::SOUND_CHANNEL(_Channel), _fVloume, _bLoop);
}

HRESULT CGameInstance::PlayBGM(const TCHAR* _TagSound, _float _fVolume, _bool _bLoop)
{
	return m_pSound_Manager->PlayBGM(_TagSound, _fVolume, _bLoop);
}

HRESULT CGameInstance::Play3D(TCHAR* _TagSound, _int _Channel, _float _fVolume, _float3 _vPos, _bool _bLoop)
{
	return m_pSound_Manager->Play3D(_TagSound, _Channel, _fVolume, _vPos, _bLoop);
}

HRESULT CGameInstance::Play_NonStack(TCHAR* _TagSound, _int _Channel, _float _fVolume, _bool _bLoop)
{
	return m_pSound_Manager->Play_NonStack(_TagSound, _Channel, _fVolume, _bLoop);
}

void CGameInstance::MuteAll()
{
	m_pSound_Manager->MuteAll();
}

void CGameInstance::StopSound(int _iChannel)
{
	m_pSound_Manager->StopSound(CSoundManager::SOUND_CHANNEL(_iChannel));
}

void CGameInstance::StopSoundALL()
{
	m_pSound_Manager->StopSoundALL();
}

void CGameInstance::Set_ChannelVolume(int _Channel, _float _fVolume)
{
	m_pSound_Manager->SetChannelVolume(CSoundManager::SOUND_CHANNEL(_Channel), _fVolume);
}

HRESULT CGameInstance::Set_SoundPos(_int _iChannel, _float3 _vPos)
{
	return m_pSound_Manager->Set_SoundPos(_iChannel, _vPos);
}

HRESULT CGameInstance::Add_Sound(const char* _fullpath, const TCHAR* _filename, const TCHAR* _path)
{
	return m_pSound_Manager->Add_Sound(_fullpath, _filename, _path);
}

HRESULT CGameInstance::Load_Sound(const char* _folderName)
{
	return m_pSound_Manager->Load_Sound(_folderName);
}

HRESULT CGameInstance::Set_Listener(_float3 _vPos, _float3 _vLook, _float3 _vUp)
{
	return m_pSound_Manager->Set_Listener(_vPos, _vLook, _vUp);
}

#pragma endregion

void CGameInstance::Release_Engine()
{	
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pUIManager);
	Safe_Release(m_pKeyMgr);
	Safe_Release(m_pSound_Manager);

	CGameInstance::Get_Instance()->Destroy_Instance();	
}

void CGameInstance::Free()
{	
	__super::Free();

}
