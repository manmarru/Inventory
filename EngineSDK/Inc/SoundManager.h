#pragma once
#include "Base.h"
#include "fmod.h"
#include "fmod.hpp"
#include <io.h>

BEGIN(Engine)

class CSoundManager :
	public CBase
{
private:
	CSoundManager();
	virtual ~CSoundManager();

public:
	void Initialize();
	void Update();			//���� �Լ��� �������� �����Ӹ��� ������

	FMOD::System* Get_System() { return m_pSystem; }

public:
	enum SOUND_CHANNEL
	{
		CHANNEL_BGM, 


		CHANNEL_TALK = 25,
		CHANNEL_CHAOS = 26,
		CHANNEL_END
	};

public:
	HRESULT		PlayDefault(TCHAR* _TagSound, SOUND_CHANNEL _Channel, _float _fVloume, _bool _bLoop);
	HRESULT		PlayBGM(const TCHAR* _TagSound, _float _fVolume, _bool _bLoop);
	HRESULT		Play3D(TCHAR* _TagSound, _int _Channel, _float _fVolume, _float3 _vPos, _bool _bLoop);
	HRESULT		Play_NonStack(TCHAR* _TagSound, int _Channel, _float _fVolume, _bool _bLoop);
	HRESULT		Add_Sound(const char* _fullpath, const TCHAR* _filename, const TCHAR* _path);
	HRESULT		Load_Sound(const char* _folderName);
	void		StopSound(SOUND_CHANNEL _Channel);
	void		MuteAll();
	void		StopSoundALL();
	void		SetChannelVolume(SOUND_CHANNEL _Channel, _float _fVolume);
	HRESULT		Set_Listener(_float3 _vListener, _float3 _vLook, _float3 _vUp);
	HRESULT		Set_SoundPos(_int _iChannel, _float3 _vPos);

private:
	map<const TCHAR*, FMOD::Sound*>	m_mapSound; //�������
	FMOD::Channel* m_pChannel[32]{ nullptr };		//ä��
	FMOD::System* m_pSystem;					//�ý���

private:
	FMOD::Sound* Find_Sound(TCHAR* _TagSound);

public:
	static CSoundManager* Create();
	virtual void Free();
};

END