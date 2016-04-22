Texture2D txDiffuse[3] : register(t0);
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
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;
	output.tex = input.tex;

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 col1 = txDiffuse[0].Sample(samLinear, input.tex);
	float4 col2 = txDiffuse[1].Sample(samLinear, input.tex);
	float alpha = txDiffuse[2].Sample(samLinear, input.tex).r;

	float4 blendColor = (alpha * col1) + (1.0f - alpha) * col2;
	blendColor = saturate(blendColor);

	return blendColor;
}

