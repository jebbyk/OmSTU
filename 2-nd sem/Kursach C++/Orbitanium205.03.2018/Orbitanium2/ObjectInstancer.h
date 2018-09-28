#ifndef OBJECT_INSTANCER_H
#define OBJECT_INSTANCER_H

#include "ComponentContainer.h"

void InstanceParticle(ParticleController *particleController)
{
	GameObject *particle = CreateGameObject();

	particle->SetTransform(particleController->GetTransform());
	particle->SetCollider(particleController->GetCollider());
	particle->SetDrawComponent(particleController->GetDrawer());
	particle->SetBody(particleController->GetBody());
	particle->SetParticleController(particleController);
}
void InstanceParticleSystem(ParticleSystem *_particleSystem)
{
	GameObject *particleSystem = CreateGameObject();
	particleSystem->SetTransform(_particleSystem->GetTransform());
	particleSystem->SetParticleSystem(_particleSystem);
}

void InstanceExplosionLight(ExplosionLight *_light)
{
	GameObject *explosionLight = CreateGameObject();
	explosionLight->SetTransform(_light->GetTransform());
	explosionLight->SetDrawComponent(_light->GetDrawer());
	explosionLight->SetExplosionLightController(_light);
}

void InstanceBullet(Bullet *_bullet)
{
	GameObject *bullet = CreateGameObject();
	bullet->SetTransform(_bullet->GetTransform());
	bullet->SetCollider(_bullet->GetCollider());
	bullet->SetBody(_bullet->GetBody());
	bullet->SetDrawComponent(_bullet->GetDrawer());
	bullet->SetBulletController(_bullet);
}

void InstanceAsteroid(Asteroid *_asteroid, FireController *_particleSystem)
{
	GameObject *asteroid = CreateGameObject();
	GameObject *particleSystem = CreateGameObject();

	asteroid->SetTransform(_asteroid->GetTransform());
	asteroid->SetDrawComponent(_asteroid->GetDrawer());
	asteroid->SetCollider(_asteroid->GetCollider());
	asteroid->SetBody(_asteroid->GetBody());
	asteroid->SetAteroidController(_asteroid);

	particleSystem->SetTransform(_particleSystem->GetTransform());
	particleSystem->SetParticleSystem(_particleSystem->GetParticleSystem());
	particleSystem->SetFireController(_particleSystem);
}

void UpdateInstancer()
{
	for (particleSystemsIt = particleSystems.begin(); particleSystemsIt != particleSystems.end(); ++particleSystemsIt)
	{
		while((*particleSystemsIt)->GetClock() > 1)
		{
			
			(*particleSystemsIt)->SetClock((*particleSystemsIt)->GetClock()-1);
			(*particleSystemsIt)->PrepareNewParticle();
			InstanceParticle((*particleSystemsIt)->GetP());
		}
		if ((*particleSystemsIt)->GetExplode())
		{
			(*particleSystemsIt)->GetTransform()->Destroy();
		}
	}

	for (asteroidControllersIt = asteroidControllers.begin(); asteroidControllersIt != asteroidControllers.end(); ++asteroidControllersIt)
	{
		if ((*asteroidControllersIt)->GetDestroy())
		{
			InstanceParticleSystem((*asteroidControllersIt)->GetExplosion());
		}
	}

	for (weaponsIterator = weapons.begin(); weaponsIterator != weapons.end(); ++weaponsIterator)
	{
		if ((*weaponsIterator)->GetNewBullet() != NULL)
		{
			InstanceBullet((*weaponsIterator)->GetNewBullet());
			InstanceParticleSystem((*weaponsIterator)->GetNewFireEffect());
			(*weaponsIterator)->SetNewBullet(NULL);
		}
	}

	for (bulletsIterator = bullets.begin(); bulletsIterator != bullets.end(); ++bulletsIterator)
	{
		if ((*bulletsIterator)->GetTransform()->GetDestroy())
		{
			InstanceParticleSystem((*bulletsIterator)->GetNewExplosion());
			InstanceExplosionLight((*bulletsIterator)->GetNewExplosionLight());
		}
	}

	for (asteroidSpawnersIt = asteroidSpawners.begin(); asteroidSpawnersIt != asteroidSpawners.end(); ++asteroidSpawnersIt)
	{
		if ((*asteroidSpawnersIt)->GetNewAsteroid() != NULL)
		{
			InstanceAsteroid((*asteroidSpawnersIt)->GetNewAsteroid(), (*asteroidSpawnersIt)->GetNewFireController());
			(*asteroidSpawnersIt)->SetNewAsteroid(NULL);
		}
	}
}
#endif

