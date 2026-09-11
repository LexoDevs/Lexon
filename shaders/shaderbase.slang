struct VSInput
{
    float3 inPosition : POSITION;
    float3 inNormal   : NORMAL;
    float2 inUv       : TEXCOORD0;
};

struct UniformBuffer
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

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

    output.position = mul(
        ubo.proj,
        mul(
            ubo.view,
            mul(ubo.model, float4(input.inPosition, 1.0))
        )
    );

    const float3 normal = normalize(input.inNormal);
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