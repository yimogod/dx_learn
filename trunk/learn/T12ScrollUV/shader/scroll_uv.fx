Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix view;
	matrix perspective;
}

cbuffer ScrollBuffer : register(b1){
	float scroll;
}

cbuffer FadeBuffer : register(b2){
	float fade;
}

struct VS_INPUT{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = float4(1, 1, 1, 1);
	output.color.a -= fade;
	output.tex = input.tex;
	output.tex.x += scroll;

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 col = txDiffuse.Sample(samLinear, input.tex);
	col.a = input.color.a;
	return col;
}

