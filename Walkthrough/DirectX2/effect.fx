//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------



Texture2D txDiffuse  : register( t0 );
Texture2D txDiffuse1  : register( t1 );
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
    i.Pos.w = 1;
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
float4 PS2(PS_INPUT p) : SV_Target
{
    float x, y;
    float4 c = txDiffuse.Sample(g_samLinear, p.Text);
    int counter = 1;
    float f = txDiffuse1.Sample(g_samLinear, p.Text);
    //if(f.x>0.5)
    /*{
    c = float4(0,0,0,0);
    counter = 0;
    for(int i=0;i<10;i++)
    {
    	for(int j=0;j<10;j++)
    	{
    		x = i/500.0 -.01;
    		y = j/500.0 -.01;
    		float2 temp = p.Text+float2(x,y);
    		c = c + txDiffuse.Sample( g_samLinear, temp );
    		counter++;
    	}
    }
    }
    return c/counter;*/
    return (1-f) * c;
    //1920, 1080
}

float4 PS(PS_INPUT p) : SV_Target
{
    //float2 t=float2(1.0,1.0);
    float t = 0.8;
    float4 f=txDiffuse.Sample(g_samLinear, p.Text);
    f.a=t;
    return f;//txDiffuse.Sample(g_samLinear, p.Text);
}
