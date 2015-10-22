Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix View;
	matrix Perspective;
}

struct VS_INPUT{
	float4 Pos : POSITION;
	float4 color : COLOR;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Perspective);

	output.color = input.color;
	output.Tex = input.Tex;

	return output;
}


float4 PS(PS_INPUT input) : SV_Target{
	return txDiffuse.Sample(samLinear, input.Tex);
}
