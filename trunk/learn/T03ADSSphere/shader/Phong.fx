Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

/*方向光*/
struct DirectionLight{
	float4 color;
	float4 direction;
};

/*点光源*/
struct PointLight{
	float4 color;
	float4 worldPos;
};

cbuffer cbTransform : register(b0){
	matrix view;
	matrix perspective;
}

cbuffer cbPhong : register(b1){
	float4 ambient;

	DirectionLight directionLight;
	PointLight pointLight;

	float4 eyeWorldPos;
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


float4 PS(PS_INPUT input) :SV_Target{
	return txDiffuse.Sample(samLinear, input.tex);
}
