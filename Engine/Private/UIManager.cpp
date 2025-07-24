
#include "uimanager.h"

CUIManager::CUIManager()
{
}

HRESULT CUIManager::Initialize()
{
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
