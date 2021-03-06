SamplerState samLinear : register(s0);

/*�ռ�ת��*/
cbuffer cbTransform : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

/*��������*/
struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
};

/*��������*/
struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 depthPos : TEXCOORD0;
};


PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);
	
	output.depthPos = output.pos;
	return output;
}

float4 PS(PS_INPUT input) :SV_Target{
	float depth = input.depthPos.z / input.depthPos.w;
	float4 color = float4(depth, depth, depth, 1.0f);
	return color;
}
