#ifndef PARTICLE_CONTROLLER_H
#define PARTICLE_CONTROLLER_H

#include "body.h"
#include "DrawComponent.h"

class ParticleController {
	float startHealth;
	float curentHealth;
	float healthPercent;
	Color startColor;
	Color endColor;
	Color curentColor;
	bool isStretched;
	float stretchFactor;
	float exponentialGrow;
	float linearGrow;
	TransformComponent *transform;
	Body *body;
	Collider *collider;
	DrawComponent *drawer;
	bool destroy = false;
public:
	ParticleController(){}
	ParticleController(float stHl, Color stCol, Color endCol, bool isStr, float strFact, float expGrow, float linGrow, TransformComponent *tr, Collider *coll, Body *bod, DrawComponent *drw)
	{
		startHealth = stHl;
		curentHealth = stHl;
		startColor = stCol;
		endColor = endCol;
		curentColor = stCol;
		isStretched = isStr;
		stretchFactor = strFact;
		exponentialGrow = expGrow;
		linearGrow = linGrow;
		transform = tr;
		collider = coll;
		body = bod;
		drawer = drw;
		destroy = false;
	}
	
	void Destroy(){destroy = true;}

	void SetHealth(float _health)
	{
		startHealth = _health;
		curentHealth = _health;
	}
	void SetStartColor(Color _color)
	{
		startColor = _color;
		curentColor = _color;
	}
	void SetEndColor(Color _color){endColor = _color;}
	void SetTransform(TransformComponent *t){transform = t;}
	void SetCollider(Collider *c){collider = c;}
	void SetBody(Body *b){body = b;}
	void SetDrawer(DrawComponent *d){drawer = d;}

	float GetHealth(){return curentHealth;}
	Color GetColor(){return curentColor;}
	Color GetEndColor(){return endColor;}
	bool GetDestroy(){return destroy;}
	TransformComponent *GetTransform(){return transform;}
	Collider *GetCollider(){return collider;}
	DrawComponent *GetDrawer(){return drawer;}
	Body *GetBody(){return body;}

	void Update()
	{
		curentHealth -= deltaTime;
		healthPercent = curentHealth / startHealth;

		curentColor.r = endColor.r + ((startColor.r - endColor.r) * healthPercent);
		curentColor.g = endColor.g + ((startColor.g - endColor.g) * healthPercent);
		curentColor.b = endColor.b + ((startColor.b - endColor.b) * healthPercent);
		curentColor.a = endColor.a + ((startColor.a - endColor.a) * healthPercent);
		drawer->SetColor(curentColor);

		Vector2f v;

		v.x = linearGrow * deltaTime;
		v.y = linearGrow * deltaTime;
		v.x += ((transform->GetScale().x * exponentialGrow) - transform->GetScale().x)*deltaTime;
		v.y += ((transform->GetScale().y * exponentialGrow) - transform->GetScale().y)*deltaTime;

		transform->SetScale(transform->GetScale() + v);
		
		if(isStretched)
		{
			float vMod = ((body->GetVelocity().x)*(body->GetVelocity().x)) + ((body->GetVelocity().y)*(body->GetVelocity().y));
			transform->SetRotation(-((atan2(body->GetVelocity().x, body->GetVelocity().y) * 180) / 3.1416)+90);
			transform->SetScale( Vector2f((transform->GetScale().y*vMod*0.0001*stretchFactor) + transform->GetScale().y, transform->GetScale().y));
		}
	
		if (collider->DetectCollision() || curentHealth < 0)
		{
			transform->Destroy();
		}
	}
};

std::list<ParticleController*> particles;
std::list<ParticleController*>::iterator particlesIterator;

ParticleController* CreateParticleController(float stHl, Color stCol, Color endCol, bool isStr, float strFact, float expGrow, float linGrow, TransformComponent *tr, Collider *coll, Body *bod, DrawComponent *drw)
{
	ParticleController *c = new ParticleController(stHl, stCol, endCol, isStr, strFact, expGrow, linGrow, tr, coll, bod, drw);
	particles.push_back(c);
	return c;
}


#endif