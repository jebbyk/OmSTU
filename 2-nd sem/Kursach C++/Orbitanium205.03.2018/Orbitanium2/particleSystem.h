#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "particleController.h"
#include "extMath.h"

using namespace sf;

class ParticleSystem {

	ParticleController *refParticle;
	TransformComponent *transform;

	Vector2f spawningArea;
	Vector2f velocity;
	float scaleRnd;
	float rotationRnd;
	float healthRnd;
	float velocityRnd;
	float angVelRnd;
	float rate;
	float curentClock = 0;
	bool emit;

	ParticleController *p;
	TransformComponent *t;
	Collider *c;
	DrawComponent *d;
	Body *b;
	bool destroy = false;
	bool explode = false;

public:
	ParticleSystem() {}
	ParticleSystem(float _rate, float _scalRnd, float stRotRnd, float healRnd, float velRnd, float _angVelRnd, Vector2f _spawningArea, bool _emit, ParticleController *_refParticle, TransformComponent *tr)
	{
		rate = _rate;
		scaleRnd = _scalRnd;
		rotationRnd = stRotRnd;
		healthRnd = healRnd;
		velocityRnd = velRnd;
		angVelRnd = _angVelRnd;
		transform = tr;
		refParticle = _refParticle;
		spawningArea = _spawningArea;
		emit = _emit;
		destroy = false;
	}

	void SetRate(float _rate){rate = _rate;}
	void SetRefParticle(ParticleController *_particle){refParticle = _particle;}
	void SetArea(Vector2f _area){spawningArea = _area;}
	void SetClock(float _clock){curentClock = _clock;}
	void Destroy(){destroy = true;}
	void SetEmit(bool _emit){emit = _emit;}
	void SetTransform(TransformComponent *_t){transform = _t;}
	void SetVelocity(Vector2f _v){velocity = _v;}

	TransformComponent *GetTransform(){return transform;}
	float GetRate(){return rate;}
	float GetClock(){return curentClock;}
	ParticleController *GetP(){return p;}
	bool GetDestroy(){return destroy;}
	bool GetEmit(){return emit;}
	bool GetExplode(){return explode;}
	void Update()
	{
		if (!explode )
		{
			if (emit){curentClock += rate * deltaTime;}
		}
	}
	void Explode()
	{
		curentClock = rate;
		explode = true;
	}
	void PrepareNewParticle()
	{
		p = new ParticleController();
		t = new TransformComponent();
		c = new Collider();
		b = new Body();
		d = new DrawComponent();
		*p = *refParticle;
		*t = *(refParticle->GetTransform());
		transforms.push_back(t);
		*c = *(refParticle->GetCollider());
		c->SetTransform(t);
		colliders.push_back(c);
		*b = *(refParticle->GetBody());
		b->SetTransform(t);
		b->SetCollider(c);
		bodies.push_back(b);
		*d = *(refParticle->GetDrawer());
		d->SetTransform(t);
		drawings.push_back(d);

		p->SetTransform(t);
		p->SetCollider(c);
		p->SetBody(b);
		p->SetDrawer(d);


		p->GetTransform()->SetPosition(transform->GetPosition() + zeroRand2f(spawningArea/2.0f));
		p->GetTransform()->SetRotation(transform->GetRotation() + zeroRand(rotationRnd/2.0f));
		p->GetTransform()->SetScale(blurRand2f(refParticle->GetTransform()->GetScale(), scaleRnd));
		p->GetBody()->SetVelocity(refParticle->GetBody()->GetVelocity() + zeroRand2f(velocityRnd) + velocity);
		p->GetBody()->SetAngularVelocity(refParticle->GetBody()->GetAngularVelocity() + zeroRand(angVelRnd));
		p->SetHealth(blurRand(refParticle->GetHealth(), healthRnd));
		p->SetStartColor(refParticle->GetColor());
		p->SetEndColor(refParticle->GetEndColor());
		particles.push_back(p);
	}
};

std::list<ParticleSystem*> particleSystems;
std::list<ParticleSystem*>::iterator particleSystemsIt;

ParticleSystem *CreateParticleSystem(float _rate, float _scalRnd, float stRotRnd, float healRnd, float velRnd, float angVelRnd, Vector2f _spawningArea, bool _emit, ParticleController *_refParticle, TransformComponent *tr)
{
	ParticleSystem *c = new ParticleSystem(_rate, _scalRnd, stRotRnd, healRnd, velRnd, angVelRnd, _spawningArea, _emit, _refParticle, tr);
	particleSystems.push_back(c);
	return c;
}
#endif