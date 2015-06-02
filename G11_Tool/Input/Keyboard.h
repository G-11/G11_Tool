//=============================================================================
// �L�[�{�[�h�N���X
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//=============================================================================
// �C���N���[�h
//=============================================================================
#include"VC.h"
#include <Windows.h>
//=============================================================================
// �N���X��`
//=============================================================================
class Keyboard
{
public:
	
	~Keyboard();
	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	static void Uninit(void);
	void Update(void);

	bool Press(int nKey);
	bool Trigger(int nKey);
	bool Release(int nKey);
	bool Repeat(int nKey);

	static Keyboard* Instance(void){ return Self; }

private:
	Keyboard(){}
	Keyboard(const Keyboard& other){};
	Keyboard &operator=(const Keyboard& other){};

	static Keyboard* Self;
	BYTE KeyState[256];		//�L�[���͏��
	BYTE TriggerKeyState[256];	//�g���K�[�L�[���͏��
	BYTE ReleaseKeyState[256];	//�����[�X�L�[���͏��
	BYTE RepeateKeyState[256];	//���s�[�g�L�[���͏��
	short RepeateKeyFlame[256];	//���s�[�g�L�[�p�t���[���J�E���g

	LPDIRECTINPUTDEVICE8 InputDevice;

};

#endif