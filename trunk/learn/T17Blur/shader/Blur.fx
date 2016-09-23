Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

cbuffer ScreenSizeBuffer : register(b1){
	float screenWidth;
	float screenHeight;
	float pad0;
	float pad1;
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
	float2 tex : TEXCOORD0;

	float2 tex1 : TEXCOORD1; //top
	float2 tex2 : TEXCOORD2; //right
	float2 tex0 : TEXCOORD3; //center
	float2 tex3 : TEXCOORD4; //down
	float2 tex4 : TEXCOORD5; //left
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);
	output.tex = input.tex;

	float texelWidth = 1.0f / screenWidth;
	float texelHeight = 1.0f / screenHeight;

	output.tex0 = input.tex + float2(0.0f, 0.0f);
	output.tex1 = input.tex + float2(0.0f, -texelHeight);
	output.tex3 = input.tex + float2(0.0f, texelHeight);

	output.tex4 = input.tex + float2(0.0f, -texelWidth);
	output.tex2 = input.tex + float2(0.0f, texelWidth);

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	color += txDiffuse.Sample(samLinear, input.tex0);
	color += txDiffuse.Sample(samLinear, input.tex1);
	color += txDiffuse.Sample(samLinear, input.tex2);
	color += txDiffuse.Sample(samLinear, input.tex3);
	color += txDiffuse.Sample(samLinear, input.tex4);

	color *= 0.2f;
	color.a = 1.0f;
	return color;
}
