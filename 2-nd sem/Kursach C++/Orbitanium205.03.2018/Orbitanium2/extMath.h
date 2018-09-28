#ifndef EXT_MATH_H
#define EXT_MATH_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
float inv_rand_max = (float)1 / RAND_MAX;

float  frand(float min, float max)
{
	float fval = rand();
	max -= min;
	return (fval*inv_rand_max*max + min);
}

float blurRand(float base, float percent)
{
	return (base + frand(-base * percent, base * percent));
}
Vector2f blurRand2f(Vector2f baseV, Vector2f percentV)
{
	return Vector2f(blurRand(baseV.x, percentV.x), blurRand(baseV.y, percentV.y));
}
Vector2f blurRand2f(Vector2f baseV, float percent)
{
	float a = blurRand(baseV.x, percent);
	return Vector2f(a, a);
}

Color blurRandColor(Color baseC, Color percentC)
{
	Color c = Color(blurRand(baseC.r, percentC.r), blurRand(baseC.g, percentC.g), blurRand(baseC.b, percentC.b), blurRand(baseC.a, percentC.a));
	return c;
	
}

float zeroRand(float percent)
{
	return (frand(-percent, percent));
}
Vector2f zeroRand2f(Vector2f percentV)
{
	return Vector2f(zeroRand(percentV.x), zeroRand(percentV.y));
}
Vector2f zeroRand2f(float percent)
{
	return Vector2f(zeroRand(percent), zeroRand(percent));
}
#endif