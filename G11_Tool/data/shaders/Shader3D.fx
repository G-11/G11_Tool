static float4x4 WorldMtx;
float4x4 Projection;
float4x4 ViewMtx;
float4x4 InvViewMtx;
float4x4 ViewPort;
float4x4 Scl;
float4x4 Rot;
float4x4 Pos;
float3 LightVec;
float4 Ambient = float4(0.01f,0.01f,0.05f,1.0f);
float4 LightColor;
float4 Color;
float3 Size;
float4 uv;//left,top,width,height

texture DiffuseMap;
texture MaskMap;

//�e�N�X�`���T���v���[
sampler diffuseSampler = sampler_state {
	Texture = <DiffuseMap>;

	//MinFilter = ANISOTROPIC;
	MinFilter = LINEAR;
	//MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

//�}�X�N�T���v���[
sampler maskSampler = sampler_state {
	Texture = <MaskMap>;

	//MinFilter = ANISOTROPIC;
	MinFilter = LINEAR;
	//MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_IN
{
	float3 pos : POSITION0;
	float3 nor : NORMAL0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

//�ϊ��}�g���b�N�X�̍쐬
float4 CreateWVP(vector pos)
{
	pos.x *= Size.x;
	pos.y *= Size.y;
	WorldMtx = mul(Scl,Rot);
	WorldMtx = mul(WorldMtx,Pos);
	float4 Out;
	Out = mul(pos,WorldMtx);
	Out = mul(Out,ViewMtx);
	Out = mul(Out,Projection);
	
	return Out;
}

//�r���{�[�h�}�g���b�N�X�ϊ�
float4 BillboardWVP(vector pos)
{
	pos.x *= Size.x;
	pos.y *= Size.y;
	WorldMtx = mul(Scl,Rot);
	WorldMtx = mul(WorldMtx,Pos);
	float4 Out;
	WorldMtx = mul(WorldMtx,InvViewMtx);
	Out = mul(pos,WorldMtx);
	Out = mul(Out,ViewMtx);
	Out = mul(Out,Projection);

	return Out;
}

//=========================================================
//���_�V�F�[�_�[
//=========================================================
//=========================================================
//�W��(�n�[�t�����o�[�g)
//=========================================================
VS_OUT VS_Normal(VS_IN Input)
{
	VS_OUT Output;

	float4 Out;
	Out = CreateWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Input.nor = mul(Input.nor,WorldMtx).xyz;
	float vec = max(0.0f,dot(Input.nor,LightVec.xyz));//�@���ƃ��C�g�̃x�N�g���̓��ς��Ƃ�
	float p = dot(Input.nor,LightVec.xyz);
	p = p*0.5f + 0.5f;
	p = p * p;
	Output.color = float4(Color.rgb*vec*p,Color.a);//�@����F�Ɋ|����
	Output.color.rgb += Ambient.rgb;//������
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);
	
	return Output;
}

//=========================================================
//���C�e�B���O�Ȃ�
//=========================================================

VS_OUT VS_NoLight(VS_IN Input)
{
	VS_OUT Output;

	float4 Out;
	Out = CreateWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Output.color = Color;
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);

	return Output;
}

//=========================================================
//�r���{�[�h�ϊ�
//=========================================================
VS_OUT VS_Billboard(VS_IN Input)
{
	VS_OUT Output;
	float4 Out;
	Out = BillboardWVP(float4(Input.pos,1.0f));

	Output.pos = Out;
	Output.color = Color;
	Output.uv = Input.uv*float2(uv.z,uv.w) + float2(uv.x,uv.y);

	return Output;
}

//=========================================================
//�s�N�Z���V�F�[�_�[
//=========================================================
//=========================================================
// �W��
//=========================================================
float4 PS_Normal(VS_OUT Input) : COLOR0
{
	return tex2D(diffuseSampler,Input.uv)*Input.color*LightColor;
}

//=========================================================
//���C�g����
//=========================================================
float4 PS_NoLight(VS_OUT Input) : COLOR0
{
	return tex2D(diffuseSampler,Input.uv)*Input.color;
}

//=========================================================
//�e�N�j�b�N
//=========================================================
technique Polygon3D
{
	//�ʏ�
	pass Normal
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS_Normal();
		PixelShader = compile ps_2_0 PS_Normal();
	}

	//���Z
	pass Add
	{
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_NoLight();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//���Z
	pass Sub
	{
		BlendOp = REVSUBTRACT;
		DestBlend = ONE;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Normal();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//�ʏ�
	pass Nolight
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_NoLight();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//---------------------------------------------------------
	//�r���{�[�h
	//---------------------------------------------------------
	pass Billboard
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//���Z
	pass AddEffect
	{
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

	//���Z
	pass SubEffect
	{
		BlendOp = REVSUBTRACT;
		DestBlend = ONE;
		SrcBlend = SRCALPHA;
		CullMode = NONE;
		ZEnable = true;
		ZWriteEnable = false;

		VertexShader = compile vs_2_0 VS_Billboard();
		PixelShader = compile ps_2_0 PS_NoLight();
	}

};