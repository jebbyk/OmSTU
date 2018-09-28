#ifndef COLLIDER_H
#define COLLIDER_H

#include "TransformComponent.h"

class Collider {

float radius; // circle collider
String tag;
TransformComponent *transform;
std::list<String> collisionMask;
std::list<String>::iterator maskIterator;
bool symple = false;
bool destroy;

public:
	Collider() {}
	Collider(float _radius, TransformComponent *_transform)
	{
		radius = _radius;
		transform = _transform;
		if (transform->GetTag() == "particle")
		{
			symple = true;
		}
		destroy = false;
		tag = transform->GetTag();
	}

	void SetRadius(float _radius){radius = _radius;}
	void SetTransform(TransformComponent *_transform){transform = _transform;}
	void Destroy(){destroy = true;}
	void AddToCollisionMask(String _tag){collisionMask.push_back(_tag);}

	float GetRadius(){return radius;}
	TransformComponent *GetTransform(){return transform;}
	String *GetTag(){return &tag;}
	bool GetDestroy(){return destroy;}
	bool CheckMask(String *_tag)
	{
		bool a = false;
		for (maskIterator = collisionMask.begin(); maskIterator != collisionMask.end(); ++maskIterator)
		{
			if (*_tag == (*maskIterator))
			{
				a = true;
				break;
			}
		}
		return a;
	}
	bool DetectCollision(TransformComponent **_collisionTransform);
	bool DetectCollision();
};

std::list<Collider*> colliders;
std::list<Collider*>::iterator collidersIterator;
std::list<Collider*>::iterator collidersIteratorEnd;

bool Collider::DetectCollision(TransformComponent **_collisionTransform)
{
		bool b = false;
		int c = 0;
		for (collidersIterator = colliders.begin(), collidersIteratorEnd = colliders.end(); collidersIterator != collidersIteratorEnd; ++collidersIterator)
		{
			if (c < 2)//super big optimization
			{
				if(symple) c++;
				if (transform != (*collidersIterator)->GetTransform())
				{
					if (!(CheckMask((*collidersIterator)->GetTag())))
					{
						Vector2f v = (*collidersIterator)->GetTransform()->GetPosition() - transform->GetPosition();
						float vMod = sqrt(v.x * v.x + v.y*v.y);
						if (radius + (*collidersIterator)->GetRadius() > vMod)
						{
							*_collisionTransform = (*collidersIterator)->GetTransform();
							b = true;
							break;
						}
					}
				}
			}
			else { break; }
		}
		return b;
}


bool Collider::DetectCollision()
{
	bool b = false;
	int c = 0;
	for (collidersIterator = colliders.begin(), collidersIteratorEnd = colliders.end(); collidersIterator != collidersIteratorEnd; ++collidersIterator)
	{
		if (c < 3)
		{
			if(symple) c++;
			if (transform != (*collidersIterator)->GetTransform())
			{
				if (!(CheckMask((*collidersIterator)->GetTag())))
				{
					Vector2f v = (*collidersIterator)->GetTransform()->GetPosition() - transform->GetPosition();
					float vMod = sqrt(v.x * v.x + v.y*v.y);
					if (radius + (*collidersIterator)->GetRadius() > vMod)
					{
						b = true;
						break;
					}
				}
			}
		}
		else { break; }
	}
	return b;
}

Collider* CreateCollider(float _radius, TransformComponent *_transform)
{
	Collider *c = new Collider(_radius, _transform);
	colliders.push_back(c);
	return c;
}
#endif