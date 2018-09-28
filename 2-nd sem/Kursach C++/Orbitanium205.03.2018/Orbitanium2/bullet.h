#ifndef BULLET_H
#define BULLET_H

#include "Asteroid.h"
#include "spacecraft.h"
#include "Light.h"

class Bullet {
	int damage;
	float health;
	TransformComponent *t;
	Collider *c;
	Body *b;
	DrawComponent *d;
	ParticleSystem *explosion;
	ParticleSystem *newExplosion;
	ExplosionLight *explosionLight;
	ExplosionLight *newExplosionLight;
	bool destroy;
	Vector2f direction;
	TransformComponent *collisionTransform;

public:
	Bullet(){}
	Bullet(int _damage, float _health, TransformComponent *_t, Collider *_c, Body *_b, DrawComponent *_d, ParticleSystem *_explosion, ExplosionLight *_light)
	{
		damage = _damage;;
		health = _health;
		t = _t;
		c = _c;
		b = _b;
		d = _d;
		explosion = _explosion;
		explosionLight = _light;
		destroy = false;
		collisionTransform = NULL;
		newExplosion = NULL;
		newExplosionLight = NULL;
	}

	void SetTransform(TransformComponent *_t){t = _t;}
	void SetCollider(Collider *_c){c = _c;}
	void SetBody(Body *_b){b = _b;}
	void SetDrawer(DrawComponent *_d){d = _d;}
	void SetExplosion(ParticleSystem *_explosion){explosion = _explosion;}
	void SetDamage(int _damage){damage = _damage;}
	void SetHealth(float _health){health = _health;}
	
	bool GetDestroy(){return destroy;}
	TransformComponent *GetTransform(){return t;}
	Collider *GetCollider(){return c;}
	Body *GetBody(){return b;}
	DrawComponent *GetDrawer(){return d;}
	ParticleSystem *GetNewExplosion(){return newExplosion;}
	ExplosionLight *GetNewExplosionLight(){return newExplosionLight;}

	void Destroy(){destroy = true;}
	void Update()
	{
		direction = b->GetVelocity();
		t->SetRotation(-((atan2(direction.x, direction.y) * 180) / 3.1416) + 180);

		health -= deltaTime;
		
		if (c->DetectCollision(&collisionTransform) || health <= 0)
		{
			if (collisionTransform != NULL)
			{
				if (collisionTransform->GetTag() == "asteroid")
				{
					collisionTransform->Destroy();
					points += 10;
				}
			}

			t->Destroy();
			TransformComponent *_t = new TransformComponent();
			*_t = *(explosion->GetTransform());
			_t->SetPosition(t->GetPosition());
			transforms.push_back(_t);
			newExplosion = new ParticleSystem();
			*newExplosion = *explosion;
			newExplosion->SetTransform(_t);
			newExplosion->Explode();
			particleSystems.push_back(newExplosion);

			TransformComponent *_t2 = new TransformComponent();
			*_t2 = *(explosionLight->GetTransform());
			_t2->SetPosition(t->GetPosition());
			transforms.push_back(_t2);
			DrawComponent *newDrawer = new DrawComponent();
			*newDrawer = *(explosionLight->GetDrawer());
			newDrawer->SetTransform(_t2);
			drawings.push_back(newDrawer);
			newExplosionLight = new ExplosionLight();
			*newExplosionLight = *explosionLight;
			newExplosionLight->SetTransform(_t2);
			newExplosionLight->SetDrawer(newDrawer);
			newExplosionLight->SetLightIntance(255);
			explosionLights.push_back(newExplosionLight);
		}
	}
};

std::list<Bullet*> bullets;
std::list<Bullet*>::iterator bulletsIterator;

Bullet* CreateBullet(int _damage, float _health, TransformComponent *_t, Collider *_c, Body *_b, DrawComponent *_d, ParticleSystem *_explosion, ExplosionLight *_el)
{
	Bullet *c = new Bullet(_damage, _health, _t, _c, _b, _d, _explosion, _el);
	bullets.push_back(c);
	return c;
}
#endif