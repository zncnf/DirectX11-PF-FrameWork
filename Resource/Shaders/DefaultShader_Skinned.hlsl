Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct vertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    #ifdef SKINNED
    uint4 boneids : BONEIDS0;
    float4 weights : WEIGHTS0;
    #endif
};

struct PixelInput
{
    float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 viewDirection : TEXCOORD1;
};

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};

cbuffer BoneBuffer : register(b2)
{
    matrix bones[100];
};

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

PixelInput VS(vertexInput Input)
{
    PixelInput pixelout = (PixelInput)0;

    matrix BoneTransform;
    
    #ifdef SKINNED
    BoneTransform = bones[Input.boneids[0]] * Input.weights[0];
    BoneTransform += bones[Input.boneids[1]] * Input.weights[1];
    BoneTransform += bones[Input.boneids[2]] * Input.weights[2];
    BoneTransform += bones[Input.boneids[3]] * Input.weights[3];
    #endif

    Input.position.w = 1.0f;

    pixelout.position = mul(Input.position, world);
    
    #ifdef SKINNED
    pixelout.position = mul(Input.position, BoneTransform);
    pixelout.position = mul(pixelout.position, world);
    #endif    

    pixelout.position = mul(pixelout.position, view);
    pixelout.position = mul(pixelout.position, proj);

    pixelout.uv = Input.uv;
    
    pixelout.normal = mul(Input.normal, (float3x3) world);
    pixelout.normal = normalize(pixelout.normal);
    
    float4 worldPosition = mul(Input.position, world);
    
    pixelout.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    pixelout.viewDirection = normalize(pixelout.viewDirection);
    
    return pixelout;
}

float4 PS(PixelInput input) : SV_Target
{
    //float4 textureColor;
    //float4 color;
    //float4 specular;
    //float3 lightDir;
    //float lightIntensity;

    //textureColor = shaderTexture.Sample(SampleType, input.uv);
    //color = ambientColor;
    //specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //lightDir = -lightDirection;
    //lightIntensity = saturate(dot(input.normal, lightDir));
    
    //if (lightIntensity > 0.0f)
    //{
    //    color += (diffuseColor * lightIntensity);
    //    color = saturate(color);
    //    float3 reflection = normalize(2 * lightIntensity * input.normal - lightDir);
    //    specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    //}
    
    //color = color * textureColor;
    //color = saturate(color + specular);

    float4 color = shaderTexture.Sample(SampleType, input.uv);
    return color;
}