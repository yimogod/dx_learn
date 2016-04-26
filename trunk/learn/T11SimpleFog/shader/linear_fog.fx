Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

cbuffer FogBuffer : register(b1){
	float fogMinDis;
	float fogMaxDis;
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
	float fogFactor : FOG;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;
	output.tex = input.tex;

	/*�������������ϵ��, �����λ�þ������,���Կ���ֱ����cameraPos.z������Ƚ��м���*/
	float4 cameraPos = mul(input.pos, model);
	cameraPos = mul(cameraPos, view);
	output.fogFactor = saturate((fogMaxDis - cameraPos.z) / (fogMaxDis - fogMinDis));

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 texCol = txDiffuse.Sample(samLinear, input.tex);
	float4 fogCol = float4(0.5f, 0.5f, 0.5f, 1.0f);
	float4 col = input.fogFactor * texCol + (1.0 - input.fogFactor) * fogCol;
	return col;
}
