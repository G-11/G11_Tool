//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : TOMOHIRO KOUNO
//
//=============================================================================
#include "sound.h"

Sound* Sound::Self = nullptr;

float Sound::MasterVolumeBGM = 1.0f;
float Sound::MasterVolumeSE = 1.0f;

// �e���f�ނ̃p�����[�^
const PARAM Sound::Param[MAX_SOUND] =
{
	{ NULL,false }, //��f�[�^�Ƃ��ėp��
	{ "data/sound/BGM/���̔j��.wav",true },
	{ "data/sound/SE/Jingle.wav",false },

};

Sound::Sound()
{
	XAudio2Device = nullptr;
	MasteringVoice = nullptr;
	for (int cnt = 0;cnt < MAX_SOUND;cnt++)
	{
		SourceVoice[cnt] = nullptr;
		AudioData[cnt] = nullptr;
		AudioSize[cnt] = 0;
		CurrentVolume[cnt] = 1.0f;
	}
	MasterVolumeBGM = 1.0f;
	MasterVolumeSE = 1.0f;
}

void Sound::Initialize(void)
{
	Self = new Sound;
	if (Self)
	{
		Self->Init();
	}
}

void Sound::Finalize(void)
{
	if (Self != nullptr)
	{
		Self->Uninit();
		delete Self;
		Self = nullptr;
	}
}
//=============================================================================
// ������
//=============================================================================
HRESULT Sound::Init(void)
{
	HWND hWnd = Window::Instance()->hWnd();
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&XAudio2Device,0);
	if (FAILED(hr))
	{
		MessageBox(hWnd,"XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I","�x���I",MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = XAudio2Device->CreateMasteringVoice(&MasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd,"�}�X�^�[�{�C�X�̐����Ɏ��s�I","�x���I",MB_ICONWARNING);

		if (XAudio2Device)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			XAudio2Device->Release();
			XAudio2Device = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for (int cnt = 1; cnt < MAX_SOUND; cnt++)
	{

		HANDLE file;
		DWORD chunkSize = 0;
		DWORD chunkPosition = 0;
		DWORD fileType;
		WAVEFORMATEXTENSIBLE format;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&format,0,sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer,0,sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		file = CreateFile(Param[cnt].FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			SourceVoice[cnt] = nullptr;
			continue;
			//MessageBox(hWnd,"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
			//return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(file,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd,"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(file,'FFIR',&chunkSize,&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(file,&fileType,sizeof(DWORD),chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}
		if (fileType != 'EVAW')
		{
			MessageBox(hWnd,"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(file,' tmf',&chunkSize,&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(file,&format,chunkSize,chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(file,'atad',&AudioSize[cnt],&chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}
		AudioData[cnt] = (BYTE*)malloc(AudioSize[cnt]);
		hr = ReadChunkData(file,AudioData[cnt],AudioSize[cnt],chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd,"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}

		// �\�[�X�{�C�X�̐���
		hr = XAudio2Device->CreateSourceVoice(&SourceVoice[cnt],&(format.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd,"�\�[�X�{�C�X�̐����Ɏ��s�I","�x���I",MB_ICONWARNING);
			return S_FALSE;
		}

		memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = AudioSize[cnt];
		buffer.pAudioData = AudioData[cnt];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = 0;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		SourceVoice[cnt]->SubmitSourceBuffer(&buffer);

		//�X�e�[�^�X�̏�����
		Volume[cnt] = 0;
		ChangeVolume[cnt] = 0;
		FadeFlag[cnt] = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Sound::Uninit(void)
{
	// �ꎞ��~
	for (int cnt = 0; cnt < MAX_SOUND; cnt++)
	{
		if (SourceVoice[cnt])
		{
			// �ꎞ��~
			SourceVoice[cnt]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			SourceVoice[cnt]->DestroyVoice();
			SourceVoice[cnt] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(AudioData[cnt]);
			AudioData[cnt] = NULL;
			SourceVoice[cnt] = nullptr;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	MasteringVoice->DestroyVoice();
	MasteringVoice = NULL;

	if (XAudio2Device)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		XAudio2Device->Release();
		XAudio2Device = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(��~)
//=============================================================================
HRESULT Sound::Play(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return E_FAIL;
	}
	XAUDIO2_VOICE_STATE state;

	XAUDIO2_BUFFER buffer;

	memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = AudioSize[label];
	buffer.pAudioData = AudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	//���x���̃p�����[�^�����Ė������[�v���P��SE����ݒ�
	buffer.LoopCount = (Param[label].Loop) ? XAUDIO2_LOOP_INFINITE : 0;

	// ��Ԏ擾
	SourceVoice[label]->GetState(&state);
	if (state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		SourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		SourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	SourceVoice[label]->SubmitSourceBuffer(&buffer);
	//Volume[label] = ChangeVolume[label] = 1.0f;
	// �Đ�
	SourceVoice[label]->Start(0);
	SourceVoice[label]->SetVolume((Param[label].Loop) ? MasterVolumeBGM : MasterVolumeSE);
	

	return S_OK;
}

//=============================================================================
// �ꎞ��~����̃Z�O�����g�Đ�
//=============================================================================
HRESULT Sound::Pause(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return E_FAIL;
	}
	//�Đ����\�ȃf�[�^�Ȃ�Đ���v��

	XAUDIO2_VOICE_STATE state;

	XAUDIO2_BUFFER buffer;

	memset(&buffer,0,sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = AudioSize[label];
	buffer.pAudioData = AudioData[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	//���x���̃p�����[�^�����Ė������[�v���P��SE����ݒ�
	buffer.LoopCount = (Param[label].Loop) ? XAUDIO2_LOOP_INFINITE : 0;

	// ��Ԏ擾
	SourceVoice[label]->GetState(&state);
	// �Đ��I�����Ă����ꍇ
	if (state.BuffersQueued == 0)
	{
		//���[�v�ݒ�Ȃ��������ꍇ
		if (buffer.LoopCount == 0)
		{
			// ��~
			SourceVoice[label]->Stop(0);
			SourceVoice[label]->FlushSourceBuffers();

			// �I�[�f�B�I�o�b�t�@�̓o�^
			SourceVoice[label]->SubmitSourceBuffer(&buffer);
			SourceVoice[label]->SetVolume(MasterVolumeSE);
			CurrentVolume[label] = 1.0f;
		}
		// �Đ�
		SourceVoice[label]->Start(0);
	}
	else
	{
		//�Đ����Ȃ�ꎞ��~
		SourceVoice[label]->Stop(0);
	}


	return S_OK;
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void Sound::Stop(SOUND_TYPE label)
{
	if (SourceVoice[label] == nullptr)
	{
		return ;
	}
	XAUDIO2_VOICE_STATE state;

	// ��Ԏ擾
	SourceVoice[label]->GetState(&state);
	if (state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		SourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		SourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void Sound::Stop(void)
{
	// �ꎞ��~
	for (int cnt = 0; cnt < MAX_SOUND; cnt++)
	{
		Stop((SOUND_TYPE)cnt);
		//if (SourceVoice[cnt])
		//{
		//	// �ꎞ��~
		//	SourceVoice[cnt]->Stop(0);
		//}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT Sound::CheckChunk(HANDLE file,DWORD format,DWORD *pChunkSize,DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD read;
	DWORD chunkType;
	DWORD chunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD fileType;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file,0,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(file,&chunkType,sizeof(DWORD),&read,NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(file,&chunkDataSize,sizeof(DWORD),&read,NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunkType)
		{
		case 'FFIR':
			RIFFDataSize = chunkDataSize;
			chunkDataSize = 4;
			if (ReadFile(file,&fileType,sizeof(DWORD),&read,NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(file,chunkDataSize,NULL,FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format)
		{
			*pChunkSize = chunkDataSize;
			*pChunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= RIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT Sound::ReadChunkData(HANDLE file,void *pBuffer,DWORD dwBuffersize,DWORD dwBufferoffset)
{
	DWORD read;

	if (SetFilePointer(file,dwBufferoffset,NULL,FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file,pBuffer,dwBuffersize,&read,NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//=============================================================================
//�T�E���h�̍X�V
//=============================================================================
void Sound::Update(void)
{

	for (int cnt = 1;cnt<MAX_SOUND;cnt++)
	{

		if (SourceVoice[cnt])
		{
			if (FadeFlag[cnt])
			{
				CurrentVolume[cnt] += ChangeVolume[cnt];

				if (CurrentVolume[cnt] <= 0)
				{//���������ɂȂ����Ƃ�
					SourceVoice[cnt]->SetVolume(0);//0�ɒ���
					CurrentVolume[cnt] = 0;
					ChangeVolume[cnt] = 0;
				}
				else if (CurrentVolume[cnt] > 1)
				{//100%�ȏ�ɂȂ��Ă��܂����Ƃ�
					ChangeVolume[cnt] = 0;
					CurrentVolume[cnt] = 1.0f;
					SourceVoice[cnt]->SetVolume(1.0f);
				}//else if

				if (CurrentVolume[cnt] <= 0.01f)
				{
					Stop((SOUND_TYPE)cnt);//�����~
					ChangeVolume[cnt] = 1;
					FadeFlag[cnt] = false;//�t�F�[�h�̃t���O��OFF
				}
			}
			float volume = (Param[cnt].Loop) ? CurrentVolume[cnt] * MasterVolumeBGM : CurrentVolume[cnt] * MasterVolumeSE;
			SourceVoice[cnt]->SetVolume(volume);//���ʂ�ύX
		}
	}//for
}
//=============================================================================
//���ʂ̒���
//����:SOUND_TYPE�c		�ύX���������̃��x��
//		float volume�c	�ύX��̉���(1:100%�A0:����)
//=============================================================================
void Sound::SetVolume(SOUND_TYPE label,float volume)
{
	XAUDIO2_VOICE_STATE state;

	if (volume<0)//���ɂȂ�Ȃ��悤�ɂ���
	{
		volume = 0;
	}
	
	SourceVoice[label]->GetState(&state);

	if (state.BuffersQueued != 0)
	{// �Đ���
		CurrentVolume[label] = volume;
		float vol = CurrentVolume[label] * (Param[label].Loop)? MasterVolumeBGM:MasterVolumeSE;
		SourceVoice[label]->SetVolume(vol);//���ʂ�ύX
	}
}
//=============================================================================
//���ʂ̒���
//����:SOUND_TYPE�c		�ύX���������̃��x��
//		float volume�c	�ύX��̉���(1:100% 0:����)
//		float flame�c	���t���[�������ĕω����邩
//=============================================================================
void Sound::SetFade(SOUND_TYPE label,float volume,int frame)
{
	if (volume<0)//�ݒ�l�𐳋K��
	{
		Volume[label] = 0;
	}
	else
	{
		Volume[label] = volume;
	}

	SourceVoice[label]->GetVolume(&ChangeVolume[label]);//���݂̉��ʂ��擾

	ChangeVolume[label] = (volume - ChangeVolume[label]) / frame;//1�t���[���ŕω����鉹�ʂ��v�Z
}
//=============================================================================
//���ׂẲ��̃t�F�[�h
//����:float flame�c	���t���[�������ăt�F�[�h�����邩�H
//=============================================================================
void Sound::Fade(int frame)
{
	for (int cnt = 1;cnt<MAX_SOUND;cnt++)
	{

		XAUDIO2_VOICE_STATE state;
		// ��Ԏ擾
		SourceVoice[cnt]->GetState(&state);
		if (state.BuffersQueued != 0 && FadeFlag[cnt] == false)
		{
			SourceVoice[cnt]->GetVolume(&ChangeVolume[cnt]);//���݂̉��ʂ��擾
			//�ω��ʂ��v�Z
			ChangeVolume[cnt] = -(ChangeVolume[cnt] / frame);
			Volume[cnt] = 0;
			FadeFlag[cnt] = true;//�t�F�[�h�t���O�𗧂Ă�
		}

	}
}

//=============================================================================
//���̃t�F�[�h
//����:SOUND_TYPE�c		�t�F�[�h�����������̃��x��
//		float flame�c	���t���[�������ăt�F�[�h�����邩�H
//=============================================================================
void Sound::Fade(SOUND_TYPE label,int frame)
{
	SourceVoice[label]->GetVolume(&ChangeVolume[label]);//���݂̉��ʂ��擾
	//�ω��ʂ��v�Z
	ChangeVolume[label] = -(ChangeVolume[label] / frame);
	Volume[label] = 0;
	FadeFlag[label] = true;//�t�F�[�h�t���O�𗧂Ă�
}