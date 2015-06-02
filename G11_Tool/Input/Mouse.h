#ifndef _MOUSE_H_
#define _MOUSE_H_

#include"VC.h"
class Mouse
{
public:
	enum
	{
		MOUSE_LEFT =0,
		MOUSE_RIGHT,
		MOUSE_CENTER,
		MOUSE_MAX,
	};

	enum
	{
		WHEEL_NEUTRAL=0,
		WHEEL_UP,
		WHEEL_DOWN,
		WHEEL_MAX
	};

	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	void Update(void);
	static void Uninit(void);
	float PosX(void);	//X���W�擾
	float PosY(void);	//Y���W�擾
	float MoveX(void);	//X���W�擾
	float MoveY(void);	//Y���W�擾
	int Wheel(void);	//�E�B�[�����擾
	bool Press(int nKey);	//�v���X���
	bool Trigger(int nKey);	//�g���K�[���
	bool Release(int nKey);	//�����[�X���
	static void SetHWND(HWND hWnd){m_phWnd = hWnd;}

	static Mouse* Instance(void){return Self;}
private:
	Mouse(){};
	~Mouse();
	Mouse(const Mouse& other){};
	Mouse &operator= (const Mouse& other){};
	static Mouse* Self;

	LPDIRECTINPUTDEVICE8 InputDevice;
	DIMOUSESTATE2 MouseStatus;				//�}�E�X�̃X�e�[�^�X�ۑ��p
	BYTE TriggerMouse[3];							//�}�E�X�{�^���̃g���K�[���
	BYTE ReleaseMouse[3];							//�}�E�X�{�^���̃����[�X���
	int WheelStatus;								//�E�B�[���̏��
	float _MoveX;
	float _MoveY;
	POINT pos;
	D3DXVECTOR2 OriginalPos;
	static HWND m_phWnd;
};

#endif