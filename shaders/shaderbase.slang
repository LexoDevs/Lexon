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

    // xyz = dirección en la que viajan los rayos
    // w   = intensidad solar
    float4 sunDirectionIntensity;

    // rgb = color solar
    // w   = intensidad ambiente
    float4 sunColorAmbient;

    // x = modo de visualización
    uint4 renderFlags;
};

static const uint RENDER_MODE_TEXTURED = 0;
static const uint RENDER_MODE_WHITE    = 1;

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
    float4 position    : SV_Position;
    float2 uv          : TEXCOORD0;
    float3 worldNormal : TEXCOORD1;
};

[shader("vertex")]
VSOutput vertMain(VSInput input)
{
    VSOutput output;

    const float4 worldPosition =
        mul(
            objectData.model,
            float4(input.inPosition, 1.0)
        );

    output.position =
        mul(
            ubo.proj,
            mul(
                ubo.view,
                worldPosition
            )
        );

    output.uv = input.inUv;

    output.worldNormal =
        normalize(
            mul(
                (float3x3)objectData.normalMatrix,
                input.inNormal
            )
        );

    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target
{
    // ==========================================
    // Color base: textura o blanco
    // ==========================================

    float3 baseColor =
        float3(1.0, 1.0, 1.0);

    float alpha = 1.0;

    if (ubo.renderFlags.x ==
        RENDER_MODE_TEXTURED)
    {
        const float4 sampledColor =
            albedoTexture.Sample(input.uv);
if (sampledColor.a < 0.60)
{
    discard;
}
        baseColor = sampledColor.rgb;
        alpha = sampledColor.a;
    }

    // ==========================================
    // Luz solar direccional
    // ==========================================

    const float3 N =
        normalize(input.worldNormal);

    // direction representa la dirección en la que
    // viajan los rayos solares. Para Lambert
    // necesitamos la dirección hacia el sol.
    const float3 L =
        normalize(
            -ubo.sunDirectionIntensity.xyz
        );

    const float nDotL =
        saturate(dot(N, L));

    const float sunIntensity =
        ubo.sunDirectionIntensity.w;

    const float3 sunColor =
        ubo.sunColorAmbient.rgb;

    const float ambientIntensity =
        ubo.sunColorAmbient.w;

    const float3 directLighting =
        sunColor *
        sunIntensity *
        nDotL;

    const float3 ambientLighting =
        float3(
            ambientIntensity,
            ambientIntensity,
            ambientIntensity
        );

    const float3 finalLighting =
        ambientLighting +
        directLighting;

    // ==========================================
    // Resultado
    // ==========================================

    return float4(
        baseColor * finalLighting,
        alpha
    );
}