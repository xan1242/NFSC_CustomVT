///////////////////////////////////////////////////////////////////////////////
//
// Visual Treatment
//
///////////////////////////////////////////////////////////////////////////////
#define VIGNETTE_SCALE		0.50

// uncomment to enable ReShade FakeHDR pass
//#define USE_FAKEHDR
#define FAKEHDR_POWER 1.1
#define FAKEHDR_RADIUS1 0.848
#define FAKEHDR_RADIUS2 0.848

// uncomment to disable bloom rendering
//#define BLOOM_DISABLE

// uncomment to enable bloom texture render to screen
//#define BLOOM_DEBUG

// you can tune bloom in screeneffect.fx
// enter then exit video options to recompile shader


#include "global.h"
#include "gradient.h"
#include "visualtreatment.h"

// The FMV capture guys use this to render the depth as B&W
//#define RENDER_DEPTH



///////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////

// These defines just make the parameters more readable
#define DO_DOF(value)				value
#define DO_COLOUR_FADE(value)		value

technique visualtreatment
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_VisualTreatment(DO_DOF(false), DO_COLOUR_FADE(false));
	}
}

technique visualtreatment_enchanced
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader = compile ps_2_0 PS_VisualTreatment(DO_DOF(true), DO_COLOUR_FADE(true));
	}
}


technique motionblur
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS_MotionBlur();
		PixelShader	 = compile ps_2_0 PS_MotionBlur();
	}
}



technique composite_blur
{
	pass p0
	{
		VertexShader = compile vs_1_1 VS_CompositeBlur();
		PixelShader  = compile ps_2_0 PS_CompositeBlur();
	}
}



technique screen_passthru
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_PassThru();
	}
}


technique uvesovercliff
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_UvesOverCliff(DO_COLOUR_FADE(true));
	}
}

technique uvesovercliffdarken
{
	pass p0
	{
		VertexShader = compile vs_1_1 vertex_shader_passthru();
		PixelShader	 = compile ps_2_0 PS_UvesOverCliff(DO_COLOUR_FADE(true));
	}
}
