//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------



Texture2D txDiffuse  : register( t0 );
Texture2D txDiffuse1  : register( t1 );
SamplerState g_samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float x;
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
    i.Pos = mul( i.Pos, World );
    i.Pos = mul( i.Pos, View);
//    Pos = mul( Pos, Projection);
	//i.Pos.x = i.Pos.x - x;
	//Pos.x = Pos.x+0.5;

	
	PS_INPUT p;
	p.Pos = i.Pos;
	p.Text = i.Text;

	//p.Text.x = p.Text.x + x;
	//p.Color = i.Color;//float4(1,0,0,1);


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
	//if(x>0.5)
    
    //PS_INPUT temp = p;
    //temp.Text.x = temp.Text.x + x;
    //return  (txDiffuse.Sample( g_samLinear, temp.Text ) + txDiffuse1.Sample( g_samLinear, p.Text ))/2; 


    //BLACK AND WHITE
    //float4 color = txDiffuse.Sample( g_samLinear, p.Text );
    //float val = (color.r+color.g+color.b)/3;
    //return float4(val, val, val, color.w);

    float x, y;
    float4 c = float4(0,0,0,0);
    int counter = 0;
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
    return c/counter;
    //1920, 1080
}

float4 PS(PS_INPUT p) : SV_Target
{
    return  txDiffuse.Sample(g_samLinear, p.Text);
}
