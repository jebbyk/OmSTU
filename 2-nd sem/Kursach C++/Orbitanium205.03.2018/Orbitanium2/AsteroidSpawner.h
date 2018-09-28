#ifndef ASTEROID_SPAWNER_H
#define ASTEROID_SPAWNER_H

#include "Asteroid.h"
#include "Light.h"

class AsteroidSpawner{
	TransformComponent *parentTransform;
	TransformComponent *transform;
	Asteroid *asteroidPrefab;
	FireController *refFireController;
	FireLight *asteroidLight;
	bool destroy;
	float rate;
	float minVel;
	float maxVel;
	float timer;
	float spawningRadius;
	float dificulty = 1.0f;

	Asteroid *newAsteroid;
	FireController *newFireController;
public:
	AsteroidSpawner() {}
	AsteroidSpawner(float _rate, float _spawningRadius, float _minVel, float _maxVel, TransformComponent *_parentTransform, TransformComponent *_transform, Asteroid *_asteroidPrefab, FireController *_asteroidParticleSystem)
	{
		parentTransform = _parentTransform;
		spawningRadius = _spawningRadius;
		rate = _rate;
		minVel = _minVel;
		maxVel = _maxVel;
		transform = _transform;
		asteroidPrefab = _asteroidPrefab;
		refFireController = _asteroidParticleSystem;
		destroy = false;
		newAsteroid = NULL;
		timer = 1;
	}

	void SetNewAsteroid(Asteroid *_asteroid){newAsteroid = _asteroid;}

	bool GetDestroy(){return destroy;}
	float GetTimer() { return timer; }
	Asteroid *GetNewAsteroid() { return newAsteroid; }
	FireController *GetNewFireController() { return newFireController; }

	void Destroy(){destroy = true;}
	void Update()
	{
		timer += deltaTime * rate;
		if (timer > 1)
		{
			rate += 0.01;
			timer = 0;
			TransformComponent *newTransform = new TransformComponent();
			Collider *newCollider = new Collider();
			Body *newBody = new Body();
			DrawComponent *newDrawer = new DrawComponent();

			float newRotation = 180 + zeroRand(180);

			Vector2f v;
			v.x = cos(((newRotation - 90) / 360)*3.1416 * 2);
			v.y = sin(((newRotation - 90) / 360)*3.1416 * 2);
			//float vMod = sqrt(v.x * v.x + v.y*v.y);
			//v = v / vMod;//единичный вектор 
			v *= spawningRadius;

			*newTransform = *(asteroidPrefab->GetTransform());
			newTransform->SetPosition(parentTransform->GetPosition() + v);
			transforms.push_back(newTransform);
			*newCollider = *(asteroidPrefab->GetCollider());
			newCollider->SetTransform(newTransform);
			colliders.push_back(newCollider);
			*newBody = *(asteroidPrefab->GetBody());
			newBody->SetTransform(newTransform);

			v = parentTransform->GetPosition() - newTransform->GetPosition();
			//v = newTransform->GetPosition() - parentTransform->GetPosition();
			float vMod = sqrt(v.x * v.x + v.y*v.y);
			v = v / vMod;

			//newRotation = zeroRand(90);
			//v.x += cos(((newRotation - 90) / 360)*3.1416 * 2);
			//v.y += sin(((newRotation - 90) / 360)*3.1416 * 2);
			v *=  frand(minVel, maxVel);

			newBody->SetVelocity(v);

			bodies.push_back(newBody);
			*newDrawer = *(asteroidPrefab->GetDrawer());
			newDrawer->SetTransform(newTransform);
			drawings.push_back(newDrawer);
			newAsteroid = new Asteroid();
			*newAsteroid = *asteroidPrefab;
			newAsteroid->SetTransform(newTransform);
			newAsteroid->SetCollider(newCollider);
			newAsteroid->SetBody(newBody);
			newAsteroid->SetDrawer(newDrawer);
			asteroidControllers.push_back(newAsteroid);



			newTransform = new TransformComponent();
			ParticleSystem *newParticleSystem = new ParticleSystem();

			*newTransform = *(refFireController->GetTransform());
			newTransform->SetPosition(transform->GetPosition());
			transforms.push_back(newTransform);
			*newParticleSystem = *(refFireController->GetParticleSystem());
			newParticleSystem->SetTransform(newTransform);
			particleSystems.push_back(newParticleSystem);
			newFireController = new FireController();
			*newFireController = *refFireController;
			newFireController->SetTransform(newTransform);
			newFireController->SetParticleSystem(newParticleSystem);
			newFireController->SetParentTransform(newAsteroid->GetTransform());
			fireControllers.push_back(newFireController);
		}
	}

};

std::list<AsteroidSpawner*> asteroidSpawners;
std::list<AsteroidSpawner*>::iterator asteroidSpawnersIt;

AsteroidSpawner *CreateAsteroidSpwner(float _rate, float _spawningRadius, float _minVel, float _maxVel, TransformComponent *_parentTransform, TransformComponent *_transform, Asteroid *_asteroidPrefab, FireController *_asteroidParticleSystem)
{
	AsteroidSpawner *c = new AsteroidSpawner(_rate,_spawningRadius, _minVel, _maxVel, _parentTransform, _transform, _asteroidPrefab, _asteroidParticleSystem);
	asteroidSpawners.push_back(c);
	return c;
}
#endif