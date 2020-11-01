#include "include/perlin.h"

static float       noise(int x, int y) //seed this??
{
    int n;

        n = x + y * 57 * 2749;
        n = (n << 13) ^ n * 1087;
        return (1.0 - ( (n * ((n * n * 15731) + 789221) +  1376312589) & 0x7fffffff) / 1073741824.0)*3433;
}

static float       interpolate(float a, float b, float x)
{
  float     pi_mod;
  float     f_unk;

  pi_mod = x * 3.1415927;
  f_unk = (1 - cos(pi_mod)) * 0.5;
  return (a * (1 - f_unk) + b * x);
}

static float       smooth_noise(int x, int y)
{
  float     corners;
  float     center;
  float     sides;

  corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) +
         noise(x - 1, x + 1) + noise(x + 1, y + 1)) / 16;
  sides = (noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) +
       noise(x, y + 1)) / 8;
  center = noise(x, y) / 4;
  return (corners + sides + center);
}

static float       noise_handler(float x, float y)
{
  int       int_val[2];
  float     frac_val[2];
  float     value[4];
  float     res[2];

  int_val[0] = (int)x;
  int_val[1] = (int)y;
  frac_val[0] = x - int_val[0];
  frac_val[1] = y - int_val[1];
  value[0] = smooth_noise(int_val[0], int_val[1]);
  value[1] = smooth_noise(int_val[0] + 1, int_val[1]);
  value[2] = smooth_noise(int_val[0], int_val[1] + 1);
  value[3] = smooth_noise(int_val[0] + 1, int_val[1] + 1);
  res[0] = interpolate(value[0], value[1], frac_val[0]);
  res[1] = interpolate(value[2], value[3], frac_val[0]);
  return (interpolate(res[0], res[1], frac_val[1]));
}

float perlin_noise(float x, float y) //implement Fractal Brownian Motion?
{
    srand(time(NULL));
  float     total;
  float     per;
  float     amp;
  int       hz;
  int       octave;

  total = 0.0;
  per = 0.5;
  octave = 10;
  for(int i = 0; i < octave; i++)
    {
      hz = pow(2, i);
      amp = pow(per, (float)i);
      total += noise_handler(x * (float)hz, y * (float)hz) * amp;
    }
  return (total);
}
