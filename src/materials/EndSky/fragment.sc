#ifndef INSTANCING
  $input v_posTime, v_texcoord0
#endif

#include <bgfx_shader.sh>

#ifndef INSTANCING
  #include <newb/main.sh>
uniform vec4 FogAndDistanceControl;
uniform vec4 ViewPositionAndTime;
uniform vec4 FogColor;
uniform vec4 TimeOfDay;
uniform vec4 Day;
uniform vec4 DimensionID;
  SAMPLER2D_AUTOREG(s_SkyTexture);
#endif

void main() {
  #ifndef INSTANCING
     vec3 viewDir = normalize(v_posTime.xyz);
     nl_environment env = nlDetectEnvironment(DimensionID.x, TimeOfDay.x, Day.x, FogColor.rgb, FogAndDistanceControl.xyz);
    vec4 diffuse = texture2D(s_SkyTexture, v_texcoord0);

    vec3 color = renderEndSky(getEndHorizonCol(), getEndZenithCol(), normalize(v_posTime.xyz), v_posTime.w);

    color = colorCorrection(color);

    gl_FragColor = vec4(color, 1.0);
  #else
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
  #endif
}