
float4x4 Projection2D;
float value;//�ėp�ϐ�

texture DiffuseMap;
texture MaskMap;
//�e�N�X�`���T���v���[
sampler diffuseSampler = sampler_state {
	Texture = <DiffuseMap>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

sampler maskSampler = sampler_state {
	Texture = <MaskMap>;

	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 16;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct Vs_in_2D
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct Vs_out
{
	float4 posH : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 PosL : TEXCOORD1;
};

//2D�m�[�}��
Vs_out VS(Vs_in_2D Input)
{
	float4 Out;
	Vs_out Output;
	Out = mul(float4(Input.pos,1.0f),Projection2D);

	Output.posH = Out;//mul(float4(Input.pos,1.0f),MatWVP);
	Output.color = float4(1.0f,1.0f,1.0f,1.0f);
	Output.uv = Input.uv;
	Output.PosL = Out.xyz;//mul(float4(Input.pos,1.0f),MatWVP);

	return Output;
}

//2D
float4 PS(Vs_out VInput) : COLOR
{
	float4 color = tex2D(diffuseSampler,VInput.uv)*VInput.color;
	color.a = min(1.0f,color.a*1.5f);
	return color;
}
//2D�l�K
float4 PS_Nega(Vs_out VInput) : COLOR
{
	float4 color = tex2D(diffuseSampler,VInput.uv)*VInput.color;
	color.r = 1.0f - color.r;
	color.g = 1.0f - color.g;
	color.b = 1.0f - color.b;
	return color;
}
//2D���m�N��
float4 PS_Mono(Vs_out VInput) : COLOR
{
	float4 color = tex2D(diffuseSampler,VInput.uv)*VInput.color;
	color.r = color.b = color.g = (color.r + color.b + color.g) / 3;
	return color;
}
//2D�Z�s�A
float4 PS_Sepia(Vs_out VInput) : COLOR
{
	float4 color = tex2D(diffuseSampler,VInput.uv)*VInput.color;
	float av = (color.r + color.b + color.g) / 3;
	float4 sepia = float4(0.42f,0.29f,0.17f,1.0f);
	color.r = sepia.r * av;
	color.g = sepia.g * av;
	color.b = sepia.b * av;
	return color;
}

float4 PS_Sky(Vs_out VInput) : COLOR
{
	float4 OutColor = tex2D(diffuseSampler,VInput.uv)*VInput.color;
	OutColor.rgb *= tex2D(maskSampler,float2(value,0.5f)).rgb;
	return OutColor;
}
technique Screen
{
	pass Normal
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}

	pass Nega
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Nega();
	}

	pass Mono
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Mono();
	}

	pass Sepia
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Sepia();
	}

	pass Sky
	{
		BlendOp = ADD;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		CullMode = CCW;
		ZEnable = true;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS_Sky();
	}
};