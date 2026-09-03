struct VSInput
{
    float3 inPosition : POSITION;
    float3 inNormal   : NORMAL;
};

struct UniformBuffer
{
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};

[[vk::binding(0, 0)]]
ConstantBuffer<UniformBuffer> ubo;

struct VSOutput
{
    float4 pos : SV_Position;
    float grayscale : COLOR0;
};

[shader("vertex")]
VSOutput vertMain(VSInput input)
{
    VSOutput output;

    output.pos = mul(
        ubo.proj,
        mul(
            ubo.view,
            mul(ubo.model, float4(input.inPosition, 1.0))
        )
    );

    float3 normal = normalize(input.inNormal);
    float3 lightDirection = normalize(float3(0.35, 0.75, 0.55));

    output.grayscale =
        0.15 + 0.85 * abs(dot(normal, lightDirection));

    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_TARGET
{
    return float4(
        input.grayscale,
        input.grayscale,
        input.grayscale,
        1.0
    );
}