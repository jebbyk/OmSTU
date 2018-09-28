#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "body.h"
#include "UIController.h"
#include "extMath.h"

class Spacecraft{
	float hull;
	float shield;
	float energy;
	float enginesPower;

	float maxHull;
	float maxShield;
	float maxEnergy;

	bool shoot = false;
	bool throtle = false;
	Vector2f direction;

	TransformComponent *transform;
	Collider *collider;
	Body *body;

	bool destroy = false;

public:
	Spacecraft() {}
	Spacecraft(float _hull, float _shield,float _energy, float _enginesPower, TransformComponent *_transform, Collider *_collider, Body *_body)
	{
		hull = _hull;
		maxHull =_hull;
		shield = _shield;
		maxShield = _shield;
		energy = _energy;
		maxEnergy = _energy;
		enginesPower = _enginesPower;
		transform = _transform;
		collider = _collider;
		body = _body;
		destroy = false;
	}

	void SetHull(float _hull)
	{
		hull = _hull;
		if (hull > maxHull){hull = maxHull;}
	}
	void SetShield(float _shield)
	{
		shield = _shield;
		if (shield > maxShield){shield = maxShield;}
	}
	void SetMaxHull(float _maxHull){	maxHull = _maxHull;}
	void SetMaxShield(float _maxShield){maxShield = _maxShield;}
	void SetEnergy(float _energy)
	{
		energy = _energy;
		if (energy > maxEnergy){energy = maxEnergy;}
	}
	void SetMaxEnergy(float _energy){maxEnergy = _energy;	}
	void SetEnginesPower(float _power){enginesPower = _power;}
	void SetTransform(TransformComponent *_transform){transform = _transform;}
	void SetCollider(Collider *_collider){collider = _collider;}
	void SetBody(Body *_body){body = _body;}
	void SetThrotle(bool _b){throtle = _b;}
	void SetShoot(bool _b){shoot = _b;}
	void Destroy(){destroy = true;}

	float GetHull(){return hull;}
	float GetShield(){return shield;}
	float GetMaxHull(){return maxHull;}
	float GetMaxShield(){return maxShield;}
	float GetEnergy(){return energy;}
	float GetMaxEnergy(){return maxEnergy;}
	float GetEnginesPower(){return enginesPower;}
	TransformComponent* GetTransform(){return transform;}
	Collider* GetCollider(){return collider;}
	Body* GetBody(){return body;}
	Vector2f GetDirection(){return direction;}
	bool IsThrottled(){return throtle;}
	bool GetDestroy(){return destroy;}
	bool GetShoot(){return shoot;}

	void Update()
	{
		direction.x = cos(((transform->GetRotation() - 90) / 360) * 3.1426f * 2);
		direction.y = sin(((transform->GetRotation() - 90) / 360)*3.1416f * 2);

		if (throtle == true)
		{
			body->AddForce(direction * enginesPower);
		}
		if (collider->DetectCollision())
		{
			if (transform->GetTag() == "player")
			{
				curentSelectedMenu = 5;
			}
			loadMain = true;
			transform->Destroy();
			collider->Destroy();
			body->Destroy();
			Destroy();
		}
	}
};

std::list<Spacecraft*> spacecrafts;
std::list<Spacecraft*>::iterator spacecraftsIterator;

Spacecraft* CreateSpacecraft(float _hull, float _shield, float _energy, float _enginesPower, TransformComponent *_transform, Collider *_collider, Body *_body)
{
	Spacecraft *c = new Spacecraft(_hull, _shield, _energy, _enginesPower, _transform, _collider, _body);
	spacecrafts.push_back(c);
	return c;
}

void DeleteSpacecraft(Spacecraft *_spacecraft)
{
	delete(_spacecraft);
	_spacecraft = NULL;
}
#endif