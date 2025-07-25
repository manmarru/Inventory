
#include "uimanager.h"
#include "GameObject.h"

CUIManager::CUIManager()
{
}

HRESULT CUIManager::Initialize()
{
	return S_OK;
}

HRESULT CUIManager::Add_RenderObejct(CGameObject* pRenderObject)
{
	if (nullptr == pRenderObject)
		return E_FAIL;

	m_RenderUIs.emplace_back(pRenderObject);
	Safe_Release(pRenderObject);

	return S_OK;
}

HRESULT CUIManager::Render()
{
	for (auto UI : m_RenderUIs)
	{
		if (nullptr != UI)
			UI->Render();

		Safe_Release(UI);
	}

	m_RenderUIs.clear();
	return S_OK;
}

CUIManager* CUIManager::Create()
{
	CUIManager* pInstance = new CUIManager();

	return pInstance;
}

void CUIManager::Free()
{
	__super::Free();
}
