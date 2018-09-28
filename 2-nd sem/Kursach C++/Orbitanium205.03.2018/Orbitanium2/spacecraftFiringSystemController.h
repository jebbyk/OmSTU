#ifndef S_F_S_C_H
#define S_F_S_C_H

#include "spacecraft.h"
#include "particleSystem.h"

class FireController {
	TransformComponent *t;
	TransformComponent *pt;
	ParticleSystem *particleSystem;
	bool destroy;
public:
	FireController() {	}
	FireController(TransformComponent *_t, TransformComponent *_pt, ParticleSystem *_particleSystem) 
	{
		t = _t;
		pt = _pt;
		particleSystem = _particleSystem;
		destroy = false;
	}

	void Destroy(){destroy = true;}
	void Update()
	{
		if (!pt->GetDestroy()) { t->SetPosition(pt->GetPosition()); }
		else { t->Destroy(); }
	}
	void SetParentTransform(TransformComponent *_pt){pt = _pt;}
	void SetTransform(TransformComponent *_t){t = _t;}
	void SetParticleSystem(ParticleSystem *_particleSystem){particleSystem = _particleSystem;}

	bool GetDestroy(){return destroy;}
	TransformComponent *GetParentTransform(){return pt;}
	TransformComponent *GetTransform(){return t;}
	ParticleSystem *GetParticleSystem() { return particleSystem; }
};

std::list<FireController*> fireControllers;
std::list<FireController*>::iterator fireControllersIt;

FireController *CreateFireController(TransformComponent *_t, TransformComponent *_pt, ParticleSystem *_particleSystem)
{
	FireController *c = new FireController(_t, _pt, _particleSystem);
	fireControllers.push_back(c);
	return c;
}
#endif