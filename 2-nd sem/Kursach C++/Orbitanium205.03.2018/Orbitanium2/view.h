#ifndef VIEW_H
#define VIEW_H

#include "TransformComponent.h"

View view;
Vector2f targetPoint;

Vector2f p;

float speed = 1;

void SetViewTargetPoint(Vector2f _point)
{
	targetPoint = _point;
}

void UpdateView()
{
	p += ((targetPoint - view.getCenter())* speed)  * deltaTime;
	view.setCenter(p);
}

#endif