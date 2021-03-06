/*贴图*/
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

/*方向光*/
struct DirectionLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 direction;
};

/*点光源*/
struct PointLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 worldPos;
	float range;
	float3 attenuate;
};

/*顶点输入*/
struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4 normal: NORMAL;
};

/*像素输入*/
struct PS_INPUT{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float2 tex : TEXCOORD0;
	float4 normalW: NORMAL;
};

/*空间转换*/
cbuffer cbTransform : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

/*ads信息*/
cbuffer cbPhong : register(b1){
	float4 eyePosW;

	DirectionLight directionLight;
	PointLight pointLight;
}

void computeDirectionLight(float4 textColor, DirectionLight light,
	float3 pixelNormal, float3 toEyeW,
	out float4 ambient, out float4 diffuse, out float4 spec){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	ambient = textColor * light.ambientColor * light.ambientColor.a;

	float3 lightVec = -light.direction.xyz;
	float diffuseFactor = dot(lightVec, pixelNormal);
	if(diffuseFactor > 0.0f){
		diffuse = diffuseFactor * textColor * light.diffuseColor;

		float3 v = reflect(-lightVec, pixelNormal);
		float specFactor = max(0.0f, dot(v, toEyeW));
		specFactor = pow(specFactor, 1.0f);
		spec = specFactor * textColor * light.specularColor;
	}
}

void computePointLight(float4 textColor, PointLight light,
	float3 pixelPos, float3 pixelNormal, float3 toEyeW,
	out float4 ambient, out float4 diffuse, out float4 spec){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	ambient = textColor * light.ambientColor * light.ambientColor.a;

	float3 lightVec = light.worldPos.xyz - pixelPos;
	float d = length(lightVec);
	if(d > light.range)return;

	lightVec /= d;

	float diffuseFactor = dot(lightVec, pixelNormal);
	if(diffuseFactor > 0){
		diffuse = diffuseFactor * light.diffuseColor * textColor;

		float3 v = reflect(-lightVec, pixelNormal);
		float specFactor = max(0.0f, dot(v, toEyeW));
		spec = specFactor * light.specularColor * textColor;
	}

	float att = 1.0f / dot(light.attenuate, float3(1.0f, d, d * d));

	diffuse *= att;
	spec *= att;
}


PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.posW = mul(input.pos, model);
	output.posH = mul(output.posW, view);
	output.posH = mul(output.posH, perspective);
	
	output.tex = input.tex;

	output.normalW = mul(input.normal, model);
	output.normalW = normalize(output.normalW);
	return output;
}

float4 PS(PS_INPUT input) :SV_Target{
	float3 normalW = normalize(input.normalW).xyz;
	float3 toEyeW = normalize(eyePosW - input.posW).xyz;
	float3 posW = input.posW.xyz;

	float4 color = txDiffuse.Sample(samLinear, input.tex);

	float4 ac = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 dc = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 sc = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 A, D, S;
	computeDirectionLight(color, directionLight, normalW, toEyeW, A, D, S);
	ac += A;
	dc += D;
	sc += S;

	//computePointLight(color, pointLight, posW, normalW, toEyeW, A, D, S);
	//ac += A;
	//dc += D;
	//sc += S;

	float4 lit = ac + dc + sc;
	return lit;
}
