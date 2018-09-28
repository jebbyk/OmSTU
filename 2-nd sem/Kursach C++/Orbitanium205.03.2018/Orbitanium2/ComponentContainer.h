#ifndef COMPONENT_CONTAINER_H
#define COMPONENT_CONATINER_H

#include <SFML/Graphics.hpp>
#include "PlayerController.h"
#include "backgroundController.h"
#include "Button.h"
#include "window.h"
#include "spacecraftFiringSystemController.h"
#include "Explosion.h"
#include "engineParticleSystemController.h"
#include "weapon.h"
#include "AsteroidSpawner.h"

class GameObject{
	TransformComponent *transform;
	Body *body;
	Collider *collider;
	DrawComponent *draw;
	Spacecraft *spacecraft;
	ParticleController *particleController;
	ParticleSystem *particleSystem;
	BackgroundController *backgroundController;
	FireController *fireController;
	Button *button;
	TextWindow *textWindow;
	FireLight *fireLightController;
	WeaponLight *weaponLightController;
	ExplosionLight *explosionLightController;
	Asteroid *asteroidController;
	ExplosionController *explosionController;
	PlayerController *playerController;
	EngineParticleController *engineParticleController;
	Bullet *bulletController;
	Weapon *weaponController;
	AsteroidSpawner *asteroidSpawnerController;
	bool destroy = false;
	String tag;
public:
	GameObject()
	{
		transform = NULL;
		body = NULL;
		collider = NULL;
		draw = NULL;
		spacecraft = NULL;
		particleSystem = NULL;
		particleController = NULL;
		backgroundController = NULL;
		fireController = NULL;
		button = NULL;
		textWindow = NULL;
		fireLightController = NULL;
		asteroidController = NULL;
		explosionController = NULL;
		playerController = NULL;
		bulletController = NULL;
		weaponController = NULL;
		weaponLightController = NULL;
		explosionLightController = NULL;
		asteroidSpawnerController = NULL;
	}

	void SetTransform(TransformComponent *_trnasform)
	{
		transform = _trnasform;
		tag = transform->GetTag();
	}
	void SetDrawComponent(DrawComponent *_draw){draw = _draw;}
	void SetCollider(Collider *_collider){collider = _collider;}
	void SetBody(Body *_body){body = _body;}
	void SetSpacecraft(Spacecraft *_spacecraft){spacecraft = _spacecraft;}
	void SetParticleController(ParticleController *_particleController){particleController = _particleController;}
	void SetBackgroundController(BackgroundController *_backgroundController){backgroundController = _backgroundController;}
	void SetButtonController(Button *_button){button = _button;}
	void SetParticleSystem(ParticleSystem *_particleSystem){particleSystem = _particleSystem;}
	void SetFireController(FireController *_fireController){fireController = _fireController;}
	void SetTextWindowController(TextWindow *_textWindow){textWindow = _textWindow;}
	void SetFireLitghtController(FireLight *_fireLightController){fireLightController = _fireLightController;}
	void SetAteroidController(Asteroid *_asteriodController){asteroidController = _asteriodController;}
	void SetExplosionController(ExplosionController *_explosionController){explosionController = _explosionController;}
	void SetPlayerController(PlayerController *_playerController){playerController = _playerController;}
	void SetEngineParticleController(EngineParticleController *_engineParticleController){engineParticleController = _engineParticleController;}
	void SetBulletController(Bullet *_bulletController){bulletController = _bulletController;}
	void SetWeaponController(Weapon *_weapon) { weaponController = _weapon; }
	void SetWeaponLightController(WeaponLight *_weaponLight) { weaponLightController = _weaponLight; }
	void SetExplosionLightController(ExplosionLight *_explosionLight) { explosionLightController = _explosionLight; }
	void SetAsteroidSpawnerController(AsteroidSpawner *_asteroidSpawner) { asteroidSpawnerController = _asteroidSpawner; }

	TransformComponent* GetTransformComponent(){return transform;}
	DrawComponent* GetDrawComonent(){return draw;}
	Collider* GetCollider() {return collider;}
	Body *GetBody(){return body;}
	Spacecraft *GetSpacecraft(){return spacecraft;}
	ParticleController *GetParticleController(){return particleController;}
	BackgroundController *GetBackgroundController(){return backgroundController;}
	Button *GetButton() { return button; }
	TextWindow *GetTextWindow(){return textWindow;}
	FireLight *GetFireLightController(){return fireLightController;}
	Asteroid *GetAsteroidController(){return asteroidController;}
	ExplosionController *GetExplosionController(){return explosionController;}
	PlayerController *GetPlayerController(){return playerController;}
	EngineParticleController *GetEngineParticleController(){return engineParticleController;}
	bool GetDestroy(){return destroy;}
	ParticleSystem *GetParticleSystem(){return particleSystem;}
	FireController *GetFireController(){return fireController;}
	Bullet *GetBulletController(){return bulletController;}
	Weapon *GetWeaponController() { return weaponController; }
	WeaponLight *GetWeaponLightController() { return weaponLightController; }
	ExplosionLight *GetExplosionLightController() { return explosionLightController; }
	AsteroidSpawner *GetAsteroidSpwnerController() { return asteroidSpawnerController; }
	String *GetTag(){return &tag;}
	void Update(RenderWindow &window)
	{
		//printf("\n\n%s \n", transform->GetName().toAnsiString().c_str());
		if (backgroundController != NULL){backgroundController->Update();}
		if (textWindow != NULL){textWindow->Update(window);}
		if (button != NULL){button->Update(window);}
		if (body != NULL){body->Update();}
		if (spacecraft != NULL){spacecraft->Update();}
		if (particleSystem != NULL){particleSystem->Update();}
		if (particleController != NULL){particleController->Update();}
		if (fireController != NULL){fireController->Update();}
		if (fireLightController != NULL){fireLightController->Update();}
		if (asteroidController != NULL){asteroidController->Update();}
		if (explosionController != NULL){explosionController->Update();}
		if (playerController != NULL){playerController->Update();}
		if (engineParticleController != NULL){engineParticleController->Update();}
		if (bulletController != NULL){bulletController->Update();}
		if (weaponController != NULL) { weaponController->Update(); }
		if (weaponLightController != NULL) { weaponLightController->Update(); }
		if (explosionLightController != NULL) { explosionLightController->Update(); }
		if (asteroidSpawnerController != NULL) { asteroidSpawnerController->Update(); }
		if (draw != NULL){draw->Draw(window);}
	}

	void Destroy()
	{
		if (transform != NULL){transform->Destroy();}
		if (collider != NULL){collider->Destroy();}
		if (draw != NULL){draw->Destroy();}
		if (body != NULL){body->Destroy();}
		if (spacecraft != NULL){spacecraft->Destroy();}
		if (particleController != NULL){particleController->Destroy();}
		if (particleSystem != NULL){particleSystem->Destroy();}
		if (backgroundController != NULL) { backgroundController->Destroy(); }
		if (button != NULL){button->Destroy();}
		if (textWindow != NULL){textWindow->Destroy();}
		if (fireController != NULL){fireController->Destroy();}
		if (asteroidController != NULL){asteroidController->Destroy();}
		if (fireLightController != NULL){fireLightController->Destroy();}
		if (explosionController != NULL){explosionController->Destroy();}
		if (playerController != NULL){playerController->Destroy();}
		if (engineParticleController != NULL){engineParticleController->Destroy();}
		if (bulletController != NULL){bulletController->Destroy();}
		if (weaponController != NULL) { weaponController->Destroy(); }
		if (weaponLightController != NULL) { weaponLightController->Destroy(); }
		if (explosionLightController != NULL) { explosionLightController->Destroy(); }
		if (asteroidSpawnerController != NULL) { asteroidSpawnerController->Destroy(); }
		destroy = true;
	}
};

std::list<GameObject*> objects;
std::list<GameObject*>::iterator objectsIterator;

GameObject* CreateGameObject()
{
	GameObject *c = new GameObject();
	objects.push_back(c);
	return c;
}

bool TryFindWithTag(String _t)
{
	bool b = false;
	for (objectsIterator = objects.begin(); objectsIterator != objects.end(); objectsIterator++)
	{
		if ((*objectsIterator)->GetTransformComponent()->GetTag() == _t){b = true;}
	}
	return b;
}

GameObject* FindGameObjectWithTag(String _tag)
{
	for (objectsIterator = objects.begin(); objectsIterator != objects.end(); objectsIterator++)
	{
		if (*((*objectsIterator)->GetTag()) == _tag)
		{
			return *objectsIterator;
			break;
		}
	}
}
#endif