//===========================================================
//
// �O�p�֐��e�[�u��	[MathTable.cpp]
//
//		����ҁ@�V�� �T�M
//
//===========================================================

//===========================================================
//�C���N���[�h
//===========================================================
#include "MathTable.h"

std::mt19937 CMath::MTRand;

CMath math;
//===========================================================
//�R���X�g���N�^
//===========================================================
CMath::CMath()
{

	//����������
	int cnt=0;

	while(cnt<360)
	{
		_tsin[cnt] = sinf(cnt*(PI/180.0f));
		if(cnt==0||cnt==180)
		{
			_tsin[cnt] = 0;
		}
		_tcos[cnt] = cosf(cnt*(PI/180.0f));
		if(cnt==90||cnt==270)
		{
			_tcos[cnt] = 0;
		}
		_ttan[cnt] = tanf(cnt*(PI/180.0f));
		cnt++;
	}
	float num = 256.0f;
	for (int cnt = 0;cnt <= 512;cnt++,num--)
	{
		_tatan[cnt] = (float)atan2(num,256);
	}
	std::random_device randDevice;
	MTRand = std::mt19937(randDevice());
}


//===========================================================
//�œK��
//===========================================================
int CMath::Optimize(int deg)
{
	deg = deg%360;//360�����Ɏ��߂�����

	if(deg<0)//0��菬���������琳�ʒu�ɍœK��
	{
		 deg+=360;
	}

	return deg;
}
//===========================================================
//���W�A�����f�B�O���[�ɍœK��
//===========================================================
int CMath::RadOptimize(float rad)
{
	int radiation = (int)(rad/(PI/180));//���W�A�����̋t
	radiation = radiation%360;

	if(radiation < 0)//0��菬���������琳�ʒu�ɍœK��
	{
		radiation += 360;
	}

	return radiation;
}
//===========================================================
//�f�B�O���[�����sin�l�擾
//===========================================================
float CMath::tSin_deg(int deg)
{	
	int degree = deg;
	degree = Optimize(degree);

	return _tsin[degree];
}
//===========================================================
//�f�B�O���[�����cos�l�擾
//===========================================================
float CMath::tCos_deg(int deg)
{
	int degree=deg;
	
	degree = Optimize(degree);

	return _tcos[degree];
}
//===========================================================
//�f�B�O���[�����tan�l�擾
//===========================================================
float CMath::tTan_deg(int deg)
{
	int degree=deg;
	
	degree = Optimize(degree);

	return _ttan[degree];
}
//===========================================================
//���W�A�������sin�l�擾
//===========================================================
float CMath::tSin_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _tsin[deg];
}
//===========================================================
//���W�A�������cos�l�擾
//===========================================================
float CMath::tCos_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _tcos[deg];
}
//===========================================================
//���W�A�������tan�l�擾
//===========================================================
float CMath::tTan_rad(float rad)
{
	int deg = RadOptimize(rad);

	return _ttan[deg];
}

float CMath::RandomF(float min,float max)
{
	std::uniform_real<float> dist(min,max);
	return dist(MTRand);
}

int CMath::RandomI(int min,int max)
{
	std::uniform_int<int> dist(min,max);
	return dist(MTRand);
}

float Sin_Rad(float rad)
{
	return math.tSin_rad(rad);
}

float Cos_Rad(float rad)
{
	return math.tCos_rad(rad);
}

float Tan_Rad(float rad)
{
	return math.tTan_rad(rad);
}

float Sin_Deg(int deg)
{
	return math.tSin_deg(deg);
}

float Cos_Deg(int deg)
{
	return math.tCos_deg(deg);
}

float Tan_Deg(int deg)
{
	return math.tTan_deg(deg);
}

float Atan2(float y,float x)
{
	if (x <= y)
	{
		if (x >= -y)
		{
			return math.tAtan2(x * (1 << 8) / y + (1 << 8)) + 1.57079637f;
		}
		else
		{
			if (y < 0)
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8)) + 3.1503193865f;
			}
			else
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8)) + PI;
			}

		}
	}
	else
	{
		if (x > -y)
		{
			if (y > 0)
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8));
			}
			else
			{
				return math.tAtan2(y * (1 << 8) / -x + (1 << 8))-0.0087266465f;
			}
		}
		else
		{
			return math.tAtan2(x * (1 << 8) / y + (1 << 8)) - 1.5795230165f;
		}
	}
	return 0;
}

float t_sqrtF(const float& x)
{
	float xHalf = 0.5f * x;
	int   tmp = 0x5F3759DF - (*(int*)&x >> 1); //initial guess
	float xRes = *(float*)&tmp;

	xRes *= (1.5f - (xHalf * xRes * xRes));
	//xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//�R�����g�A�E�g���O���Ɛ��x���オ��
	return xRes * x;
}

int Randi(int min,int max)
{
	return CMath::RandomI(min,max);
}
float Randf(float min,float max)
{
	return CMath::RandomF(min,max);
}


D3DXVECTOR2 EaseIn(const D3DXVECTOR3& startPos,const D3DXVECTOR3& destPos,float percent)
{
	D3DXVECTOR3 sub = -(startPos - destPos);
	D3DXVECTOR3 pos = sub * (percent*percent) + startPos;
	
	return D3DXVECTOR2(pos.x,pos.y);
}
D3DXVECTOR2 EaseOut(const D3DXVECTOR3& startPos,const D3DXVECTOR3& destPos,float percent)
{
	D3DXVECTOR3 sub = destPos - startPos;
	D3DXVECTOR3 pos = startPos + (2.0f*sub*percent) - (sub*percent*percent);

	return D3DXVECTOR2(pos.x,pos.y);
}
D3DXVECTOR2 EaseInOut(const D3DXVECTOR3& startPos,const D3DXVECTOR3& destPos,float percent)
{
	D3DXVECTOR3 sub = destPos - startPos;
	D3DXVECTOR3 pos = startPos + (-2.0f * sub * percent * percent * percent) + (3.0f * sub * percent * percent);
	return D3DXVECTOR2(pos.x,pos.y);
}

D3DXVECTOR2 Bezier(const D3DXVECTOR2& startPos,const D3DXVECTOR2& destPos,const D3DXVECTOR2& relay1,const D3DXVECTOR2& relay2,float percent)
{
	float rev = 1.0f - percent;
	float p[4] = { rev*rev*rev,3.0f*percent*(rev*rev),3.0f*(percent*percent)*rev,percent*percent*percent };
	return (startPos*p[0]) + (relay1*p[1]) + (relay2 * p[2]) + (destPos*p[3]);

}