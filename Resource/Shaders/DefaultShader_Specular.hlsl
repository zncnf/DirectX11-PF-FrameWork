Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

PixelInputType LightVertexShader(VertexInputType input)
{   
    PixelInputType output = (PixelInputType)0;
    
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
    
	output.tex = input.tex;
    
	output.normal = mul(input.normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
    
    float4 worldPosition = mul(input.position, worldMatrix);
    
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);
    
	return output;
}

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float4 color;
    float4 specular;
	float3 lightDir;
	float lightIntensity;

	textureColor = shaderTexture.Sample(SampleType, input.tex);
    color = ambientColor;
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	lightDir = -lightDirection;
	lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        float3 reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }
    
	color = color * textureColor;
    color = saturate(color + specular);

	return color;
}