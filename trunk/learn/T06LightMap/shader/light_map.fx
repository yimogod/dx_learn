Texture2D txDiffuse[2] : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
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
	output.pos = input.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;
	output.tex = input.tex;

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 col = txDiffuse[0].Sample(samLinear, input.tex);
	float4 lightCol = txDiffuse[1].Sample(samLinear, input.tex);
	float4 finalColor = col * lightCol;

	//clip(finalColor.a - 0.2f);

	return finalColor;
}

