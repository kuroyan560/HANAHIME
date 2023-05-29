struct Camera
{
    matrix view;
    matrix proj;
    matrix billBoard;
    matrix billBoardY;
    float3 eyePos;
    float nearClip;
    float farClip;
};

cbuffer cbuff0 : register(b0)
{
    Camera cam;
}

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct VSInput
{
    float4 pos : POS;
    float2 size : SIZE;
    float4 color : COLOR;
};

VSInput VSmain(VSInput input)
{
    return input;
}

struct GSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 uv : TEXCOORD;    
    float depthInView : CAM_Z;
};

float4 GetPos(float4 Pos, float2 Offset)
{
    float4 offset = float4(Offset, 0, 1);
    offset = mul(cam.billBoard, offset);
    float4 result = Pos;
    result.xyz += offset.xyz;
    result = mul(cam.proj, mul(cam.view, result));
    return result;
}

[maxvertexcount(4)]
void GSmain(
    point VSInput input[1],
inout TriangleStream<GSOutput> output)
{
    float2 offset = input[0].size / 2.0f;
    
    GSOutput element;
    element.col = input[0].color;
    
    //左下
    element.pos = GetPos(input[0].pos, float2(-offset.x, -offset.y));
    element.uv = float2(0, 1);
    element.depthInView = element.pos.z;
    output.Append(element);
    
    //左上
    element.pos = GetPos(input[0].pos, float2(-offset.x, offset.y));
    element.uv = float2(0, 0);
    element.depthInView = element.pos.z;
    output.Append(element);
    
    //右下
    element.pos = GetPos(input[0].pos, float2(offset.x, -offset.y));
    element.uv = float2(1, 1);
    element.depthInView = element.pos.z;
    output.Append(element);
    
    //右上
    element.pos = GetPos(input[0].pos, float2(offset.x, offset.y));
    element.uv = float2(1, 0);
    element.depthInView = element.pos.z;
    output.Append(element);
}

struct PSOutput
{
    float4 color : SV_Target0;
    float4 emissive : SV_Target1;
    float depth : SV_Target2;
};

PSOutput PSmain(GSOutput input) : SV_TARGET
{
    float4 texCol = tex.Sample(smp, input.uv);

    PSOutput output;
    output.color = input.col * texCol;
    output.emissive = float4(0,0,0,0);
    output.depth = 0.0f;
    return output;
}