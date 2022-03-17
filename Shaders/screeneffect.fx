///////////////////////////////////////////////////////////////////////////////
//
// HDR Effects
//
///////////////////////////////////////////////////////////////////////////////

#include "global.h"

// you can tune the bloom parameters here, recommended to first enable BLOOM_DEBUG in visualtreatment.fx
// enter then exit video options to recompile shader
#define BLOOM_EXPOSURE 6.0
#define BLOOM_BRIGHTPASS_THRESHOLD 1.0

///////////////////////////////////////////////////////////////////////////////

static const int	MAX_SAMPLES				= 16;	// Maximum texture grabs

// Contains	sampling offsets used by the techniques 
shared float4 cavSampleOffsets[MAX_SAMPLES]	: cavSampleOffsets;
shared float4 cavSampleWeights[MAX_SAMPLES]	: cavSampleWeights;
shared float4 cvBlurParams							: cvBlurParams;
shared float4 cvBaseAlphaRef		: cvBaseAlphaRef; //BASEALPHAREF;
   
static const float3	LUMINANCE_VECTOR  =	float3(0.2125f,	0.7154f, 0.0721f);

///////////////////////////////////////////////////////////////////////////////

sampler PERLINNOISE_SAMPLER = sampler_state  
{
	AddressU = WRAP;
	AddressV = WRAP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)
sampler	DIFFUSE_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};
DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE)
sampler	DEPTHBUFFER_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	LINEAR;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP1_TEXTURE)
sampler	MISCMAP1_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP1_TEXTURE)			   // needed for the PC
	AddressU = CLAMP; 
	AddressV = CLAMP; 
	MIPFILTER =	LINEAR;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP2_TEXTURE)
sampler	MISCMAP2_SAMPLER = sampler_state
{
    ASSIGN_TEXTURE(MISCMAP2_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	LINEAR;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

///////////////////////////////////////////////////////////////////////////////

struct VS_INPUT_WORLD
{
	float4 position : POSITION;
	float4 color    : COLOR;
	float4 texcoord : TEXCOORD;
};

struct VS_INPUT_SCREEN
{
	float4 position	: POSITION;
	float4 texcoord	: TEXCOORD;
};

struct VtoP
{
	float4 position	: POSITION;
	float4 tex		: TEXCOORD0;
};

struct DepthSpriteOut
{
	float4 colour	: COLOR0;
	float  depth	: DEPTH0;
};

///////////////////////////////////////////////////////////////////////////////

VtoP vertex_shader_passthru(const VS_INPUT_SCREEN IN)
{
	VtoP OUT;
	OUT.position = IN.position;
	OUT.tex	= IN.texcoord;

	return OUT;
}

///////////////////////////////////////////////////////////////////////////////
float4 GetBase(float2 texUV)
{
	float4 base = tex2D( DIFFUSE_SAMPLER, texUV );
	//base.xyz += (dot( base.xyz, LUMINANCE_VECTOR ) > 0.95) ? base.xyz : 0.0f;
	return base;
}

float4 PS_GaussBlur5x5(	const VtoP IN )	: COLOR
{
	float4	sample = 0.0f;

	for( int i=0; i	< 13; i++ )
	{
		sample += cavSampleWeights[i] * GetBase(IN.tex.xy + cavSampleOffsets[i].xy );
	}

	//return 1.0;
	return sample;
}

///////////////////////////////////////////////////////////////////////////////

// Code taken from: Magic Bloom by luluco250
/*
    Uncharted 2 Tonemapper

    Thanks John Hable and Naughty Dog.
*/
float3 tonemap(float3 col, float exposure) {
    static const float A = 0.15; //shoulder strength
    static const float B = 0.50; //linear strength
	static const float C = 0.10; //linear angle
	static const float D = 0.20; //toe strength
	static const float E = 0.02; //toe numerator
	static const float F = 0.30; //toe denominator
	static const float W = 11.2; //linear white point value

    col *= exposure;

    col = ((col * (A * col + C * B) + D * E) / (col * (A * col + B) + D * F)) - E / F;
    static const float white = 1.0 / (((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F);
    col *= white;
    return col;
}

float4 PS_Bloom( const VtoP	IN ) : COLOR
{
	float4 sample = 0;

	//for( int i=0; i	< 15; i++ )
	//{
	//	sample += cavSampleWeights[i] * tex2D(	DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[i].xy );
	//}

    sample += cavSampleWeights[0] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[0].xy);
	sample += cavSampleWeights[1] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[1].xy);
	sample += cavSampleWeights[2] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[2].xy);
	sample += cavSampleWeights[3] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[3].xy);
	sample += cavSampleWeights[4] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[4].xy);
	sample += cavSampleWeights[5] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[5].xy);
	sample += cavSampleWeights[6] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[6].xy);
	sample += cavSampleWeights[7] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[7].xy);
	sample += cavSampleWeights[8] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[8].xy);
	sample += cavSampleWeights[9] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[9].xy);
	sample += cavSampleWeights[10] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[10].xy);
	sample += cavSampleWeights[11] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[11].xy);
	sample += cavSampleWeights[12] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[12].xy);
	sample += cavSampleWeights[13] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[13].xy);
	sample += cavSampleWeights[14] * tex2D( DIFFUSE_SAMPLER, IN.tex.xy + cavSampleOffsets[14].xy);


	//return float4(tonemap(sample, 8.0), 1.0);
    return sample;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS_DownScale4x4(	in float2 vScreenPosition :	TEXCOORD0 ) : COLOR
{
	float4 result = 0;

	for( int i=0; i	< 16; i++ )
	{
		result += GetBase(vScreenPosition + cavSampleOffsets[i].xy );;
	}
		
	result /= 16;

	return result;
}

float4 PS_DownScale2x2(	in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
	float4 sample =	0.0f;

	for( int i=0; i	< 4; i++ )
	{
		sample += tex2D( DIFFUSE_SAMPLER,	vScreenPosition	+ cavSampleOffsets[i].xy );
	}
	
	return sample /	4;
}

float4 PS_DownScale2x2ForMotionBlur(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
/*	
	On further inspection the 2x2 box filter makes no bloody difference to the out come... so out it goes!!
*/
//	float4 sample =	0.0f;
	//float depth = 0.0f;
//
//	sample += tex2D( DIFFUSE_SAMPLER,	vScreenPosition	+ cavSampleOffsets[0].xy );
//	depth  += tex2D( DEPTHBUFFER_SAMPLER,	vScreenPosition	+ cavSampleOffsets[0].xy ).x;
//
 //   sample += tex2D( DIFFUSE_SAMPLER,	vScreenPosition	+ cavSampleOffsets[1].xy );
//	depth  += tex2D( DEPTHBUFFER_SAMPLER,	vScreenPosition	+ cavSampleOffsets[1].xy ).x;
//
//    sample += tex2D( DIFFUSE_SAMPLER,	vScreenPosition	+ cavSampleOffsets[2].xy );
//	depth  += tex2D( DEPTHBUFFER_SAMPLER,	vScreenPosition	+ cavSampleOffsets[2].xy ).x;
//
//    sample += tex2D( DIFFUSE_SAMPLER,	vScreenPosition	+ cavSampleOffsets[3].xy );
//	depth  += tex2D( DEPTHBUFFER_SAMPLER,	vScreenPosition	+ cavSampleOffsets[3].xy ).x;

	//sample /= 4;
	//depth  /= 4;


	float4 sample = tex2D( DIFFUSE_SAMPLER,	vScreenPosition);
	float depth  = tex2D( DEPTHBUFFER_SAMPLER,	vScreenPosition).x;
	// The Alpha channel of the screen is a bloom mask so make sure the mask maintains it's bloom
	// intensity through the motion blur
	//
	// Alpha channel no longer used for bloom
	sample.xyz += (sample.w*sample.xyz);

	// Ramp the colour down with distance (depth) - this way far away objects don't motion blur
	//
	float kDistFalloffStart = cvBlurParams.x;	// meters
	//float kDistFalloffEnd   = cvBlurParams.y;	// meters
    //float kDistFalloffStart = 0.0;	// meters
	float kDistFalloffEnd   = cvBlurParams.y * 200;	// meters
	float kM = -1 / (kDistFalloffEnd-kDistFalloffStart);	// slope: how rapid the fall off is
	float kC = -kDistFalloffEnd * kM;
	depth    = (1 / (1-depth));
	depth    = saturate(depth*kM+kC);
	//sample.xyz *= depth;
	sample.w    = depth ;
	
	return sample;
}

// NOTE by xan1242/Tenjoin: BLOOM IS BROKEN IN CUTSCENES IF YOU DON'T PATCH THE SHADER OBJECT IN MEMORY
// cavSampleWeights is also used by GaussBlur5x5 and is interfering with the bloom (unless the technique is redirected to the HDR shader instead! it exists there too!)
float4 PS_DownScaleForBloom(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
    float3 sample = 0;

    sample += cavSampleWeights[0] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[0].xy ).xyz;
    sample += cavSampleWeights[1] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[1].xy ).xyz;
    sample += cavSampleWeights[2] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[2].xy ).xyz;
    sample += cavSampleWeights[3] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[3].xy ).xyz;
    sample += cavSampleWeights[4] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[4].xy ).xyz;
    sample += cavSampleWeights[5] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[5].xy ).xyz;
    sample += cavSampleWeights[6] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[6].xy ).xyz;
    sample += cavSampleWeights[7] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[7].xy ).xyz;
    sample += cavSampleWeights[8] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[8].xy ).xyz;
    sample += cavSampleWeights[9] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[9].xy ).xyz;
    sample += cavSampleWeights[10] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[10].xy ).xyz;
    sample += cavSampleWeights[11] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[11].xy ).xyz;
    sample += cavSampleWeights[12] * tex2D( DIFFUSE_SAMPLER, vScreenPosition + cavSampleOffsets[12].xy ).xyz;

    sample = float4(tonemap(sample.rgb, BLOOM_EXPOSURE), 1.0); // exposure

    sample = DeCompressColourSpace(sample);

    // Operate on the max colour channel to remove the hue blowout and blue tinge
	// exhibited by the original filter
	const float kBrightPassThreshold = BLOOM_BRIGHTPASS_THRESHOLD;
	float maxChannel = max(max(sample.x, sample.y), sample.z);
	maxChannel -= kBrightPassThreshold;
	//maxChannel = max(maxChannel, 0.0f);
	maxChannel = 2*maxChannel / (kBrightPassThreshold+maxChannel);

	return float4(sample*0.5 + sample*maxChannel, 0);

    //return float4(sample.rgb,1);
    //return 0;
}

float4 PS_BlendTextures(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
	float4 result;
	result  = tex2D( DIFFUSE_SAMPLER,vScreenPosition) * cavSampleWeights[0];
	result += tex2D( MISCMAP1_SAMPLER,	vScreenPosition) * cavSampleWeights[1];
	
	//result = tex2D( DIFFUSE_SAMPLER,vScreenPosition);
	return result;
}


DepthSpriteOut PS_DepthSprite(in float2 vScreenPosition : TEXCOORD0 )
{
	DepthSpriteOut OUT;

	OUT.colour = float4(tex2D( DIFFUSE_SAMPLER, vScreenPosition).xyz, 0);
	OUT.depth  = tex2D( DEPTHBUFFER_SAMPLER, vScreenPosition).x;

	return OUT;
}

float4 PS_CombineReflectionColourAndHeadlightAlpha(in float2 vScreenPosition :	TEXCOORD0 )	: COLOR
{
	float4 result;
	// Flares
	result.xyz = tex2D( DIFFUSE_SAMPLER,vScreenPosition).xyz;
	// Headlight
	result.w   = dot(tex2D( MISCMAP1_SAMPLER,vScreenPosition).xyz, LUMINANCE_VECTOR);

	return result;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// PREP BUFFER SHADER
//
//

struct PrepBuffersOut
{
	float4 colour0	: COLOR0;
	float4 colour1	: COLOR1;
	float  depth	: DEPTH0;
};

struct VS_PassThru_OUT
{
	float4 position	: POSITION;
	float2 texcoord	: TEXCOORD;
};

VS_PassThru_OUT VS_PassThru(const VS_INPUT_SCREEN IN)
{
	VS_PassThru_OUT OUT;
	OUT.position = IN.position;
	OUT.texcoord	= IN.texcoord;

	return OUT;
}

PrepBuffersOut PS_PrepSmokeBuffers(in float2 texCoord : TEXCOORD )
{
	PrepBuffersOut OUT;

	OUT.colour0 = float4(tex2D( DIFFUSE_SAMPLER, texCoord).xyz, 0);

	//OUT.colour1 = float4(tex2D( PERLINNOISE_SAMPLER, texCoord).xyz, 0);
	OUT.colour1 = float4(0.5,0.5,0.5,1);	// default middle grey implies no perlin offset
	OUT.depth  =  tex2D( DEPTHBUFFER_SAMPLER, texCoord).x;

	return OUT;
}

struct CompositeOut
{
	float4 colour	: COLOR0;
	float  depth	: DEPTH0;
};

CompositeOut PS_CompositeSmoke(in float2 texCoord : TEXCOORD0 )
{
	CompositeOut OUT;
	OUT.depth  = tex2D( DEPTHBUFFER_SAMPLER, texCoord).x;

	float4 original_backbuffer	= tex2D( DIFFUSE_SAMPLER,  texCoord);
	
	//
	// Sample into the blurred buffer using a perlin noise offset
	//
	float2 perlin_noise	= tex2D( PERLINNOISE_SAMPLER, texCoord).xy * 2 - 1;
	float4 solid_smoke	= tex2D( MISCMAP1_SAMPLER, texCoord + perlin_noise / 25);
//30
	OUT.colour.xyz	= lerp(original_backbuffer.xyz, solid_smoke, saturate(solid_smoke.w*1.1));
	OUT.colour.w	= solid_smoke.w/2;  

	return OUT;
}

technique CompositeSmoke
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS_PassThru();
		PixelShader	 = compile ps_2_0 PS_CompositeSmoke();
	}
}

technique PrepSmokeBuffers
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS_PassThru();
		PixelShader	 = compile ps_2_0 PS_PrepSmokeBuffers();
	}
}

technique TwoPassBlur
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_Bloom();
	}
}

technique GaussBlur5x5
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_GaussBlur5x5();
	}
}

technique DownScale4x4
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_DownScale4x4();
	}
}

technique DownScale2x2
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_DownScale2x2();
	}
}

technique DownScale2x2ForMotionBlur
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_DownScale2x2ForMotionBlur();
	}
}

technique DownScaleForBloom
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_DownScaleForBloom();
	}
}

technique BlendTextures
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_BlendTextures();
	}
}

technique DepthSprite
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_DepthSprite();
	}
}

technique CombineReflectionColourAndHeadlightAlpha
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_CombineReflectionColourAndHeadlightAlpha();
	}
}
