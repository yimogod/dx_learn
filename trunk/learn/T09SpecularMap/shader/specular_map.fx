/*贴图*/
Texture2D txDiffuse[3] : register(t0);
SamplerState samLinear : register(s0);

/*方向光*/
struct DirectionLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 direction;
};

/*顶点输入*/
struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float4 normal: NORMAL;
	float4 tangent: TANGENT;
};

/*像素输入*/
struct PS_INPUT{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float2 tex : TEXCOORD0;
	float3 normalW: NORMAL;
	float3 tangentW: TANGENT;
	float3 binormalW: BINORMAL;
	float3 viewDirection : TEXCOORD1;
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
}

void computeDirectionLight(float4 textColor, float4 specColor, DirectionLight light,
	float3 pixelNormal, float3 toEyeW,
	out float4 ambient, out float4 diffuse, out float4 spec){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	ambient = textColor * light.ambientColor * light.ambientColor.a;

	float3 lightVec = -light.direction;
	float diffuseFactor = dot(lightVec, pixelNormal);
	if(diffuseFactor > 0.0f){
		diffuse = diffuseFactor * textColor * light.diffuseColor;

		float3 v = reflect(-lightVec, pixelNormal);
		float specFactor = max(0.0f, dot(v, toEyeW));
		specFactor = pow(specFactor, 1.0f);

		spec = specFactor * specColor * textColor;
	}
}

PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.posW = mul(input.pos, model);
	output.posH = mul(output.posW, view);
	output.posH = mul(output.posH, perspective);

	output.tex = input.tex;

	output.normalW = normalize(mul(input.normal, model)).xyz;
	output.tangentW = normalize(mul(input.tangent, model)).xyz;
	output.binormalW = normalize(cross(output.normalW, output.tangentW));

	output.viewDirection = normalize(eyePosW.xyz - output.posW.xyz);
	return output;
}

float4 PS(PS_INPUT input) :SV_Target{
	float4 color = txDiffuse[0].Sample(samLinear, input.tex);
	float4 ncolor = txDiffuse[1].Sample(samLinear, input.tex);
	ncolor = (ncolor * 2.0f) - 1.0f;
	float4 scolor = txDiffuse[2].Sample(samLinear, input.tex);

	float3 bumpNormal = ncolor.x * input.tangentW +
						ncolor.y * input.binormalW +
						ncolor.z * input.normalW;

	float4 ac = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 dc = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 sc = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 A, D, S;
	computeDirectionLight(color, scolor, directionLight, bumpNormal, input.viewDirection, A, D, S);
	ac += A;
	dc += D;
	sc += S;

	float4 lit = ac + dc + sc;
	return lit;
}
