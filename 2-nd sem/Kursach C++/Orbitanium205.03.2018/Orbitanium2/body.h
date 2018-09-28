#ifndef BODY_H
#define BODY_H

#include "collider.h"

class Body {
	Vector2f velocity;
	float mass, angularVelocity;
	bool movable;//makes object to be static
	TransformComponent *transform;
	Collider *collider;
	float reactionForce = 0;
	bool destroy;
	String tag;
	int tick = 0;
	std::list<String> reactionMask;
	std::list<String>::iterator reactionMaskIt;
public:
	Body() {}
	Body(Vector2f _velocity, float _mass, float _angularVelocity, bool _movable, TransformComponent *_transform, Collider *_collider)
	{
		mass = _mass;
		velocity = _velocity;
		angularVelocity = _angularVelocity;
		transform = _transform;
		tag = transform->GetTag();
		collider = _collider;
		movable = _movable;
		destroy = false;
	}

	void SetMass(float _mass){mass = _mass;}
	void SetVelocity(Vector2f _velocity){velocity = _velocity;}
	void SetAngularVelocity(float _angularVelocity){angularVelocity = _angularVelocity;}
	void SetTransform(TransformComponent *_transform){transform = _transform;}
	void SetCollider(Collider *_collider){collider = _collider;}
	void SetMovable(bool _bool){movable = _bool;}
	void SetReactionForce(float _force){reactionForce = _force;}
	void Destroy(){destroy = true;}
	void AddToMask(String _tag){reactionMask.push_back(_tag);}

	Vector2f GetVelocity(){return velocity;}
	float GetAngularVelocity(){return angularVelocity;}
	float GetMass(){return mass;}
	TransformComponent *GetTransform(){return transform;}
	String *GetTag(){return &tag;}
	Collider *GetCollider(){return collider;}
	bool GetMovable(){return movable;}
	float GetReactionForce(){return reactionForce;}
	bool GetDestroy(){return destroy;}
	bool CheckMask(String *_tag)
	{
		bool a = false;
		for (reactionMaskIt = reactionMask.begin(); reactionMaskIt != reactionMask.end(); ++reactionMaskIt)
		{
			if (*_tag == (*reactionMaskIt))
			{
				a = true;
				break;
			}
		}
		return a;
	}
	void AddForce(Vector2f force)
	{
		if (movable)
		{
			force /= mass;
			velocity += force * deltaTime;
		}
	}
	void ApplyGravity();
	void Update()
	{
		ApplyGravity();
		transform->Move(velocity);
		transform->Rotate(angularVelocity);
	}
};

std::list<Body*> bodies;
std::list<Body*>::iterator bodiesIterator;
std::list<Body*>::iterator bodiesIteratorEnd;

void Body::ApplyGravity()
{
	int c = 0;
	for (bodiesIterator = bodies.begin(), bodiesIteratorEnd = bodies.end(); bodiesIterator != bodiesIteratorEnd; ++bodiesIterator)
	{
		if (c < 3)//super big optimization;
		{
			
			c++;
			if (transform != (*bodiesIterator)->GetTransform() && movable)
			{
				if (!CheckMask((*bodiesIterator)->GetTag()))
				{
					//printf("reaction\n");
					Vector2f v = (*bodiesIterator)->GetTransform()->GetPosition() - transform->GetPosition();
					float vMod = sqrt(v.x * v.x + v.y*v.y);
					v = v / vMod;//единичный вектор (направление на источник гравитации)
					v *= (mass*(*bodiesIterator)->GetMass()) / (vMod*vMod);
					AddForce(v);
				}
			}
		}
		else { break; }
	}
}

Body* CreateBody(float _mass, Vector2f _velocity, float _angularVelocity, bool _movable, TransformComponent *_transform, Collider *_collider)
{
	Body *c = new Body(_velocity, _mass, _angularVelocity, _movable, _transform, _collider);
	bodies.push_back(c);
	return c;
}
#endif