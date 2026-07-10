#ifndef NOISE_H
#define NOISE_H

#include "utils.h"
SAMPLER2D_AUTOREG(s_NoiseTexture);

// functions under [1] are from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83

// [1] hash function for noise (for highp only)
float rand(highp vec2 n) {
  return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

// 1D noise - used in plants,lantern wave
float noise1D(highp float x) {
  return texture2DLod(s_NoiseTexture, vec2_splat(x)*0.0001, 0).g;
}

// simpler rand
float fastRand(vec2 n){
  return fract(37.45*sin(dot(n, vec2(4.36, 8.28))));
}

// used by caustic
float disp(vec3 pos, float t) {
  float n = sin(8.0*PI_HALF*(pos.x+pos.y*pos.z) + 0.7*t);
  pos.y += t + 0.8*n;
  float p = floor(pos.y);
  return (0.8+0.2*n) * mix(fastRand(pos.xz+p), fastRand(pos.xz+p+1.0), pos.y - p);
}


float noise2D(vec2 u) {
  return texture2DLod(s_NoiseTexture, u*0.01, 0).b;
}

// 3D noise - used by galaxy
float noise3D(vec3 p) {
  vec3 w = abs(p);
  w = w / (w.x + w.y + w.z);

  vec3 n = vec3(
    texture2DLod(s_NoiseTexture, p.yz*0.01, 0).b*w.x, 
    texture2DLod(s_NoiseTexture, p.xz*0.01, 0).b*w.y, 
    texture2DLod(s_NoiseTexture, p.xy*0.01, 0).b*w.z
    );
  return n.x + n.y + n.z;
}

float fastVoronoi2(vec2 pos, float f) {
  return 1.0-f*texture2DLod(s_NoiseTexture, pos.xy*0.1, 0).r;



  
}

float movingNoise2D(vec2 pos, float t, float f) {
  vec2 tpos = 16.0*fract(pos/16.0);
  float nf0 = fastVoronoi2(0.125*pos.xy, 12.0);
  float nf2 = fastVoronoi2(0.04*pos.xy*vec2(0.5,1.0) + 0.5*nf0 + 0.05*t, 2.0);
  float n0 = sin(1.5*nf0*nf0 + pos.x - sin(pos.y) + t);
  float n1 = sin(0.05*(pos.x+pos.y) + 8.0*nf2 + 0.4*t);
  return mix(n0*n0, n1*n1, f);
}

#endif
