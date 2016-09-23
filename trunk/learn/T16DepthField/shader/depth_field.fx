SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

struct VS_INPUT{
	float4 pos : POSITION;
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 depthPos : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.depthPos = input.pos;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target{
	float depthValue = input.depthPos.z;
	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	// The next 0.025% portion of the depth buffer color green.
	if(depthValue > 0.9f)
		color = float4(0.0f, 1.0f, 0.0f, 1.0f);

	// The remainder of the depth buffer color blue.
	if(depthValue > 0.925f)
		color = float4(0.0f, 0.0f, 1.0f, 1.0f);

	return color;
}

