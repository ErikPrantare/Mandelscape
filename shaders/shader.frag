#version 330 core

in vec2 position;
in float distance;
out vec4 outColour;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;

uniform float time;

vec2 cSqr(vec2 z)
{
  return vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
}

void main(void)
{
  if(distance == 0.0) return;

  vec3 rgb = vec3(0.0);
  vec2 c = offset + position;
  vec2 z = vec2(0.0);
  vec3 s = vec3(0.0, 0.0, 0.0);
  vec3 t = vec3(0.0);
  const float R = 1e10;
  for (int i = 0; i < iterations; ++i)
  {
    t = sin(vec3(time, 3.0, 5.0) * atan(z.y, z.x)) * 0.5 + 0.5;
    float m = dot(z,z);
    if (m > R)
    {
      float j = 2.0 - log(m)/log(R);
      rgb = vec3((s + t * j) / (float(i) + j));
      break;
    }
    s += t;
    z = cSqr(z) + c;
  }
  outColour = vec4(rgb, 1.0);
}
