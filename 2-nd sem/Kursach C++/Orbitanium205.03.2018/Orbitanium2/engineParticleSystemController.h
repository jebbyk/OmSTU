#ifndef E_P_S_C_H
#define E_P_S_C_H

#include "particleSystem.h"
#include "spacecraft.h"

class EngineParticleController {
	Spacecraft *ps;
	TransformComponent *pt;
	TransformComponent *t;
	ParticleSystem *particleSystem;
	float power;
	bool destroy;
public:
	EngineParticleController() {}
	EngineParticleController(float _power, Spacecraft *_ps, TransformComponent *_t, ParticleSystem *_particleSystem)
	{
		power = _power;
		ps = _ps;
		pt = ps->GetTransform();
		t = _t;
		particleSystem = _particleSystem;
		destroy = false;
	}

	void SetParentSpacecraft(Spacecraft *_ps)
	{
		ps = _ps;
		pt = ps->GetTransform();
	}
	void SetTransform(TransformComponent *_t){t = _t;}
	void SetParticleSystem(ParticleSystem *_particleSystem){particleSystem = _particleSystem;}

	void Destroy(){destroy = true;}
	void Update()
	{
		if (!pt->GetDestroy()){t->SetPosition(pt->GetPosition() - ps->GetDirection()*ps->GetCollider()->GetRadius()*2.0f);}
		else { t->Destroy(); }

		if (ps->IsThrottled())
		{
			Vector2f direction;
			direction.x = cos(((pt->GetRotation() - 90) / 360)*3.1416 * 2);
			direction.y = sin(((pt->GetRotation() - 90) / 360)*3.1416 * 2);
			direction *= -(ps->GetEnginesPower())/3.0f * power;
			direction += ps->GetBody()->GetVelocity()/1.5f;
			particleSystem->SetVelocity(direction);
			particleSystem->SetEmit(true);
		}
		else { 
			particleSystem->SetEmit(false); 
		}
	}
	bool GetDestroy(){return destroy;}
};

std::list<EngineParticleController*> engineParticleControllers;
std::list<EngineParticleController*>::iterator engineParticleControllersIt;

EngineParticleController *CreateEngineParticleController(float _power, Spacecraft *_ps, TransformComponent *_t, ParticleSystem *_particleSystem)
{
	EngineParticleController *c = new EngineParticleController(_power, _ps, _t, _particleSystem);
	engineParticleControllers.push_back(c);
	return c;
}
#endif