#ifndef WEAPON_H
#define WEAPON_H

#include "bullet.h"
#include  "spacecraft.h"

class Weapon {
	TransformComponent *t;
	Spacecraft *s;
	TransformComponent *st;
	Bullet *bulletPrefab;
	ParticleSystem *fireEffect;
	WeaponLight *weaponLightController;
	float rnd;
	float rate;
	float weaponPower;
	float timer;
	bool destroy;
	Bullet *newBullet;
	ParticleSystem *newFireEffect;
public:
	Weapon() {}
	Weapon(float _rnd, float _rate, float _power, TransformComponent *_t, Spacecraft *_s, Bullet *_bulletPrefab, ParticleSystem *_fireEffect)
	{
		rnd = _rnd;
		rate = _rate;
		weaponPower = _power;
		t = _t;
		s = _s;
		st = s->GetTransform();
		bulletPrefab = _bulletPrefab;
		fireEffect = _fireEffect;
		timer = 0;
	}

	void SetRandom(float _rnd) { rnd = _rnd; }
	void SetRate(float _rate) { rate = _rate; }
	void SetTransform(TransformComponent *_t) { t = _t; }
	void SetParentSpacecraft(Spacecraft *_s) { s = _s; }
	void SetFireEffect(ParticleSystem *_fireEffect) { fireEffect = _fireEffect; }
	void SetNewBullet(Bullet *_b)
	{
		newBullet = _b;
	}
	void SetPower(float _power)
	{
		weaponPower = _power;
	}
	void SetWeaponLightController(WeaponLight *_weaponLight) { weaponLightController = _weaponLight; }

	bool GetDestroy() { return destroy; }
	Bullet *GetNewBullet() { return newBullet; }
	ParticleSystem *GetNewFireEffect() { return newFireEffect; }
	TransformComponent *GetTransform() { return t; }

	void Destroy() { destroy = true; }
	void Update()
	{
		timer += deltaTime * rate;
		t->SetPosition(st->GetPosition() + s->GetDirection() * 16.0f);
		t->SetRotation(st->GetRotation());
		if (timer > 1 && s->GetShoot())
		{
			timer = 0;
			TransformComponent *newTransform = new TransformComponent();
			Collider *newCollider = new Collider();
			Body *newBody = new Body();
			DrawComponent *newDrawer = new DrawComponent();
			newBullet = new Bullet();

			*newTransform = *(bulletPrefab->GetTransform());
			newTransform->SetPosition(t->GetPosition());
			newTransform->SetRotation(t->GetRotation());
			transforms.push_back(newTransform);
			*newDrawer = *(bulletPrefab->GetDrawer());
			newDrawer->SetTransform(newTransform);
			*newCollider = *(bulletPrefab->GetCollider());
			newCollider->SetTransform(newTransform);
			colliders.push_back(newCollider);
			*newBody = *(bulletPrefab->GetBody());
			newBody->SetTransform(newTransform);
			newBody->SetVelocity(s->GetBody()->GetVelocity()/2.0f + s->GetDirection()*weaponPower + zeroRand2f(weaponPower*rnd));
			bodies.push_back(newBody);

			*newBullet = *bulletPrefab;
			newBullet->SetTransform(newTransform);
			newBullet->SetBody(newBody);
			newBullet->SetCollider(newCollider);
			newBullet->SetDrawer(newDrawer);
			bullets.push_back(newBullet);



			TransformComponent *newTransform2 = new TransformComponent();
			*newTransform2 = *(fireEffect->GetTransform());
			newTransform2->SetPosition(t->GetPosition());
			transforms.push_back(newTransform2);
			newFireEffect = new ParticleSystem();
			*newFireEffect = *fireEffect;
			newFireEffect->SetTransform(newTransform2);
			newFireEffect->Explode();
			newFireEffect->SetVelocity(s->GetBody()->GetVelocity() + s->GetDirection()*weaponPower / 8.0f);
			particleSystems.push_back(newFireEffect);
	
			weaponLightController->SetLightIntance(255.0f);
		}
	}
};

std::list<Weapon*> weapons;
std::list<Weapon*>::iterator weaponsIterator;

Weapon *CreateWeapon(float _rnd, float _rate, float _power, TransformComponent *_t, Spacecraft *_s, Bullet *_bulletPrefab, ParticleSystem *_fireEffect)
{
	Weapon *c = new Weapon(_rnd, _rate, _power, _t, _s, _bulletPrefab, _fireEffect);
	weapons.push_back(c);
	return c;
}
#endif