#ifndef LIGHT_H
#define LIGHT_H

#include "particleSystem.h"

class FireLight
{
	float intanceRnd;
	float attenuationSpeed;
	float normalScale;
	float normalLight;
	float curentLight;
	float c;
	TransformComponent *t;
	DrawComponent *d;
	ParticleSystem *p;
	bool destroy;

public:
	FireLight(float _intanceRnd, float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d, ParticleSystem *_p)
	{
		intanceRnd = _intanceRnd;
		attenuationSpeed = _attenuationSpeed;
		t = _t;
		normalScale = t->GetScale().x;
		d = _d;
		normalLight = d->GetColor().a;
		p = _p;
		destroy = false;
	}

	void SetIntanceRnd(float _intanceRnd){intanceRnd = _intanceRnd;}
	void SetAttenuationSpeed(float _attenuationSpeed){attenuationSpeed = _attenuationSpeed;}
	void SetTransform(TransformComponent *_t){t = _t;}
	void SetDrawer(DrawComponent *_d){d = _d;}
	void SetParticleSystem(ParticleSystem *_p){p = _p;}
	void Destroy(){destroy = true;}
	void Update()
	{
		t->SetPosition(p->GetTransform()->GetPosition());
		if (p->GetEmit()){curentLight += (((normalLight - curentLight) * attenuationSpeed))*deltaTime;}
		else{curentLight -= curentLight * attenuationSpeed * deltaTime;}
		c = curentLight + curentLight * zeroRand(intanceRnd);
		if (c > 255) c = 255;
		d->SetColor(Color(d->GetColor().r, d->GetColor().g, d->GetColor().b, c));
	}

	bool GetDestroy(){return destroy;}
};

std::list<FireLight*> fireLights;
std::list<FireLight*>::iterator fireLightsIterator;

FireLight *CreateFireLight(float _intanceRnd, float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d, ParticleSystem *_p)
{
	FireLight *c = new FireLight(_intanceRnd, _attenuationSpeed, _t, _d, _p);
	fireLights.push_back(c);
	return c;
}

class WeaponLight {
	float attenuationSpeed;
	float curentLight;
	float c;
	TransformComponent *t;
	DrawComponent *d;
	TransformComponent *pt;
	bool destroy;

public:
	WeaponLight( float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d, TransformComponent *_pt)
	{
		pt = _pt;
		attenuationSpeed = _attenuationSpeed;
		t = _t;
		d = _d;
		destroy = false;
	}

	void SetAttenuationSpeed(float _attenuationSpeed){attenuationSpeed = _attenuationSpeed;}
	void SetTransform(TransformComponent *_t){t = _t;}
	void SetDrawer(DrawComponent *_d){d = _d;}
	void SetParentTransform(TransformComponent *_t){pt = _t;}
	void SetLightIntance(float _f){curentLight = _f;}
	void Destroy(){destroy = true;}
	void Update()
	{
		t->SetPosition(pt->GetPosition());
		curentLight -= curentLight * attenuationSpeed * deltaTime;
		d->SetColor(Color(d->GetColor().r, d->GetColor().g, d->GetColor().b, curentLight));
	}

	bool GetDestroy(){return destroy;}

};

std::list<WeaponLight*> weaponLights;
std::list<WeaponLight*>::iterator weaponLightsIterator;

WeaponLight *CreateWeaponLight(float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d, TransformComponent *_pt)
{
	WeaponLight *c = new WeaponLight( _attenuationSpeed, _t, _d, _pt);
	weaponLights.push_back(c);
	return c;
}

class ExplosionLight {
	float attenuationSpeed;
	float normalLight;
	float curentLight;
	float c;
	TransformComponent *t;
	DrawComponent *d;
	bool destroy;
public:
	ExplosionLight() {}
	ExplosionLight(float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d)
	{
		attenuationSpeed = _attenuationSpeed;
		t = _t;
		d = _d;
		destroy = false;
		normalLight = 255;
		curentLight = 0;
	}

	void SetLightIntance(float _intance){curentLight = _intance;}
	void SetAttenuationSpeed(float _attenuationSpeed){attenuationSpeed = _attenuationSpeed;}
	void SetTransform(TransformComponent *_t){t = _t;}
	void SetDrawer(DrawComponent *_d){d = _d;}
	
	void Destroy(){destroy = true;}
	void Update()
	{
		curentLight -= curentLight * attenuationSpeed * deltaTime;
		d->SetColor(Color(d->GetColor().r, d->GetColor().g, d->GetColor().b, curentLight));
		if (curentLight < 0.1){t->Destroy();}
	}

	bool GetDestroy(){return destroy;}
	TransformComponent *GetTransform(){return t;}
	DrawComponent *GetDrawer(){return d;}
};

std::list<ExplosionLight*> explosionLights;
std::list<ExplosionLight*>::iterator explosionLightsIterator;

ExplosionLight *CreateExplosionLight(float _attenuationSpeed, TransformComponent *_t, DrawComponent *_d)
{
	ExplosionLight *c = new ExplosionLight(_attenuationSpeed, _t, _d);
	explosionLights.push_back(c);
	return c;
}
#endif