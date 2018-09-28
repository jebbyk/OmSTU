#ifndef ASTEROID_H
#define ASTEROID_H

#include "body.h"
#include "particleSystem.h"

class Asteroid
{
	int health;
	TransformComponent *transform;
	ParticleSystem *explosion;
	Collider *collider;
	Body *body;
	DrawComponent *drawer;
	bool destroy;
	int tick = 0;
public:
	Asteroid() {}
	Asteroid(int _health, Collider *_collider, Body *_body, TransformComponent *_transform, DrawComponent *_drawer)
	{
		health = _health;
		transform = _transform;
		collider = _collider;
		body = _body;
		drawer = _drawer;
		destroy = false;
		explosion = NULL;
	}

	void SetHealth(int _health){health = _health;}
	void SetExplosion(ParticleSystem *_explosion){explosion = _explosion;}
	void SetCollider(Collider *_collider){collider = _collider;}
	void SetBody(Body *_body) { body = _body; }
	void SetTransform(TransformComponent *_t){transform = _t;}
	void SetDrawer(DrawComponent *_drawer) { drawer = _drawer; }
	void Destroy(){destroy = true;}

	int GetHealth(){return health;}
	ParticleSystem *GetExplosion(){return explosion;}
	Collider *GetCollider(){return collider;}
	Body *GetBody() { return body; }
	TransformComponent *GetTransform(){return transform;}
	DrawComponent *GetDrawer() { return drawer; }
	bool GetDestroy(){return destroy;}

	void Update()
	{
		float vMod = sqrt(transform->GetPosition().x*transform->GetPosition().x + transform->GetPosition().y*transform->GetPosition().y);
		if (collider->DetectCollision() || vMod > 2048)
		{
			transform->Destroy();
			collider->Destroy();
			Destroy();
			TransformComponent *t = new TransformComponent();
			*t = *(explosion->GetTransform());
			t->SetPosition(transform->GetPosition());
			transforms.push_back(t);
			ParticleSystem *p = new ParticleSystem();
			*p = *explosion;
			p->SetTransform(t);
			p->Explode();
			particleSystems.push_back(p);
			explosion = p;
		}
	}
};

std::list<Asteroid*> asteroidControllers;
std::list<Asteroid*>::iterator asteroidControllersIt;

Asteroid *CreateAsteroidController(int _health, Collider *_collider, Body *_body, TransformComponent *_transform, DrawComponent *_drawer )
{
	Asteroid *c = new Asteroid(_health, _collider, _body, _transform, _drawer);
	asteroidControllers.push_back(c);
	return c;
}
#endif