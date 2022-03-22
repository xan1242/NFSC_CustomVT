shared float4 cvGradientParams					: REG_cvGradientParams;  // SCREEN X Y  & R & intensity
shared float3 cvGradientColour					: REG_cvGradientColour;  // RGB
shared float3 cvGradientColourE					: REG_cvGradientColourE;  // RGB
shared float3 cvScreenRez						: REG_ScreenRez;  		// X & Y res


float3 AddGradient(float3 result, float2 texCoord)
{
    float maxRad = cvGradientParams.z;   // X rad

    texCoord.y = cvGradientParams.y - ((texCoord.y - cvGradientParams.y) *  (cvGradientParams.z / cvScreenRez.z));

    float blend =  length(cvGradientParams.xy - texCoord.xy);
    blend =  (maxRad - clamp(blend, 0, maxRad))  /maxRad;

    float3 fincol = lerp(cvGradientColour.xyz, cvGradientColourE.xyz, 1-blend);
    result.xyz = blend * result.xyz  + lerp(result.xyz, (fincol.xyz * cvGradientParams.w), blend) ;

    return result.xyz;
}


