
struct PS_INPUT
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
};


float4 main(PS_INPUT pin) : SV_TARGET
{
    float4 color = pin.color;
    
    return color;
}