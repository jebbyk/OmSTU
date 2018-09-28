#ifndef LVL1_H
#define LVL1_H

#include "ObjectInstancer.h"
#include "ImageLoader.h"


void LoadLevel_1()
{


	TransformComponent *backgroundTransform = CreateTransform(Vector2f(640, 360), Vector2f(0.5, 0.5), 0, "Background", "dekor");
	Texture *spaceTexture = LoadTexture("textures/space");
	DrawComponent *backgroundDrawer = CreateDraw(spaceTexture, BlendAlpha, backgroundTransform);
	BackgroundController *backgroundController = CreateBackgroundController(backgroundTransform);
	GameObject *background = CreateGameObject();
	background->SetTransform(backgroundTransform);
	background->SetDrawComponent(backgroundDrawer);
	background->SetBackgroundController(backgroundController);

	TransformComponent *sunTransform = CreateTransform(Vector2f(1460, 100), Vector2f(2, 2), 30, "Sun", "dekor");
	Texture *sunTexture = LoadTexture("textures/sun");
	DrawComponent *sunDrawer = CreateDraw(sunTexture, BlendAdd, sunTransform);
	sunDrawer->SetColor(Color(255, 200, 150, 255));
	BackgroundController *sunController = CreateBackgroundController(sunTransform);
	GameObject *sun = CreateGameObject();
	sun->SetTransform(sunTransform);
	sun->SetDrawComponent(sunDrawer);
	sun->SetBackgroundController(sunController);


	TransformComponent *planetTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.12, 0.12), 0, "Mars", "planet");
	Texture *planetTexture = LoadTexture("textures/Planet_Mars");
	DrawComponent *planetDrawer = CreateDraw(planetTexture, BlendAlpha, planetTransform);
	Collider *planetCollider = CreateCollider(56, planetTransform);
	Body *planetBody = CreateBody(1000000, Vector2f(0, 0), 0, false, planetTransform, planetCollider);
	GameObject *planet = CreateGameObject();
	planet->SetTransform(planetTransform);
	planet->SetDrawComponent(planetDrawer);
	planet->SetCollider(planetCollider);
	planet->SetBody(planetBody);


	TransformComponent *moonTransform = CreateTransform(Vector2f(-90, 0), Vector2f(0.03, 0.03), 0, "Fobos", "satellite");
	Texture *moonTexture = LoadTexture("textures/moon");
	DrawComponent *moonDrawer = CreateDraw(moonTexture, BlendAlpha, moonTransform);
	moonDrawer->SetColor(Color(255, 150, 50, 255));
	Collider *moonCollider = CreateCollider(10, moonTransform);
	Body *moonBody = CreateBody(6000, Vector2f(0, 120), 0, true, moonTransform, moonCollider);
	GameObject *moon = CreateGameObject();
	moon->SetTransform(moonTransform);
	moon->SetCollider(moonCollider);
	moon->SetBody(moonBody);
	moon->SetDrawComponent(moonDrawer);

	TransformComponent *moon2Transform = CreateTransform(Vector2f(0, 360), Vector2f(0.02, 0.02), 0, "Deymos", "satellite");
	Texture *moon2Texture = LoadTexture("textures/moon");
	DrawComponent *moon2Drawer = CreateDraw(moon2Texture, BlendAlpha, moon2Transform);
	Collider *moon2Collider = CreateCollider(8, moon2Transform);
	Body *moon2Body = CreateBody(4000, Vector2f(50, 0), 0, true, moon2Transform, moon2Collider);
	GameObject *moon2 = CreateGameObject();
	moon2->SetTransform(moon2Transform);
	moon2->SetCollider(moon2Collider);
	moon2->SetBody(moon2Body);
	moon2->SetDrawComponent(moon2Drawer);



	TransformComponent *spacecraftTransform = CreateTransform(Vector2f(-640, -360), Vector2f(0.2, 0.2), 0, "Spacecraft", "player");
	Texture *spacecraftTexture = LoadTexture("textures/spacecraft1");
	DrawComponent *spacecraftDrawer = CreateDraw(spacecraftTexture, BlendAlpha, spacecraftTransform);
	Collider *spacecraftCollider = CreateCollider(8, spacecraftTransform);
	spacecraftCollider->AddToCollisionMask("particle");
	Body *spacecraftBody = CreateBody(10, Vector2f(0, 0), 0, true, spacecraftTransform, spacecraftCollider);
	spacecraftBody->AddToMask("particle");
	Spacecraft *spacecraft = CreateSpacecraft(100, 100, 100, 1600, spacecraftTransform, spacecraftCollider, spacecraftBody);
	PlayerController *playerController = CreatePlayer(1, spacecraft, spacecraftTransform);
	GameObject *ship = CreateGameObject();
	ship->SetTransform(spacecraftTransform);
	ship->SetCollider(spacecraftCollider);
	ship->SetDrawComponent(spacecraftDrawer);
	ship->SetBody(spacecraftBody);
	ship->SetSpacecraft(spacecraft);
	ship->SetPlayerController(playerController);




	Texture *particleTexture = LoadTexture("textures/particle1");
	TransformComponent *particleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.12, 0.12), 0, "Particle", "particle");
	DrawComponent *particleDrawer = CreateDraw(particleTexture, BlendAdd, particleTransform);
	Collider *particleCollider = new Collider(1, particleTransform);
	particleCollider->AddToCollisionMask("particle");
	Body *particleBody = new Body(Vector2f(0, 0), 1, 0, true, particleTransform, particleCollider);
	particleBody->AddToMask("particle");
	ParticleController *particleController = new ParticleController(60, Color(255, 255, 255, 255), Color(255, 100, 0, 0), true, 2, 1.01, 0, particleTransform, particleCollider, particleBody, particleDrawer);

	TransformComponent *particleSystemTransform = CreateTransform(Vector2f(640, 360), Vector2f(1, 1), 0, "ParticleSystem", "particle_system");
	ParticleSystem *particleSystemComponent = CreateParticleSystem(12, 0.2, 0, 0.5, 6, 0, Vector2f(1280, 720), true, particleController, particleSystemTransform);
	FireController *particleSystemController = CreateFireController(particleSystemTransform, spacecraftTransform, particleSystemComponent);
	GameObject *particleSystem = CreateGameObject();
	particleSystem->SetTransform(particleSystemTransform);
	particleSystem->SetParticleSystem(particleSystemComponent);
	particleSystem->SetFireController(particleSystemController);

	
	TransformComponent *engineParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(1, 1), 0, "Fire particle", "particle");
	DrawComponent *engineParticleDrawer = CreateDraw(particleTexture, BlendAdd, engineParticleTransform);
	Collider *engineParticleCollider = new Collider(1, engineParticleTransform);
	engineParticleCollider->AddToCollisionMask("particle");
	engineParticleCollider->AddToCollisionMask("player");
	Body *engineParticleBody = new Body(Vector2f(0, 0), 1, 0, true, engineParticleTransform, engineParticleCollider);
	engineParticleBody->AddToMask("particle");
	engineParticleBody->AddToMask("player");
	ParticleController *engineParticleController = new ParticleController(0.08, Color(0, 100, 255, 255), Color(255, 50, 0, 0), true, 0.02, 1.4, 0, engineParticleTransform, engineParticleCollider, engineParticleBody, engineParticleDrawer);

	TransformComponent *engineParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "FireParticleSystem", "particle_system");
	ParticleSystem *engineParticleSystemComponent = CreateParticleSystem(128, 0.3, 360, 0.5, 20, 0, Vector2f(6, 6), false, engineParticleController, engineParticleSystemTransform);
	//FireController *engineController = CreateFireController(engineParticleSystemTransform, spacecraftTransform);
	EngineParticleController *engineParticleSystemController = CreateEngineParticleController(1, spacecraft, engineParticleSystemTransform, engineParticleSystemComponent);
	GameObject *engineParticleSystem = CreateGameObject();
	engineParticleSystem->SetTransform(engineParticleSystemTransform);
	engineParticleSystem->SetParticleSystem(engineParticleSystemComponent);
	engineParticleSystem->SetEngineParticleController(engineParticleSystemController);


	TransformComponent *engineParticle2Transform = CreateTransform(Vector2f(0, 0), Vector2f(0.15, 0.15), 0, "Fire particle", "particle");
	DrawComponent *engineParticle2Drawer = CreateDraw(particleTexture, BlendAdd, engineParticle2Transform);
	Collider *engineParticle2Collider = new Collider(1, engineParticle2Transform);
	engineParticle2Collider->AddToCollisionMask("particle");
	engineParticle2Collider->AddToCollisionMask("player");
	Body *engineParticle2Body = new Body(Vector2f(0, 0), 1, 0, true, engineParticle2Transform, engineParticle2Collider);
	engineParticle2Body->AddToMask("particle");
	engineParticle2Body->AddToMask("player");
	ParticleController *engineParticle2Controller = new ParticleController(0.12, Color(0, 100, 255, 255), Color(255, 50, 0, 0), true, 0.2, 1.4, 0, engineParticle2Transform, engineParticle2Collider, engineParticle2Body, engineParticle2Drawer);

	TransformComponent *engineParticleSystem2Transform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "FireParticleSystem", "particle_system");
	ParticleSystem *engineParticleSystem2Component = CreateParticleSystem(128, 0.3, 360, 0.5, 30, 0, Vector2f(8, 8), false, engineParticle2Controller, engineParticleSystem2Transform);
	//FireController *engineController = CreateFireController(engineParticleSystemTransform, spacecraftTransform);
	EngineParticleController *engineParticleSystem2Controller = CreateEngineParticleController(1, spacecraft, engineParticleSystem2Transform, engineParticleSystem2Component);
	GameObject *engineParticleSystem2 = CreateGameObject();
	engineParticleSystem2->SetTransform(engineParticleSystem2Transform);
	engineParticleSystem2->SetParticleSystem(engineParticleSystem2Component);
	engineParticleSystem2->SetEngineParticleController(engineParticleSystem2Controller);



	
	Texture *smokeTexture = LoadTexture("textures/particle_dust");
	TransformComponent *smokeParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.02, 0.02), 0, "Fire particle", "particle");
	DrawComponent *smokeParticleDrawer = CreateDraw(smokeTexture, BlendAlpha, smokeParticleTransform);
	Collider *smokeParticleCollider = new Collider(1, smokeParticleTransform);
	smokeParticleCollider->AddToCollisionMask("particle");
	smokeParticleCollider->AddToCollisionMask("player");
	Body *smokeParticleBody = new Body(Vector2f(0, 0), 1, 0, true, smokeParticleTransform, smokeParticleCollider);
	smokeParticleBody->AddToMask("particle");
	smokeParticleBody->AddToMask("player");
	ParticleController *smokeParticleController = new ParticleController(1, Color(55, 155, 255, 155), Color(155, 155, 155, 0), false, 0.02, 2.4, 0, smokeParticleTransform, smokeParticleCollider, smokeParticleBody, smokeParticleDrawer);

	TransformComponent *smokeParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "FireParticleSystem", "particle_system");
	ParticleSystem *smokeParticleSystemComponent = CreateParticleSystem(32, 0.3, 360, 0.5, 20, 0, Vector2f(16, 16), false, smokeParticleController, smokeParticleSystemTransform);
	//FireController *engineController = CreateFireController(engineParticleSystemTransform, spacecraftTransform);
	EngineParticleController *smokeParticleSystemController = CreateEngineParticleController(0.3, spacecraft, smokeParticleSystemTransform, smokeParticleSystemComponent);
	GameObject *smokeParticleSystem = CreateGameObject();
	smokeParticleSystem->SetTransform(smokeParticleSystemTransform);
	smokeParticleSystem->SetParticleSystem(smokeParticleSystemComponent);
	smokeParticleSystem->SetEngineParticleController(smokeParticleSystemController);


	TransformComponent *engineLightTransform = CreateTransform(Vector2f(300, 300), Vector2f(6, 6), 0, "FireLight", "light");
	DrawComponent *engineLightDrawer = CreateDraw(particleTexture, BlendAdd, engineLightTransform);
	engineLightDrawer->SetColor(Color(50, 155, 255, 255));
	FireLight *engineLightController = CreateFireLight(0.3, 6, engineLightTransform, engineLightDrawer, engineParticleSystemComponent);
	GameObject *engineLight = CreateGameObject();
	engineLight->SetTransform(engineLightTransform);
	engineLight->SetDrawComponent(engineLightDrawer);
	engineLight->SetFireLitghtController(engineLightController);


	TransformComponent *bulletParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.01, 0.01), 0, "Bullet particle", "particle");
	DrawComponent *bulletParticleDrawer = CreateDraw(smokeTexture, BlendAdd, bulletParticleTransform);
	Collider *bulletParticleCollider = new Collider(1, bulletParticleTransform);
	bulletParticleCollider->AddToCollisionMask("particle");
	bulletParticleCollider->AddToCollisionMask("bullet");
	Body *bulletParticleBody = new Body(Vector2f(0, 0), 1, 0, true, bulletParticleTransform, bulletParticleCollider);
	bulletParticleBody->AddToMask("particle");
	bulletParticleBody->AddToMask("bullet");
	ParticleController *bulletParticleController = new ParticleController(0.4, Color(255, 150, 50, 255), Color(0, 0, 0, 0), false, 0, 5, 0, bulletParticleTransform, bulletParticleCollider, bulletParticleBody, bulletParticleDrawer);

	TransformComponent *bulletExplosionTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "BulletExplosion", "particle_system");
	ParticleSystem *bulletExplosion = new ParticleSystem(12, 0.4, 160, 0.5, 64, 0, Vector2f(16, 16), true, bulletParticleController, bulletExplosionTransform);


	TransformComponent *explosionLightTransform = new TransformComponent(Vector2f(300, 300), Vector2f(6, 6), 0, "ExplosionLight", "light");
	DrawComponent *explosionLightDrawer = new DrawComponent(particleTexture, BlendAdd, explosionLightTransform);
	explosionLightDrawer->SetColor(Color(255, 155, 55, 255));
	ExplosionLight *explosionLightController = new ExplosionLight(16, explosionLightTransform, explosionLightDrawer);

	Texture *bulletTexture = LoadTexture("textures/bullet");
	TransformComponent *bulletPrefabTransform = new TransformComponent(Vector2f(0, 0), Vector2f(0.4, 0.4), 0, "Bullet", "bullet");
	DrawComponent *bulletPrefabDrawer = new DrawComponent(bulletTexture, BlendAdd, bulletPrefabTransform);
	Collider *bulletPrefabCollider = new Collider(4, bulletPrefabTransform);
	bulletPrefabCollider->AddToCollisionMask("particle");
	bulletPrefabCollider->AddToCollisionMask("player");
	Body *bulletPrefabBody = new Body(Vector2f(0, 0), 10, 0, true, bulletPrefabTransform, bulletPrefabCollider);
	bulletPrefabBody->AddToMask("particle");
	bulletPrefabBody->AddToMask("player");
	bulletPrefabBody->AddToMask("spacecraft");
	Bullet *bulletPrefabController = new Bullet(10, 1, bulletPrefabTransform, bulletPrefabCollider, bulletPrefabBody, bulletPrefabDrawer, bulletExplosion, explosionLightController);

	TransformComponent *weaponParticleTransform = new TransformComponent(Vector2f(0, 0), Vector2f(0.01, 0.01), 0, "Weapon particle", "particle");
	DrawComponent *weaponParticleDrawer = new DrawComponent(smokeTexture, BlendAdd, weaponParticleTransform);
	Collider *weaponParticleCollider = new Collider(1, weaponParticleTransform);
	weaponParticleCollider->AddToCollisionMask("particle");
	weaponParticleCollider->AddToCollisionMask("bullet");
	weaponParticleCollider->AddToCollisionMask("player");
	Body *weaponParticleBody = new Body(Vector2f(0, 0), 1, 0, true, weaponParticleTransform, weaponParticleCollider);
	weaponParticleBody->AddToMask("particle");
	weaponParticleBody->AddToMask("bullet");
	weaponParticleBody->AddToMask("player");
	ParticleController *weaponParticleController = new ParticleController(0.15, Color(255, 150, 50, 255), Color(0, 0, 0, 0), false, 0, 8, 0, weaponParticleTransform, weaponParticleCollider, weaponParticleBody, weaponParticleDrawer);

	TransformComponent *weaponEffectTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "Weapon effect", "particle_system");
	ParticleSystem *weaponEffect = new ParticleSystem(6, 0.4, 160, 0.5, 64, 0, Vector2f(4, 4), true, weaponParticleController, weaponEffectTransform);

	TransformComponent *weaponTransform = CreateTransform(Vector2f(0, 0), Vector2f(1, 1), 0, "Weapon", "weapon");
	Weapon *weaponController = CreateWeapon(0.03, 8, 1000, weaponTransform, spacecraft, bulletPrefabController, weaponEffect);
	GameObject *weapon = CreateGameObject();
	weapon->SetTransform(weaponTransform);
	weapon->SetWeaponController(weaponController);

	TransformComponent *weaponLightTransform = CreateTransform(Vector2f(300, 300), Vector2f(8, 8), 0, "WeaponLight", "light");
	DrawComponent *weaponLightDrawer = CreateDraw(particleTexture, BlendAdd, weaponLightTransform);
	weaponLightDrawer->SetColor(Color(255, 155, 55, 255));
	WeaponLight *weaponLightController = CreateWeaponLight( 16, weaponLightTransform, weaponLightDrawer, weaponTransform);
	GameObject *weaponLight = CreateGameObject();
	weaponLight->SetTransform(weaponLightTransform);
	weaponLight->SetDrawComponent(weaponLightDrawer);
	weaponLight->SetWeaponLightController(weaponLightController);
	weaponController->SetWeaponLightController(weaponLightController);


	Texture *exitButtonTexture = LoadTexture("textures/button_exit");
	TransformComponent *exitButtonTransform = CreateTransform(Vector2f(100, 100), Vector2f(0.3, 0.3), 0, "BackButton", "button");
	DrawComponent *exitButtonDrawer = CreateDraw(exitButtonTexture, BlendAlpha, exitButtonTransform);
	Button *exitButtonController = CreateButton(128, 64, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), exitButtonTransform, exitButtonDrawer, 0, 0);
	GameObject *exitButton = CreateGameObject();
	exitButton->SetTransform(exitButtonTransform);
	exitButton->SetDrawComponent(exitButtonDrawer);
	exitButton->SetButtonController(exitButtonController);


	TransformComponent *asteroidTransform = new TransformComponent(Vector2f(1280, 100), Vector2f(0.2, 0.2), 90, "Asteroid", "asteroid");
	Texture *asteroidTexture = LoadTexture("textures/asteroid1");
	DrawComponent *asteroidDrawer = new DrawComponent(asteroidTexture, BlendAlpha, asteroidTransform);
	Collider *asteroidCollider = new Collider(12, asteroidTransform);
	asteroidCollider->AddToCollisionMask("particle");
	Body *asteroidBody = new Body( Vector2f(-5, 2), 10, -30, true, asteroidTransform, asteroidCollider);
	asteroidBody->AddToMask("particle");
	Asteroid *asteroidController = new Asteroid(10, asteroidCollider, asteroidBody, asteroidTransform, asteroidDrawer);

	TransformComponent *asteroidParticleTransform = new TransformComponent(Vector2f(0, 0), Vector2f(0.015, 0.015), 0, "Asteroid particle", "particle");
	DrawComponent *asteroidParticleDrawer = new DrawComponent(smokeTexture, BlendAlpha, asteroidParticleTransform);
	Collider *asteroidParticleCollider = new Collider(1, asteroidParticleTransform);
	asteroidParticleCollider->AddToCollisionMask("particle");
	asteroidParticleCollider->AddToCollisionMask("asteroid");
	Body *asteroidParticleBody = new Body(Vector2f(0, 0), 1, 0, true, asteroidParticleTransform, asteroidParticleCollider);
	asteroidParticleBody->AddToMask("particle");
	asteroidParticleBody->AddToMask("asteroid");
	ParticleController *asteroidParticleController = new ParticleController(2, Color(255, 255, 255, 100), Color(0, 0, 0, 0), false, 0, 1.4, 0, asteroidParticleTransform, asteroidParticleCollider, asteroidParticleBody, asteroidParticleDrawer);

	TransformComponent *asteroidExplosionTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "AsteroidExplosion", "particle_system");
	ParticleSystem *asteroidExplosionPSComponent = new ParticleSystem(16, 0.4, 160, 0.5, 24, 0, Vector2f(16, 16), true, asteroidParticleController, asteroidExplosionTransform);
	asteroidController->SetExplosion(asteroidExplosionPSComponent);

	TransformComponent *asteroidParticleSystemTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "AsteroidParticleSystem", "particle_system");
	ParticleSystem *asteroidParticleSystemComponent = new ParticleSystem(12, 0.4, 160, 0.5, 8, 0, Vector2f(16, 16), true, asteroidParticleController, asteroidParticleSystemTransform);
	FireController *asteroidParticleSystemController = new FireController(asteroidParticleSystemTransform, asteroidTransform, asteroidParticleSystemComponent);

	TransformComponent *asteroidSpawnerTransform = CreateTransform(Vector2f(0, 0), Vector2f(1, 1), 0, "Asteroid spawner", "spawner");
	AsteroidSpawner *asteroidSpawnerController = CreateAsteroidSpwner(0.05, 1024, 30, 90, spacecraftTransform, asteroidSpawnerTransform, asteroidController, asteroidParticleSystemController);
	GameObject *asteroidSpawner = CreateGameObject();
	asteroidSpawner->SetTransform(asteroidSpawnerTransform);
	asteroidSpawner->SetAsteroidSpawnerController(asteroidSpawnerController);


	///////COMET/////////////
	TransformComponent *cometTransform = new TransformComponent(Vector2f(1280, 100), Vector2f(0.2, 0.2), 90, "comet", "asteroid");
	Texture *cometTexture = LoadTexture("textures/comet");
	DrawComponent *cometDrawer = new DrawComponent(cometTexture, BlendAlpha, cometTransform);
	Collider *cometCollider = new Collider(8, cometTransform);
	cometCollider->AddToCollisionMask("particle");
	Body *cometBody = new Body(Vector2f(-5, 2), 10, -30, true, cometTransform, cometCollider);
	cometBody->AddToMask("particle");
	Asteroid *cometController = new Asteroid(10, cometCollider, cometBody, cometTransform, cometDrawer);

	TransformComponent *cometParticleTransform = new TransformComponent(Vector2f(0, 0), Vector2f(0.015, 0.015), 0, "comet particle", "particle");
	DrawComponent *cometParticleDrawer = new DrawComponent(smokeTexture, BlendAdd, cometParticleTransform);
	Collider *cometParticleCollider = new Collider(1, cometParticleTransform);
	cometParticleCollider->AddToCollisionMask("particle");
	cometParticleCollider->AddToCollisionMask("asteroid");
	Body *cometParticleBody = new Body(Vector2f(0, 0), 1, 0, true, cometParticleTransform, cometParticleCollider);
	cometParticleBody->AddToMask("particle");
	cometParticleBody->AddToMask("asteroid");
	ParticleController *cometParticleController = new ParticleController(3, Color(55, 155, 255, 100), Color(255, 255, 255, 0), false, 0, 1.13, 0, cometParticleTransform, cometParticleCollider, cometParticleBody, cometParticleDrawer);

	TransformComponent *cometExplosionTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "cometExplosion", "particle_system");
	ParticleSystem *cometExplosionPSComponent = new ParticleSystem(32, 0.4, 160, 0.5, 6, 0, Vector2f(4, 4), true, cometParticleController, cometExplosionTransform);
	cometController->SetExplosion(cometExplosionPSComponent);

	TransformComponent *cometParticleSystemTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "cometParticleSystem", "particle_system");
	ParticleSystem *cometParticleSystemComponent = new ParticleSystem(16, 0.4, 160, 0.5, 8, 0, Vector2f(16, 16), true, cometParticleController, cometParticleSystemTransform);
	FireController *cometParticleSystemController = new FireController(cometParticleSystemTransform, cometTransform, cometParticleSystemComponent);

	TransformComponent *cometSpawnerTransform = CreateTransform(Vector2f(0, 0), Vector2f(1, 1), 0, "comet spawner", "spawner");
	AsteroidSpawner *cometSpawnerController = CreateAsteroidSpwner(0.05, 1024, 50, 150, spacecraftTransform, cometSpawnerTransform, cometController, cometParticleSystemController);
	GameObject *cometSpawner = CreateGameObject();
	cometSpawner->SetTransform(cometSpawnerTransform);
	cometSpawner->SetAsteroidSpawnerController(cometSpawnerController);
}
#endif