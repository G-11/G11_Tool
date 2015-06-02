#ifndef _SHADER_2D_H_
#define _SHADER_2D_H_

#include "main.h"

//===================================================
//�QD�V�F�[�_�[�N���X
// �V���O���g��
//===================================================

class CShader2D
{
public:

	typedef enum
	{
		PROJECTION = 0,
		VIEW_PORT,
		VIEW_MTX,
		WORLD_MTX,
		SCL_MTX,
		ROT_MTX,
		POS_MTX,
		OFFSET,
		DIFFUSE,
		MASK_COLOR,
		UV,
		MASK_UV,
		TEXTURE,
		MASK,
		STATE_MAX
	}STATE;

	typedef enum
	{
		NORMAL = 0,
		ADD,
		SUB,
		ALPHA_MASK,
		COLOR_MASK,
		NEGA_MASK,
		SKY,
		PASS_MAX
	}PASS;

	HRESULT Init(void);
	void Uninit(void);

	void DrawBegin(void){Effect->Begin(&Pass,0); }
	void DrawEnd(void){ Effect->End(); }

	void BeginPass(PASS pass){ Effect->BeginPass(pass); }
	void EndPass(void){ Effect->End(); }

	void Commit(void){ Effect->CommitChanges(); }
	void Draw(PASS pass,_D3DPRIMITIVETYPE pt,int PrimitiveNum = 2,int offset = 0);

	void SetFloat(STATE param,float f)				{ Effect->SetFloat(State[param],f); }
	void SetFloatArray(STATE param,float* f,int num){ Effect->SetFloatArray(State[param],f,num); }
	void SetMatrix(STATE param,D3DXMATRIX mtx)		{ Effect->SetMatrix(State[param],&mtx); }
	void SetTexture(LPDIRECT3DTEXTURE9 tex)			{ Effect->SetTexture(State[TEXTURE],tex); }
	void SetMask(LPDIRECT3DTEXTURE9 tex)			{ Effect->SetTexture(State[MASK],tex); }
	void SetIdentity(CShader2D::STATE state)		{ Effect->SetMatrix(State[state],&MtxIdentity); }

	D3DXMATRIX Identity(void){ return MtxIdentity; }
	static CShader2D* Instance(void){ return Self; }

private:

	const static LPSTR ParamName[];

	CShader2D();
	CShader2D(const CShader2D& other){};
	CShader2D &operator=(const CShader2D& other){};

	static CShader2D* Self;
	D3DXHANDLE State[STATE_MAX];
	D3DXHANDLE Technique[PASS_MAX];
	LPD3DXEFFECT Effect;
	UINT Pass;
	D3DXMATRIX MtxIdentity;

};
#endif