//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------



Texture2D txDiffuse  : register( t0 );
Texture2D DepthValue  : register( t1 );
SamplerState g_samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
}

struct VS_INPUT{
	float4 Pos : POSITION;
	//float4 Color : COLOR;	
	float2 Text: TEXTURE;
};

struct PS_INPUT{
	float4 Pos : SV_POSITION;
	float2 Text : c;
};

PS_INPUT VS( VS_INPUT i)
{
    i.Pos = mul( World, i.Pos );
	PS_INPUT p;
	p.Pos = i.Pos;
	p.Text = i.Text;

    return p;
}


PS_INPUT VS2(VS_INPUT i)
{

    PS_INPUT p;
    p.Pos = i.Pos;
    p.Text = i.Text;
    return p;
}
float4 blur(float2 Text, float depth)
{
    //return txDiffuse.Sample( g_samLinear, Text);
    float x, y;
    float4 c = txDiffuse.Sample( g_samLinear, Text);
    int counter = 0;
    int lim = depth*9+1;

    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            x = i*(lim+1)*0.0001 -0.00005*(lim+1);
            y = j*(lim+1)*0.0001 -0.00005*(lim+1);
            float2 temp = Text+float2(x,y);
            c = c + txDiffuse.Sample( g_samLinear, temp );
            counter++;
        }
    }
    return c/counter;
}
float4 PS2(PS_INPUT p) : SV_Target
{
    //float4 col = txDiffuse.Sample(g_samLinear, p.Text);
    float4 depthcol = DepthValue.Sample(g_samLinear, p.Text);
    float depth = depthcol.r;
    
    return blur(p.Text, depth);
    //1920, 1080
}

float4 PS2g(PS_INPUT p) : SV_Target
{
    float4 col = txDiffuse.Sample(g_samLinear, p.Text);
    float4 depthcol = DepthValue.Sample(g_samLinear, p.Text);
    float depth = depthcol.r;
    col.g = depthcol.r;
    col.r = 0;
    return col;
    //1920, 1080
}
float4 PS2b(PS_INPUT p) : SV_Target
{
    float4 col = txDiffuse.Sample(g_samLinear, p.Text);
    float4 depthcol = DepthValue.Sample(g_samLinear, p.Text);
    float depth = depthcol.r;
    
    col.b = depthcol.r;
    col.r = 0;
    return col;
    
    //1920, 1080
}
float4 PS(PS_INPUT p) : SV_Target
{
    //float2 t=float2(1.0,1.0);
    float t = 1.5-DepthValue.Sample(g_samLinear, p.Text);
    if(t>1)t = 1;
    float4 f=txDiffuse.Sample(g_samLinear, p.Text);
    f.a=t;
    return f;//txDiffuse.Sample(g_samLinear, p.Text);
}

float4 PS3(PS_INPUT p):SV_TARGET
{
    float4 col = txDiffuse.Sample(g_samLinear, p.Text);
    float temp = (col.r+col.b+col.g)/3;
    col.r = temp;
    col.g = temp;
    col.b = temp;
    return col;
}