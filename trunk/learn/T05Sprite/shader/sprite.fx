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
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);
	//output.pos = input.pos;
	output.color = input.color;
	output.tex = input.tex;

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 col = txDiffuse.Sample(samLinear, input.tex) * input.color;
	return col;
}

