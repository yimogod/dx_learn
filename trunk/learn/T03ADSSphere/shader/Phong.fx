/*�����*/
struct DirectionLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 direction;
};

/*���Դ*/
struct PointLight{
	float4 color;
	float4 worldPos;
};

void computeDirectionLight(float4 color,
	DirectionLight light, float3 pixelNormal, float3 toEyeW,
	out float4 ambient, out float4 diffuse, out float4 spec){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = color * light.ambientColor;

	float3 lightVec = -light.direction.xyz;
	float diffuseFactor = dot(lightVec, pixelNormal);
	//[flatten]
	if(diffuseFactor > 0){
		diffuse = diffuseFactor * light.diffuseColor * color;

		float3 v = reflect(-lightVec, pixelNormal);
		float specFactor = max(0.0f, dot(v, toEyeW));
		spec = specFactor * light.specularColor * color;
	}
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbTransform : register(b0){
	matrix view;
	matrix perspective;
}

cbuffer cbPhong : register(b1){
	float4 eyePosW;

	DirectionLight directionLight;
	PointLight pointLight;
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
	float3 normalW = normalize(input.normalW).xyz;
	float3 toEyeW = normalize(eyePosW - input.posW).xyz;

	float4 color = txDiffuse.Sample(samLinear, input.tex);

	float4 ac = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 dc = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float sc = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;
	computeDirectionLight(color, directionLight, normalW, toEyeW, A, D, S);
	ac += A;
	dc += D;
	sc += S;

	float4 lit = ac + dc + sc;
	lit.a = color.a;

	return lit;
}
