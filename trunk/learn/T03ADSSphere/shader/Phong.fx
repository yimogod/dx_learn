/*方向光*/
struct DirectionLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 direction;
};

/*点光源*/
struct PointLight{
	float4 color;
	float4 worldPos;
};

void computeDirectionLight(float4 color, DirectionLight light, float4 pixelNormal,
	out float4 ambient, out float4 diffuse){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = color * light.ambientColor;

	float3 lightVec = -light.direction.xyz;
	float3 normalVec = pixelNormal.xyz;
	float diffuseFactor = dot(lightVec, normalVec);
	//[flatten]
	if(diffuseFactor > 0){
		diffuse = diffuseFactor * light.diffuseColor * color;
	}
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbTransform : register(b0){
	matrix view;
	matrix perspective;
}

cbuffer cbPhong : register(b1){
	DirectionLight directionLight;
	PointLight pointLight;

	float4 eyeWorldPos;
}

struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4 normal: NORMAL;
};

struct PS_INPUT{
	float4 posP : SV_POSITION;
	float4 posW : POSITION;
	float2 tex : TEXCOORD0;
	float4 normalW: NORMAL;
};

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.posP = input.pos;
	output.posP = mul(output.posP, view);
	output.posP = mul(output.posP, perspective);

	output.posW = input.pos;
	output.tex = input.tex;
	output.normalW = input.normal;

	return output;
}

float4 PS(PS_INPUT input) :SV_Target{
	input.normalW = normalize(input.normalW);
	
	float4 color = txDiffuse.Sample(samLinear, input.tex);

	float4 ac = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 dc = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D;
	computeDirectionLight(color, directionLight, input.normalW, A, D);
	ac += A;
	dc += D;

	float4 lit = ac + dc;
	lit.a = color.a;

	return lit;
}
