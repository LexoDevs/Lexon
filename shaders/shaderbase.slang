struct VSInput
{
    float3 inPosition : POSITION;
    float3 inNormal   : NORMAL;
    float2 inUv       : TEXCOORD0;
};

struct UniformBuffer
{
    float4x4 view;
    float4x4 proj;
};

struct ObjectPushConstants
{
    float4x4 model;
    float4x4 normalMatrix;
};

[[vk::push_constant]]
ConstantBuffer<ObjectPushConstants> objectData;

[[vk::binding(0, 0)]]
ConstantBuffer<UniformBuffer> ubo;

[[vk::binding(1, 0)]]
Sampler2D albedoTexture;

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
    float lighting  : TEXCOORD1;
};

[shader("vertex")]
VSOutput vertMain(VSInput input)
{
    VSOutput output;
    const float4 worldPosition = mul( objectData.model, float4(input.inPosition, 1.0));
    output.position = mul(
        ubo.proj,
        mul(
            ubo.view,
            worldPosition
        )
    );

    const float3 normal = normalize(mul(
            (float3x3)objectData.normalMatrix,
            input.inNormal
        ));
    const float3 lightDirection =
        normalize(float3(0.35, 0.75, 0.55));

    output.uv = input.inUv;
    output.lighting =
        0.15
        + 0.85
        * saturate(dot(normal, lightDirection));

    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target
{
    const float4 albedo =
        albedoTexture.Sample(input.uv);

    return float4(
        albedo.rgb * input.lighting,
        albedo.a
    );
}