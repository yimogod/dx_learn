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
	float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = input.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target{
	return input.color;
}
