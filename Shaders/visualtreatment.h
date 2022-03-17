#ifndef VISUALTREATMENT_H
#define VISUALTREATMENT_H

/********************************************************************

created:	2007/07/26
filename: 	Z:\Oban\Indep\Src\Render\Shaders\visualtreatment.h
file path:	Z:\Oban\Indep\Src\Render\Shaders
file base:	visualtreatment
file ext:	h
author:		mriegger

purpose:	Cross platform implementation of the visualtreatment shader

*********************************************************************/

#define MOTIONBLUR_DONTPASSTHRUDEPTH
//#define RENDER_DEPTH
#define FLASH_SCALE		cvVisualTreatmentParams.x
#define COP_INTRO_SCALE     cvVisualTreatmentParams.z
#define BREAKER_INTENSITY       cvVisualTreatmentParams.y
// cvVisualTreatmentParams.w -- UNUSED IN CARBON -- this can be coded in to do vignette scale control...
//#define VIGNETTE_SCALE		cvVisualTreatmentParams.w


shared float4 cvBlurParams				: cvBlurParams;
// CONSTANTS

// The per-color weighting to be used for luminance	calculations in	RGB	order.
static const float3	LUMINANCE_VECTOR  =	float3(0.2125f,	0.7154f, 0.0721f);

// PARAMETERS 

shared float4 cvVisualEffectFadeColour	: cvVisualEffectFadeColour;
//shared float cfSplitScreenUVScale		: cfSplitScreenUVScale;

// Tone	mapping	variables
shared float cfBloomScale				: cfBloomScale;

// Depth of Field variables
shared float4 cvDepthOfFieldParams		: cvDepthOfFieldParams; //DEPTHOFFIELD_PARAMS;

shared bool cbDrawDepthOfField			: cbDrawDepthOfField;
shared bool cbDepthOfFieldEnabled			: cbDepthOfFieldEnabled;

//shared float4 cvTextureOffset		: REG_cvTextureOffset; //TEXTUREANIMOFFSET;

shared float cfVignetteScale        : cfVignetteScale;
shared float4 cvVisualTreatmentParams : cvVisualTreatmentParams;
shared float cfVisualEffectVignette : cfVisualEffectVignette;
shared float cfVisualEffectBrightness : cfVisualEffectBrightness;
shared float4 cvTextureOffset	: cvTextureOffset;


/*** Forward Declarations *****************************************************************************************/


/*** Samplers *****************************************************************************************/

DECLARE_TEXTURE(DIFFUSEMAP_TEXTURE)				// needed for the PC
sampler	DIFFUSE_SAMPLER =	sampler_state
{
	ASSIGN_TEXTURE(DIFFUSEMAP_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MOTIONBLUR_TEXTURE)				// needed for the PC
sampler	MOTIONBLUR_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(MOTIONBLUR_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;//BORDER;
	AddressV = CLAMP;//BORDER;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(DEPTHBUFFER_TEXTURE)				// needed for the PC
sampler	DEPTHBUFFER_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(DEPTHBUFFER_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	LINEAR;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP2_TEXTURE)				// needed for the PC
sampler	MISCMAP2_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(MISCMAP2_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = WRAP;		// Use mirror for split screen so the vignette works
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP3_TEXTURE)				// needed for the PC
sampler	BLOOM_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(MISCMAP3_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(MISCMAP6_TEXTURE)				// needed for the PC
sampler	MISCMAP6_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(MISCMAP6_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	MIPFILTER =	LINEAR;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(VOLUMEMAP_TEXTURE)				// needed for the PC
sampler VOLUMEMAP_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(VOLUMEMAP_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

DECLARE_TEXTURE(BLENDVOLUMEMAP_TEXTURE)				// needed for the PC
sampler BLENDVOLUMEMAP_SAMPLER = sampler_state
{
	ASSIGN_TEXTURE(BLENDVOLUMEMAP_TEXTURE)			   // needed for the PC
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
	MIPFILTER =	NONE;
	MINFILTER =	LINEAR;
	MAGFILTER =	LINEAR;
};

/*** Structures **************************************************************************************/

struct DepthSpriteOut
{
	float4 colour	: COLOR0;
	// Passing thru depth on the PC (1) isn't needed, (2) seems to cause visual errors on nvidia 7-series gpus
#ifndef MOTIONBLUR_DONTPASSTHRUDEPTH
	float  depth	: DEPTH0;
#endif
};

///////////////////////////////////////////////////////////////////////////////

// should pack these two, reduce input bandwidth.   may not be significant
struct VS_INPUT_SCREEN
{
	float4 position	: POSITION;
	float4 tex0		: TEXCOORD0;
};

struct VtoP
{
	float4 position		: POSITION;
	float4 tex0			: TEXCOORD0;
};

VtoP vertex_shader_passthru(const VS_INPUT_SCREEN IN)
{
	VtoP OUT;
	OUT.position = IN.position;
	OUT.tex0	= IN.tex0;
	return OUT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Motion Blur
//
struct VS_INPUT_MOTIONBLUR
{
	float4 position	: POSITION;
	float4 tex0		: TEXCOORD0;
	float4 tex1		: TEXCOORD1;
	float4 tex2		: TEXCOORD2;
	float4 tex3		: TEXCOORD3;
	float4 tex4		: TEXCOORD4;
	float4 tex5		: TEXCOORD5;
	float4 tex6		: TEXCOORD6;
	float4 tex7		: TEXCOORD7;
};

struct VtoP_MOTIONBLUR
{
	float4 position		: POSITION;
	float2 tex[8]		: TEXCOORD0;
};

/*** Functions ***************************************************************************************/

VtoP_MOTIONBLUR VS_MotionBlur(const VS_INPUT_MOTIONBLUR IN)
{
	VtoP_MOTIONBLUR OUT;
	OUT.position = IN.position;
	OUT.tex[0] = IN.tex0.xy;
	OUT.tex[1] = IN.tex1.xy;
	OUT.tex[2] = IN.tex2.xy;
	OUT.tex[3] = IN.tex3.xy;
	OUT.tex[4] = IN.tex4.xy;
	OUT.tex[5] = IN.tex5.xy;
	OUT.tex[6] = IN.tex6.xy;
	OUT.tex[7] = IN.tex7.xy;
	return OUT;
}


///////////////////////////////////////////////////////////////////////////////

const float kWeights[8] = { 6, 5, 4, 3, 2, 1, 1, 1 };
float4 PS_MotionBlur(const VtoP_MOTIONBLUR IN) : COLOR 
{
	//float rampCoeff = cvBlurParams.x;
	float4	result = 0;

	// compute motion blurred image
	for(int i=0; i < 8; i++)
	{
		float4 blur = tex2D( MOTIONBLUR_SAMPLER, IN.tex[i]);
		//float3 ramp = blur.xyz / (rampCoeff - blur.xyz);	// ramp
		//blur.xyz = max(blur.xyz, ramp);									// don't want to darken the image
		//result += blur;// * (weights[i] / 16);
		result += blur * (kWeights[i] / 23.0f);
	}
	//result /= 8;

	//return tex2D( MOTIONBLUR_SAMPLER, IN.tex[0]);

	return result;
}

// --------------------------------------------------------------------------------------------------

VtoP VS_CompositeBlur(const VS_INPUT_SCREEN IN)
{
	VtoP OUT;
	OUT.position = IN.position;
	OUT.tex0	= IN.tex0;
	return OUT;
}


DepthSpriteOut PS_CompositeBlur(const VtoP IN)
{
	DepthSpriteOut OUT;

	float4 screen = tex2D( DIFFUSE_SAMPLER, IN.tex0.xy );
	float4 blur   = tex2D( MOTIONBLUR_SAMPLER, IN.tex0.xy );

	float senseOfSpeedScale = cvBlurParams.x; // 0 travelling slow, 1 travelling fast
	OUT.colour.xyz = lerp(screen.xyz, blur.xyz, blur.w*senseOfSpeedScale);
	OUT.colour.w = screen.w;
#ifndef MOTIONBLUR_DONTPASSTHRUDEPTH
	OUT.depth  = tex2D( DEPTHBUFFER_SAMPLER, IN.tex0.xy).x;
#endif

	//OUT.colour = blur;

	return OUT;
}

// --------------------------------------------------------------------------------------------------
float4 PS_PassThru(const VtoP IN) : COLOR
{
	float4 OUT;

	float4 diffuse = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);	// * IN;

	OUT.xyz	= diffuse;

	OUT.w =	diffuse.w;

	return OUT;
}

// Fake HDR -- ported from ReShade
/**
 * HDR
 * by Christian Cann Schuldt Jensen ~ CeeJay.dk
 *
 * Not actual HDR - It just tries to mimic an HDR look (relatively high performance cost)
 */
#ifdef USE_FAKEHDR
float4 HDRPass(const VtoP IN, in float4 result)
{
    float HDRPower = FAKEHDR_POWER;
    float radius1 = FAKEHDR_RADIUS1;
    float radius2 = FAKEHDR_RADIUS2;
    result = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy);
	float3 color = result.rgb;


	float3 bloom_sum1 = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(1.5, -1.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-1.5, -1.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 1.5,  1.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-1.5,  1.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 0.0, -2.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 0.0,  2.5) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-2.5,  0.0) * radius1 * 1).rgb;
	bloom_sum1 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 2.5,  0.0) * radius1 * 1).rgb;

	bloom_sum1 *= 0.005;

	float3 bloom_sum2 = tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(1.5, -1.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-1.5, -1.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 1.5,  1.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-1.5,  1.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 0.0, -2.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 0.0,  2.5) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2(-2.5,  0.0) * radius2 * 1).rgb;
	bloom_sum2 += tex2D(DIFFUSE_SAMPLER, IN.tex0.xy + float2( 2.5,  0.0) * radius2 * 1).rgb;

	bloom_sum2 *= 0.010;



	float dist = radius2 - radius1;
	float3 HDR = (color + (bloom_sum2 - bloom_sum1)) * dist;
	float3 blend = HDR + color;
	color = pow(abs(blend), abs(HDRPower)) + HDR; // pow - don't use fractions for HDRpower
	//result = float4(saturate(blend), 0.0);
    //result = float4(HDR, 1.0);
    result.xyz = color;

	return result;
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// Depth Of Field
//
// The blurred texture in sampler contains multiple mipmap levels
// representing different blurred intensities of the back buffer.
// The result colour passed in is the shape in focus representation
// of the image.  Use the zDepth, focal Distance and depth of field to
// compute a blurred amount between 0 and 1.  Multiple this by the max
// blur (max blur is between 1 and 3 because there are 3 mip map levels)
// to yeild which Mip level to bias to.  Then lerp between the sharp 
// image and blurred image.  This should give us a nice transition between
// 4 textures that representing different stages of depth

float4 DoDepthOfField(const VtoP IN, in float4 result, float depth)
{
	float zDist			= (1 / (1-depth));
	float focalDist		= cvDepthOfFieldParams.x;
	float depthOfField	= cvDepthOfFieldParams.y;
	float falloff		= cvDepthOfFieldParams.z;
	float maxBlur		= cvDepthOfFieldParams.w;
	float blur			= saturate((abs(zDist-focalDist)-depthOfField)*falloff/zDist);
	float mipLevel		= blur*maxBlur;
	float3 blurredTex	= tex2Dbias( MISCMAP6_SAMPLER,  float4(IN.tex0.xy, 0, mipLevel) );
	result				= float4(lerp(result.xyz, blurredTex, saturate(mipLevel)), result.w); 

	//result.xyz = saturate((abs(zDist-50)-30)*falloff/zDist);
	if( cbDrawDepthOfField )
	{
		result.x += (1-blur)*0.3;						// draw the depth of field falloff
		result.xyz += ( blur == 0.0f ) ? 0.5 : 0.0f;	// draw the crital focal point
	}

	return result;
}


float4 PS_VisualTreatment(const VtoP IN, uniform bool doDepthOfField, uniform bool doColourFade) : COLOR
{
#ifdef USE_FAKEHDR
    float4	result = HDRPass(IN, result);
#else
    float4	result;
	// Full screen image
	//
	float4 screenTex = tex2D( DIFFUSE_SAMPLER, IN.tex0.xy );
	result = screenTex;
#endif
	float depth	= tex2D( DEPTHBUFFER_SAMPLER, IN.tex0.xy ).x;
    float	zDist	  = (1 / (1-depth));

    // CARBON EDIT - uncomment the following 2 lines to simulate ProStreet
	// Convert from log space to linear space and clamp[0,1]
	//result.xyz = saturate(DeCompressColourSpace(result.xyz));
	//result.xyz = saturate(result.xyz);

	// HDR Bloom buffer
#ifndef BLOOM_DISABLE
	float3 Bloom = (cfBloomScale) * pow(tex2D( BLOOM_SAMPLER, IN.tex0.xy ),1.8) ;

	result.xyz =  (1 - ( 1- saturate(2*Bloom))*(1-result.xyz) );  //SCREEN TO GLOW BRIGHT AREAS

	result.xyz += Bloom*0.4; //ADD
#endif

    //
	// Do Depth of Field
	//
#ifndef DONTDODEPTH
	if( doDepthOfField && cbDepthOfFieldEnabled )	// Compile time branch is optimized out
	{
		result = DoDepthOfField(IN, result, depth);
	}
#endif

    // Depth of Field FMV capture stuff
	//
#ifdef RENDER_DEPTH
	//float zDist	= (1 / (1-depth));
	result.xyz = (zDist*0.5) / 100;
#endif

    // LUT filter
    result.xyz = tex3D(VOLUMEMAP_SAMPLER, result.xyz).xyz;

    // cop intro -- ported from Undercover
    float3 copTreatment = tex3D(BLENDVOLUMEMAP_SAMPLER, result.xyz).xyz;
    result.xyz = lerp(result.xyz, copTreatment, COP_INTRO_SCALE);

    // VIGNETTE
	float4 vignette  = tex2D( MISCMAP2_SAMPLER,  IN.tex0.xy );
	float mask = vignette.y;
	float black = 0;
	result.xyz = lerp(result.xyz, black, saturate(mask) * (VIGNETTE_SCALE) );

    float luminance = dot(LUMINANCE_VECTOR, result.xyz);
    // Pursuit / speed breaker
    result.xyz = lerp(result.xyz, luminance / 2.5, saturate(vignette.w) * BREAKER_INTENSITY);

    // Allow the NIS's to fade to a colour
	//
	if( doColourFade )		// Compile time branch is optimized out
	{
		result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
	}
    result.xyz *= FLASH_SCALE;

	// Lens contamination gradient effect
	//
	//result.xyz = AddGradient(result, IN.tex0.xy);

    // bloom test
#ifndef BLOOM_DISABLE
#ifdef BLOOM_DEBUG
    result.xyz = Bloom*0.4; //ADD
#endif
#endif

	return result;
}


#endif

float GetRandomValue( float2 lInPos )
{
    // cheesy random function which appears to give quite good noise, but is probably much more expensive than it needs to be
    float  lrRandom = ( pow( 9.213f, ( lInPos.x % 0.25f ) + 3.0f ) * pow( 7.12345f, ( lInPos.y % 0.25f ) + 3.0f ) ) % 1.0f;
    lrRandom = ( lrRandom - 0.5f );
    return lrRandom;// * (3.0f / 1280.0f);
}

float4 PS_UvesOverCliff(const VtoP IN, uniform bool doColourFade) : COLOR
{
	float4	result;

	// Full screen image
	//
	float4 screenTex = tex2D( DIFFUSE_SAMPLER, IN.tex0.xy );
	result = screenTex;

	// HDR Bloom buffer
	float3 Bloom = (cfBloomScale) * pow(tex2D( BLOOM_SAMPLER, IN.tex0.xy ),1.8) ;


	result.xyz =  (1 - ( 1- saturate(2*Bloom))*(1-result.xyz) );  //SCREEN TO GLOW BRIGHT AREAS

	result.xyz += Bloom*0.4; //ADD

    // LUT filter
    result.xyz = tex3D(VOLUMEMAP_SAMPLER, result.xyz).xyz;

    // VIGNETTE
	float4 vignette  = tex2D( MISCMAP2_SAMPLER,  IN.tex0.xy );
	float mask = vignette.y;
	float black = 0;
	result.xyz = lerp(result.xyz, black, saturate(mask) * (VIGNETTE_SCALE) );

    float luminance = dot(LUMINANCE_VECTOR, result.xyz);
    //float maxChannel = max(max(result.x, result.y), result.z);
    //result.xyz = max(max(result.x, result.y), result.z);


    // janky uvesovercliff effect - NOT BY EA, by Xan/Tenjoin
    result.xyz = lerp(result.x * 1.6 , luminance * 2, 1);
    if (result.x <= 0.002)
        result.x = -1 * result.x;
    if (saturate(result.x) >= 0.95)
        result.x = -1 * result.x;
    result.xyz = result.x;
    result.xyz *= GetRandomValue(result.xy) + 1;

    // Allow the NIS's to fade to a colour
	//
	if( doColourFade )		// Compile time branch is optimized out
	{
		result.xyz = lerp(result.xyz, cvVisualEffectFadeColour.xyz, cvVisualEffectFadeColour.w);
	}

	return result;
}
