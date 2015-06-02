//=============================================================================
//�����N���X
//=============================================================================
#include "Font.h"
#include "Mutex.h"
#include <windows.h>
#include <tchar.h>
#include <cstdlib>

const char* FaceName = "���˖��� Pro B\0";

FONT_DATA Font::FontData;
LOGFONT Font::FastFont = { 260,0,0,0,500,0,0,0,SHIFTJIS_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH | FF_MODERN,_T("�l�r �o����") };
//=============================================================================
//�����N���X
//=============================================================================
//�����ŕۊǂ���F�\����
//=============================================================================
struct SColor {
	unsigned char A,R,G,B;
	unsigned color;
	SColor(unsigned color) : color(color),B(color & 0xff),G((color >> 8) & 0xff),R((color >> 16) & 0xff),A((color >> 24) & 0xff) {};
};

//=============================================================================
//�R���X�g���N�^
//=============================================================================
Font::Font() :Sprite(Sprite::LAYER_INTERFACE)
{

}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
Font::~Font()
{
	if (Texture != nullptr)
	{
		Texture->Release();
		Texture = nullptr;
	}
}
//=============================================================================
//�t�H���g�f�[�^�̏�����
//=============================================================================
void Font::Initialize(void)
{
	strcpy(FontData.faceName,"���˖��� Pro B");
	FontData.charSet = SHIFTJIS_CHARSET;
	FontData.fontHeight = 450;
	FontData.weight = 0;
	FontData.penSize = 5;
	FontData.edgeColor = D3DXCOLOR(0,0,0,1.0f);
	FontData.fillColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	FontData.quality = 3;
}
//=============================================================================
//����
//=============================================================================
Font* Font::Create(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str)
{
	Font* font = new Font;
	if (font == nullptr) { return nullptr; }

	font->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	font->_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	font->_Size = D3DXVECTOR3(size.x,size.y,1.0f);

	font->Texture = createOutlineFontTexture(Window::Instance()->Device(),str);

	return font;
}
//=============================================================================
// �����t�H���g����
//=============================================================================
Font* Font::CreateFast(const D3DXVECTOR2& pos,const D3DXVECTOR2& size,char* str)
{
	Font* font = new Font;
	if (font == nullptr) { return nullptr; }

	font->Texture = createFastFontTexture(Window::Instance()->Device(),font,_T(str));
	font->_Pos = D3DXVECTOR3(pos.x,pos.y,0);
	font->_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//Font->_Size += D3DXVECTOR3(size.x,size.y,1.0f);

	return font;
}
//=============================================================================
//�X�V
//=============================================================================
void Font::Update(void)
{

}
//=============================================================================
//�����e�N�X�`�����쐬
//=============================================================================
LPDIRECT3DTEXTURE9 Font::createOutlineFontTexture(IDirect3DDevice9 *dev,const char *str,RECT *info)
{
	struct Color {
		unsigned char A,R,G,B;
		unsigned color;
		Color(unsigned color) : color(color),B(color & 0xff),G((color >> 8) & 0xff),R((color >> 16) & 0xff),A((color >> 24) & 0xff) {}
	};
	Color cEdgeColor(FontData.edgeColor),cFillColor(FontData.fillColor);

	int penSize = FontData.penSize;
	penSize *= FontData.quality;
	IDirect3DTexture9 *tex = 0;

	// �f�o�C�X�R���e�L�X�g�쐬
	HDC hDC = GetDC(NULL);
	HDC memDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL,hDC);   // ������Ă�����

	// �t�H���g�쐬
	LOGFONTA lf = {};
	lf.lfHeight = FontData.fontHeight * FontData.quality;    // �g��T�C�Y��
	lf.lfCharSet = FontData.charSet;
	lf.lfWeight = FontData.weight;
	lf.lfQuality = ANTIALIASED_QUALITY;
	memcpy(lf.lfFaceName,FontData.faceName,strlen(FontData.faceName));
	HFONT hFont = CreateFontIndirectA(&lf);
	HFONT oldFont = (HFONT)SelectObject(memDC,hFont);     // �t�H���g��ݒ肵�Ȃ��ƃG���[�ɂȂ�܂�

	// BMP���쐬
	TEXTMETRICA tm;
	GLYPHMETRICS gm;
	MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	int len = IsDBCSLeadByte(str[0]) ? 2 : 1;
	UINT code = (len == 2 ? (unsigned char)str[0] << 8 | (unsigned char)str[1] : (unsigned char)str[0]);
	GetTextMetricsA(memDC,&tm);
	GetGlyphOutlineA(memDC,code,GGO_METRICS,&gm,0,0,&mat);

	RECT charRegion = {     // LT - RB
		gm.gmptGlyphOrigin.x - penSize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y - penSize / 2,
		gm.gmptGlyphOrigin.x + gm.gmBlackBoxX + penSize / 2,
		tm.tmAscent - gm.gmptGlyphOrigin.y + gm.gmBlackBoxY + penSize / 2
	};

	RECT charWH = { 0,0,(gm.gmBlackBoxX + penSize + FontData.quality - 1) / FontData.quality * FontData.quality,(gm.gmBlackBoxY + penSize + FontData.quality - 1) / FontData.quality * FontData.quality };
	int bmpW = charWH.right;
	int bmpH = charWH.bottom;
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = bmpW;
	bmpInfo.bmiHeader.biHeight = -bmpH;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	unsigned char *p = 0;
	HBITMAP hBitMap = CreateDIBSection(0,&bmpInfo,DIB_RGB_COLORS,(void**)&p,0,0);
	HBITMAP oldBMP = (HBITMAP)SelectObject(memDC,hBitMap);

	// BMP�w�i��F�ŏ�����
	HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0,0,255));
	FillRect(memDC,&charWH,bgBrush);
	DeleteObject(bgBrush);

	// �p�X��`��
	// �p�X�F�͗΁A�h�͐�
	HPEN   hPen = (HPEN)CreatePen(PS_SOLID,penSize,RGB(0,255,0));
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255,0,0));
	HPEN   oldPen = (HPEN)SelectObject(memDC,hPen);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC,hBrush);

	SetBkMode(memDC,TRANSPARENT);
	BeginPath(memDC);
	TextOutA(memDC,-charRegion.left,-charRegion.top,str,len);
	EndPath(memDC);
	StrokeAndFillPath(memDC);

	SelectObject(memDC,oldPen);
	SelectObject(memDC,oldBrush);
	SelectObject(memDC,oldFont);
	SelectObject(memDC,oldBMP);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(hFont);

	// DirectX�̃e�N�X�`����BMP�̐F�������g���t�H���g�����
	int texW = charWH.right / FontData.quality;
	int texH = charWH.bottom / FontData.quality;
	int q2 = FontData.quality * FontData.quality;
	
	Mutex::Instance()->Enter();
	dev->CreateTexture(texW,texH,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,0);
	Mutex::Instance()->Leave();
	
	D3DLOCKED_RECT lockR;
	if (SUCCEEDED(tex->LockRect(0,&lockR,0,0))) {
		char *d = (char*)lockR.pBits;
		unsigned BMPPitch = (charWH.right * 3 + 3) / 4 * 4;
		for (int y = 0; y < texH; y++) {
			for (int x = 0; x < texW; x++) {
				unsigned &v = *((unsigned*)d + x + y * texW);   // �e�N�X�`���̃s�N�Z���ʒu
				unsigned alph = 0;
				unsigned edge = 0;
				unsigned fill = 0;
				// quality�{�����_�𕽋ω�
				for (int i = 0; i < FontData.quality; i++) {
					for (int j = 0; j < FontData.quality; j++) {
						alph += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 0];
						edge += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 1];
						fill += p[(y * FontData.quality + i) * BMPPitch + (x * FontData.quality + j) * 3 + 2];
					}
				}
				alph /= q2;
				edge /= q2;
				fill /= q2;

				// ���ߓx������ꍇ�̓G�b�W�F���̗p
				// �s�����̏ꍇ�̓u�����h�F���̗p
				unsigned a = 0xff - alph;
				if (a < 0xff) {
					// ������
					unsigned r = cEdgeColor.R;
					unsigned g = cEdgeColor.G;
					unsigned b = cEdgeColor.B;
					a = (a * cEdgeColor.A) >> 8;
					v = a << 24 | r << 16 | g << 8 | b;
				}
				else {
					// �s����
					unsigned r = ((cEdgeColor.R * edge) >> 8) + ((cFillColor.R * fill) >> 8);
					unsigned g = ((cEdgeColor.G * edge) >> 8) + ((cFillColor.G * fill) >> 8);
					unsigned b = ((cEdgeColor.B * edge) >> 8) + ((cFillColor.B * fill) >> 8);
					a = ((cEdgeColor.A * edge) >> 8) + ((cFillColor.A * fill) >> 8);
					v = a << 24 | r << 16 | g << 8 | b;
				}
			}
		}
		tex->UnlockRect(0);
	}

	DeleteObject(hBitMap);
	DeleteDC(memDC);

	if (info) {
		info->left = charRegion.left / FontData.quality;
		info->top = charRegion.top / FontData.quality;
		info->right = (gm.gmCellIncX + penSize) / FontData.quality;
		info->bottom = (tm.tmHeight + penSize) / FontData.quality;
	}
	return tex;
}

LPDIRECT3DTEXTURE9 Font::createFastFontTexture(IDirect3DDevice9 *dev,Font* instance,const char *str)
{
	IDirect3DTexture9 *tex = 0;

	HFONT hFont = CreateFontIndirect(&FastFont);
	if (hFont == nullptr)
	{
		MessageBox(nullptr,"�����t�H���g�̃n���h���̐����Ɏ��s���܂���","ERROR!",MB_OK | MB_ICONERROR);
		return nullptr; 
	}

	//�f�o�C�X�Ƀt�H���g����������
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc,hFont);

	wchar_t c[32] = L"test";

	//�t�H���g�r�b�g�}�b�v���擾
	int len = MultiByteToWideChar(CP_ACP,0,str,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,str,-1,c,len);

	UINT code = c[0];

	
	const int gradFlag = GGO_GRAY8_BITMAP;//GRAY2��GRAY8������

	int grad = 0;//�K���̍ő�l

	switch (gradFlag)
	{
	case GGO_GRAY2_BITMAP:
		grad = 4;
		break;
	case GGO_GRAY4_BITMAP:
		grad = 16;
		break;
	case GGO_GRAY8_BITMAP:
		grad = 64;
		break;
	default:
		break;
	}
	if (grad == 0)
	{
		MessageBox(nullptr,"�����t�H���g�̊K�����C���M�����[�ł�","ERROR!",MB_OK|MB_ICONERROR);
		return nullptr;
	}

	TEXTMETRIC textMetric;
	GetTextMetrics(hdc,&textMetric);
	GLYPHMETRICS glyphMetric;
	CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{0,1} };
	DWORD size = GetGlyphOutlineW(hdc,code,gradFlag,&glyphMetric,0,NULL,&mat);
	BYTE *pMono = new BYTE[size];
	GetGlyphOutlineW(hdc,code,gradFlag,&glyphMetric,size,pMono,&mat);

	//�f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h�����J��
	SelectObject(hdc,oldFont);
	ReleaseDC(NULL,hdc);

	//�e�N�X�`���̍쐬
	D3DLOCKED_RECT lockedRect;

	int width = (glyphMetric.gmBlackBoxX + 3) / 4 * 4;
	int height = glyphMetric.gmBlackBoxY;

	Mutex::Instance()->Enter();
	dev->CreateTexture((UINT)width,(UINT)height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&tex,NULL);
	Mutex::Instance()->Leave();

	tex->LockRect(0,&lockedRect,NULL,0);//���b�N
	DWORD *TexBuffer = (DWORD*)lockedRect.pBits;//�e�N�X�`���������ւ̃|�C���^���擾

	

	for(int y = 0;y < height;y++)
	{
		for (int x = 0;x < width;x++)
		{
			DWORD alpha = pMono[y*width + x]*255/grad;
			TexBuffer[y*width + x] = (alpha << 24) | 0x00ffffff;
		}
	}

	tex->UnlockRect(0);//�A�����b�N
	delete[] pMono;

	instance->SetSize(D3DXVECTOR3((float)width,(float)height,1.0f));
	instance->SetOffsetPos(D3DXVECTOR2((float)glyphMetric.gmptGlyphOrigin.x,(float)-glyphMetric.gmptGlyphOrigin.y));

	return tex;
}
//=============================================================================
//�t�H���g��ύX
//=============================================================================
void Font::SetFaceName(char faceName[])
{
	strcpy(FontData.faceName,faceName);
}

void Font::SetFastFontFacename(char name[])
{
	strcpy(FastFont.lfFaceName,name);

}

//====================================================
//String�N���X
//====================================================
String* String::Top = nullptr;
String* String::Cur = nullptr;
D3DXMATRIX String::SclMtx;
D3DXMATRIX String::Identity;

const char* HarfHeightChar[] = 
{
	"�`",
	"�[",
	"��",
};
const char* HarfSizeChar[] =
{
	"��","��","��","��","��",
	"��","��","��","��","��",
	"�@","�B","�D","�F","�H",
	"��","��","��","��","�b","�E",
};
String::String()
{

	Char = nullptr;
	_CommitMatrix = false;
	CharRotZ = nullptr;
	LinkList();
}

String::~String()
{
	UnlinkList();
}

//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void String::LinkList(void)
{
	if (Top != NULL)//��ڈȍ~�̏���
	{
		String* Polygon = Cur;
		Polygon->Next = this;
		Prev = Polygon;
		Next = NULL;
		Cur = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
	
}
//=============================================================================
//���g�����X�g����폜
//=============================================================================
void String::UnlinkList(void)
{
	if (Prev == NULL)//�擪
	{
		if (Next != NULL)//��������
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//�I�[
	{
		if (Prev != NULL)//�O������
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;

}

//=============================================================================
//������
//=============================================================================
void String::Initialize(void)
{
	D3DXMatrixIdentity(&Identity);
	D3DXMatrixScaling(&SclMtx,1.0f,1.0f,1.0f);
	Font::Initialize();
}

//=============================================================================
//����
//=============================================================================
String* String::Create(const D3DXVECTOR2& pos,float size,char* str)
{
	String* string = new String;
	if (string == nullptr){ return nullptr; }

	int num = strlen(str);
	char String[256] = { '\0' };
	strcpy(String,str);
	float offset = 0;
	for (int cnt = 0;cnt < num;)
	{//���������邩�����違�S�̂̒��������߂�
		int len = IsDBCSLeadByte(str[cnt]) ? 2 : 1;
		if ((len == 1 && islower((int)(str[cnt]))) || CheckHarfSize(&str[cnt]))
		{
			offset += size / 1.5f;
		}
		else
		{
			offset += size;
		}
		string->CharNum++;
		cnt += len;
	}
	string->Char = new Font*[string->CharNum];
	string->_Pos = pos;
	string->_RotZ = 0;
	string->_Color = 
	string->_MaskColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	string->_Length = offset;

	float charSize = size;//�������m���G��Ȃ��悤�ɏ����傫�߂̃I�t�Z�b�g�l�ɐݒ肷��

	float offsetX = -(offset/2.5f);//�S���̔����̃}�C�i�X�l���ŏ��̕����̃I�t�Z�b�g�ɐݒ�

	int len = 0;
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		//�������쐬
		len = IsDBCSLeadByte(String[charNum]) ? 2 : 1;//2�o�C�g�����H
		if (len == 1 && islower((int)(String[charNum])))
		{//���p�p���ŏ�������������
			if (cnt != 0){ offsetX += charSize/1.5f; }

			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y + size / 4.0f),D3DXVECTOR2(size / 2.0f,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,size / 4.0f));
		}
		else if (CheckHarfSize(&String[charNum]))
		{
			if (cnt != 0){ offsetX += charSize / 1.5f; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y + size / 4.0f),D3DXVECTOR2(size / 2.0f,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,size / 4.0f));
		}
		else if (CheckHarfHeight(&String[charNum]))
		{//�����𔼕��ɂ���K�v�̂��镶���̏ꍇ
			if (cnt != 0){ offsetX += charSize; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y),D3DXVECTOR2(size,size / 2.0f),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,0));
		}
		else
		{
			if (cnt != 0){ offsetX += charSize; }
			string->Char[cnt] = Font::Create(D3DXVECTOR2(pos.x + offsetX,pos.y),D3DXVECTOR2(size,size),&String[charNum]);
			string->Char[cnt]->SetOffsetPos(D3DXVECTOR2(offsetX,0));
		}
		charNum += len;
	}
	string->Init();
	return string;

}

String* String::CreateFast(const D3DXVECTOR2& pos,float size,char* str)
{
	String* string = new String;
	if (string == nullptr){ return nullptr; }

	int num = strlen(str);
	char String[256] = { '\0' };
	strcpy(String,str);
	
	for (int cnt = 0;cnt < num;)
	{//���������邩������
		int len = IsDBCSLeadByte(str[cnt]) ? 2 : 1;
		string->CharNum++;
		cnt += len;
	}
	string->Char = new Font*[string->CharNum];
	string->_Pos = pos;
	string->_RotZ = 0;
	string->_Color =
	string->_MaskColor = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	Font::SetFastFontSize((LONG)size);

	float charSize = size;//�������m���G��Ȃ��悤�ɏ����傫�߂̃I�t�Z�b�g�l�ɐݒ肷��

	float offset = 0;
	int len = 0;
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		//�������쐬
		len = IsDBCSLeadByte(String[charNum]) ? 2 : 1;//2�o�C�g�����H
		string->Char[cnt] = Font::CreateFast(D3DXVECTOR2(pos.x,pos.y),D3DXVECTOR2(size,size),&String[charNum]);
		float Csize = string->Char[cnt]->Size().x;
		offset += (Csize<charSize / 2.0f) ? charSize / 2.0f : Csize;
		string->Char[cnt]->AddPosY(string->Char[cnt]->OffsetPos().y);
		
		charNum += len;
	}
	string->_Length = offset;
	float offsetX = -(offset*0.5f);
	for (int cnt = 0,charNum = 0;charNum < num && cnt < string->CharNum;cnt++)
	{
		string->Char[cnt]->AddPosX(offsetX);
		string->Char[cnt]->AddOffsetPos(D3DXVECTOR2(offsetX,size/2.0f));
		float Csize = string->Char[cnt]->Size().x;
		offsetX += (Csize<charSize / 2.0f) ? charSize / 2.0f : Csize;
		charNum += len;
	}
	string->Init();
	return string;

}
//=============================================================================
//�ʏ�����
//=============================================================================
void String::Init(void)
{
	//���ꂼ��̕����̉�]��ۑ������邽�߂̕ϐ���p��
	CharRotZ = new float[CharNum];
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		CharRotZ[cnt] = 0;
	}
}
//=============================================================================
//���ׂčX�V
//=============================================================================
void String::UpdateAll(void)
{
	String* string = Top;
	String* next = nullptr;

	while (string)
	{
		next = string->Next;

		string->Update();

		string = next;
	}
}
//=============================================================================
//���ׂă����[�X
//=============================================================================
void String::ReleaseAll(void)
{
	String* string = Top;
	String* next = nullptr;

	while (string)
	{
		next = string->Next;

		string->Release();

		string = next;
	}
}

//=============================================================================
//�X�V
//=============================================================================
void String::Update(void)
{
	CastMatrix();
}

//=============================================================================
//�����̈ʒu���Čv�Z
//=============================================================================
void String::CastMatrix(void)
{//��������ړ��������]�������ꍇ�A�ʒu���Čv�Z����
	if (_CommitMatrix)
	{
		D3DXMATRIX WorldMatrix = Identity;
		D3DXMatrixRotationZ(&RotMtx,_RotZ);
		D3DXMatrixTranslation(&TransMtx,_Pos.x,_Pos.y,0);

		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&SclMtx);
		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&RotMtx);
		D3DXMatrixMultiply(&WorldMatrix,&WorldMatrix,&TransMtx);

		D3DXVECTOR3 pos;

		for (int cnt = 0;cnt < CharNum;cnt++)
		{
			pos = D3DXVECTOR3(Char[cnt]->OffsetPos().x,Char[cnt]->OffsetPos().y,0);
			D3DXVec3TransformCoord(&pos,&pos,&WorldMatrix);
			Char[cnt]->SetPos(pos);
			Char[cnt]->SetRot(_RotZ+CharRotZ[cnt]);
		}

		_CommitMatrix = false;

	}
}
//=============================================================================
//�F��ύX
//=============================================================================
void String::CommitColor(void)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetColor(_Color);
	}
}

//=============================================================================
//�F��ύX
//=============================================================================
void String::CommitMaskColor(void)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetMaskColor(_MaskColor);
	}
}
//=============================================================================
//������ς���ׂ������f
//=============================================================================
bool String::CheckHarfHeight(char* str)
{
	int num = sizeof(HarfHeightChar) / 4;
	for (int cnt = 0;cnt < num;cnt++)
	{
		if (HarfHeightChar[cnt][0] == str[0] && HarfHeightChar[cnt][1] == str[1])
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
//�T�C�Y��ς���ׂ������f
//=============================================================================
bool String::CheckHarfSize(char* str)
{
	int num = sizeof(HarfSizeChar) / 4;
	for (int cnt = 0;cnt < num;cnt++)
	{
		if (HarfSizeChar[cnt][0] == str[0] && HarfSizeChar[cnt][1] == str[1])
		{
			return true;
		}
	}
	return false;
}
//=============================================================================
//�J��
//=============================================================================
void String::Release(void)
{
	if (Char != nullptr)
	{
		for (int cnt = 0;cnt < CharNum;cnt++)
		{
			if (Char[cnt] != nullptr)
			{
				Char[cnt]->SetRelease();
				Char[cnt] = nullptr;
			}
		}
		delete[] Char;
	}

	if (CharRotZ != nullptr)
	{
		delete[] CharRotZ;
		CharRotZ = nullptr;
	}

	delete this;
}
//=============================================================================
//�ꕶ�����Ƃ̉�]���Z�b�g
//=============================================================================
void String::SetCharRot(float rot)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetRot(rot);
		CharRotZ[cnt] = rot;
	}
}//=============================================================================
//�ꕶ���̉�]���Z�b�g
//=============================================================================
void String::SetCharRot(float rot,int index)
{
	if (index >= 0 && index < CharNum)
	{
		Char[index]->SetRot(rot);
		CharRotZ[index] = rot;
	}
}
//=============================================================================
//�ꕶ�����Ƃ̉�]�����Z
//=============================================================================
void String::AddCharRot(float rot)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->AddRot(rot);
		CharRotZ[cnt] += rot;
	}
}
//=============================================================================
//�ꕶ���̉�]�����Z
//=============================================================================
void String::AddCharRot(float rot,int index)
{
	if (index >= 0 && index < CharNum)
	{
		Char[index]->AddRot(rot);
		CharRotZ[index] += rot;
	}
}

void String::SetMask(LPDIRECT3DTEXTURE9 mask)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetMask(mask);
	}
}

void String::SetPass(short pass)
{
	for (int cnt = 0;cnt < CharNum;cnt++)
	{
		Char[cnt]->SetPass((int)pass);
	}
}