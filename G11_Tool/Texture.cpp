//=============================================================================
//�e�N�X�`���̏���
//Creater:Arai Yuhki
//=============================================================================
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Texture.h"
#include "Mutex.h"
//=============================================================================
//�O���[�o���ϐ�
//=============================================================================
//�t�@�C���l�[���z��
const LPCSTR TextureFileName[] = {
	"data/texture/white.bmp",

	"data/texture/Interface/Number.png",
	"data/texture/Interface/CGauge.png",
	"data/texture/Interface/GaugePoint.png",

	"data/texture/Effect/SkyMap.png",
	"data/texture/Effect/GrowUp.png",
	"data/texture/Effect/CheckPoint.png",

	"data/texture/Interface/gauge.png",
	"data/texture/Interface/gaugeFrame.png",
	"data/texture/Interface/clock.png",
	"data/texture/Interface/needle.png",

	"data/texture/Char/eatan.png",
	"",
	"data/texture/Item/usagi.png",
	"data/texture/Item/neko.png",
	"data/texture/Item/kuma.png",

	"data/texture/Interface/botun.png",

	"data/texture/field1.jpg",
	"data/texture/result.jpg",
	"data/texture/gelf���S.png",

	"data/texture/Stage/field1.jpg",
	"data/texture/Stage/tenzyou1.png",
	"data/texture/Stage/yuka1.png",

	"data/texture/Stage/ueki.png",

	"data/texture/Tool/Sprite.png",
	"data/texture/Tool/Wall.png",
	"data/texture/Tool/StertDeviceTrigger.png",
	"data/texture/Tool/StertDeviceON.png",
	"data/texture/Tool/StertDeviceTimer.png",
	"data/texture/Tool/StertDeviceSwitch.png",
	"data/texture/Tool/Gimmick.png",
	"data/texture/Tool/Item.png",
	"data/texture/Tool/Roof.png",

};
LPDIRECT3DTEXTURE9 Texture[TEXTURE_MAX];
static D3DXIMAGE_INFO ImageInfo[TEXTURE_MAX];
//=============================================================================
//������
//=============================================================================
void InitTexture(void)
{
	LPDIRECT3DDEVICE9 Device = Window::Instance()->Device();
	Mutex* mutex = Mutex::Instance();
	for (int cnt = 0;cnt<TEXTURE_MAX;cnt++)
	{
		//D3DXCreateTextureFromFile(Device,TextureName[cnt],&Texture[cnt]);
		D3DXGetImageInfoFromFile(TextureFileName[cnt],&ImageInfo[cnt]);

		mutex->Enter();
		D3DXCreateTextureFromFileEx(Device,TextureFileName[cnt],ImageInfo[cnt].Width,ImageInfo[cnt].Height,
			1,D3DUSAGE_AUTOGENMIPMAP,ImageInfo[cnt].Format,D3DPOOL_MANAGED,D3DX_FILTER_TRIANGLE,D3DX_FILTER_LINEAR,0,NULL,NULL,&Texture[cnt]);
		mutex->Leave();
	}
}
//=============================================================================
//�I��
//=============================================================================
void UninitTexture(void)
{
	for(int cnt=0;cnt<TEXTURE_MAX;cnt++)
	{
		if(Texture[cnt]!= NULL)
		{
			Texture[cnt]->Release();
			Texture[cnt] = NULL;
		}
	}
}
//=============================================================================
//�e�N�X�`���̎擾
//=============================================================================
LPDIRECT3DTEXTURE9 GetTexture(TEX id)
{
	return Texture[id];

}

D3DXVECTOR2 GetImageSize(TEX texture)
{
	return D3DXVECTOR2((float)ImageInfo[texture].Width,(float)ImageInfo[texture].Height);
}
D3DXIMAGE_INFO GetImageInfo(TEX texture)
{
	return ImageInfo[texture];
}
