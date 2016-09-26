Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 insPos : TEXCOORD1;
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;

	input.pos.x += input.insPos.x;
	input.pos.y += input.insPos.y;
	input.pos.z += input.insPos.z;

	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;
	output.tex = input.tex;

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 col = txDiffuse.Sample(samLinear, input.tex);
	col.a = input.color.a;
	return col;
}

