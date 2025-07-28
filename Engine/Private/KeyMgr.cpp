#include "..\Public\KeyMgr.h"

#include "GameInstance.h"

KeyMgr::KeyMgr()
{
}

HRESULT KeyMgr::Initailize()
{
	_states.resize(KEY_TYPE_COUNT, KeyState::None);

	return S_OK;
}

void KeyMgr::Update()
{
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	//Ű���� 256���� ���¸� ������ �ܾ��
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (unsigned int key = 0; key < KEY_TYPE_COUNT; key++)
	{
		//Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			//���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KeyState::Press || state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			else
				state = KeyState::Down;
		}
		else
		{
			KeyState& state = _states[key];

			//���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else
				state = KeyState::None;
		}

	}
	// Mouse
	/*::GetCursorPos(&_mousePos);
	::ScreenToClient(g_, &_mousePos);*/
}

KeyMgr* KeyMgr::Create()
{
	KeyMgr* pInstance = new KeyMgr();
	if (FAILED(pInstance->Initailize()))
	{
		MSG_BOX(TEXT("Failed to Created : Key Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void KeyMgr::Free()
{
	__super::Free();
}
