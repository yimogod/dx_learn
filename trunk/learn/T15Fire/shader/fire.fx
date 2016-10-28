Texture2D fireTexture : register(t0);
Texture2D noiseTexture : register(t1);
Texture2D alphaTexture : register(t2);
SamplerState samWrap : register(s0);
SamplerState samClamp : register(s1);

cbuffer ConstantBuffer : register(b0){
	matrix model;
	matrix view;
	matrix perspective;
};

cbuffer NoiseBuffer : register(b1){
	float frameTime;
	float3 scrollSpeeds;
	float3 scales;
	float padding;
};

cbuffer DistortionBuffer : register(b2){
	float2 distortion1;
	float2 distortion2;
	float2 distortion3;
	float distortionScale;
	float distortionBias;
};

struct VS_INPUT{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float2 texCoords1 : TEXCOORD1;
	float2 texCoords2 : TEXCOORD2;
	float2 texCoords3 : TEXCOORD3;
};

//火焰的标准算法
//需要一张火焰图片, 一张扭曲图片, 一张透明图片
PS_INPUT VS(VS_INPUT input){
	PS_INPUT output = (PS_INPUT)0;
	output.pos = mul(input.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, perspective);

	output.color = input.color;
	output.tex = input.tex;

	//3张noise图片的滚动
	output.texCoords1 = (input.tex * scales.x);
	output.texCoords1.y = output.texCoords1.y + (frameTime * scrollSpeeds.x);

	output.texCoords2 = (input.tex * scales.y);
	output.texCoords2.y = output.texCoords2.y + (frameTime * scrollSpeeds.y);

	output.texCoords3 = (input.tex * scales.z);
	output.texCoords3.y = output.texCoords3.y + (frameTime * scrollSpeeds.z);

	return output;
}


float4 PS(PS_INPUT input):SV_Target{
	float4 noise1 = noiseTexture.Sample(samWrap, input.texCoords1);
	float4 noise2 = noiseTexture.Sample(samWrap, input.texCoords2);
	float4 noise3 = noiseTexture.Sample(samWrap, input.texCoords3);

	// Move the noise from the (0, 1) range to the (-1, +1) range.
	noise1 = (noise1 - 0.5f) * 2.0f;
	noise2 = (noise2 - 0.5f) * 2.0f;
	noise3 = (noise3 - 0.5f) * 2.0f;

	//扭曲三张噪声
	noise1.xy = noise1.xy * distortion1.xy;
	noise2.xy = noise2.xy * distortion2.xy;
	noise3.xy = noise3.xy * distortion3.xy;

	float4 finalNoise = noise1 + noise2 + noise3;

	//perturb用来增强火焰闪烁
	float perturb = ((1.0f - input.tex.y) * distortionScale) + distortionBias;

	//最终的采样坐标
	float2 noiseCoords = (finalNoise.xy * perturb) + input.tex.xy;

	float4 col = fireTexture.Sample(samClamp, noiseCoords);
	float4 alphaColor = alphaTexture.Sample(samClamp, noiseCoords);
	col.a = alphaColor.r;
	return col;
}

