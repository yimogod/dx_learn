/*��ͼ*/
Texture2D txDiffuse : register(t0);
Texture2D depthMapTexture : register(t1);
SamplerState samLinear : register(s0);

/*�����*/
struct DirectionLight{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float4 direction;
};

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
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float2 tex : TEXCOORD0;
	float4 normalW: NORMAL;
	float4 lightSpacePos : TEXCOORD1;
	float4 lightDeltaPos : TEXCOORD2;
};

//�ռ�ת��vs
cbuffer cbTransform : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
}

//vs, b�����˵Ƶ�λ��
cbuffer cbLightSpace : register(b1){
	matrix lightView;
	matrix lightPerspective;
}

//ps ads��Ϣ
cbuffer cbPhong : register(b2){
	float4 eyePosW;
	DirectionLight directionLight;
}


void ComputeDirectionLight(float4 textColor, DirectionLight light,
	float3 pixelNormal, float3 toEyeW,
	out float4 ambient, out float4 diffuse, out float4 spec){
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	ambient = textColor * light.ambientColor * light.ambientColor.a;

	float3 lightVec = -light.direction.xyz;
	float diffuseFactor = dot(lightVec, pixelNormal);
	if(diffuseFactor > 0.0f){
		diffuse = diffuseFactor * textColor * light.diffuseColor * light.diffuseColor.a;

		float3 v = reflect(-lightVec, pixelNormal);
		float specFactor = max(0.0f, dot(v, toEyeW));
		specFactor = pow(specFactor, 1.0f);
		spec = specFactor * textColor * light.specularColor;
	}
}


PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	float4 worldPos = mul(input.pos, model);

	output.posW = worldPos;
	output.posH = mul(output.posW, view);
	output.posH = mul(output.posH, perspective);
	
	output.tex = input.tex;

	output.normalW = mul(input.normal, model);
	output.normalW = normalize(output.normalW);

	output.lightSpacePos = worldPos;
	output.lightSpacePos = mul(output.lightSpacePos, lightView);
	output.lightSpacePos = mul(output.lightSpacePos, lightPerspective);

	return output;
}

float4 PS(PS_INPUT input) :SV_Target{
	float3 normalW = normalize(input.normalW).xyz;
	float3 toEyeW = normalize(eyePosW - input.posW).xyz;
	float3 posW = input.posW.xyz;
	float4 color = float4(0.0f, 0, 0, 1.0f);

	//ת����lightspace�������-1~1 ��0~1(��ͼ�ռ�)
	float2 projectTexCoord;
	projectTexCoord.x = input.lightSpacePos.x / input.lightSpacePos.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightSpacePos.y / input.lightSpacePos.w / 2.0f + 0.5f;


	//lightspace�����������, �����ڵ�view space�Ƿ�ɼ�
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		//��ǰ���ؾ���zbuffer������ֵ, �п����������㸲����������ֵ
		float depthValue = depthMapTexture.Sample(samLinear, projectTexCoord).r;
		//��ǰpixel��light space����ʵ���, �п��ܻᱻ�������zbuffer����
		float lightDepthValue = input.lightSpacePos.z / input.lightSpacePos.w;
		lightDepthValue = lightDepthValue - 0.001f;

		//�й�
		if(lightDepthValue < depthValue){
			color = txDiffuse.Sample(samLinear, input.tex);

			float4 ac = float4(0.0f, 0.0f, 0.0f, 1.0f);
			float4 dc = float4(0.0f, 0.0f, 0.0f, 1.0f);
			float4 sc = float4(0.0f, 0.0f, 0.0f, 1.0f);

			float4 A, D, S;
			ComputeDirectionLight(color, directionLight, normalW, toEyeW, A, D, S);
			ac += A;
			dc += D;
			sc += S;

			color = ac + dc + sc;
		}
	}

	return color;
}
