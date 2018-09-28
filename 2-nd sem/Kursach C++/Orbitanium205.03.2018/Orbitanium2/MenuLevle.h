#ifndef LEVEL0_H
#define LEVEL0_H

#include "ObjectInstancer.h"
#include "ImageLoader.h"

void LoadLevel_0()
{
	TransformComponent *backgroundTransform = CreateTransform(Vector2f(640, 360), Vector2f(0.5, 0.5), 0, "Background", "dekor");
	Texture *spaceTexture = LoadTexture("textures/space");
	DrawComponent *backgroundDrawer = CreateDraw(spaceTexture, BlendAlpha, backgroundTransform);
	BackgroundController *backgroundController = CreateBackgroundController(backgroundTransform);
	GameObject *background = CreateGameObject();
	background->SetTransform(backgroundTransform);
	background->SetDrawComponent(backgroundDrawer);
	background->SetBackgroundController(backgroundController);
	
	TransformComponent *sunTransform = CreateTransform(Vector2f(1360, -120), Vector2f(2, 2), 30, "Sun", "dekor");
	Texture *sunTexture = LoadTexture("textures/sun");
	DrawComponent *sunDrawer = CreateDraw(sunTexture, BlendAdd, sunTransform);
	BackgroundController *sunController = CreateBackgroundController(sunTransform);
	GameObject *sun = CreateGameObject();
	sun->SetTransform(sunTransform);
	sun->SetDrawComponent(sunDrawer);
	sun->SetBackgroundController(sunController);


	TransformComponent *planetTransform = CreateTransform(Vector2f(640, 360), Vector2f(0.2, 0.2), 0, "Earth", "planet");
	Texture *planetTexture = LoadTexture("textures/planet_earth");
	DrawComponent *planetDrawer = CreateDraw(planetTexture, BlendAlpha, planetTransform);
	Collider *planetCollider = CreateCollider(74, planetTransform);
	Body *planetBody = CreateBody(100000, Vector2f(0, 0), 0, false, planetTransform, planetCollider);
	GameObject *planet = CreateGameObject();
	planet->SetTransform(planetTransform);
	planet->SetDrawComponent(planetDrawer);
	planet->SetCollider(planetCollider);
	planet->SetBody(planetBody);


	TransformComponent *moonTransform = CreateTransform(Vector2f(640, 0), Vector2f(0.05, 0.05), 0, "Moon", "satellite");
	Texture *moonTexture = LoadTexture("textures/moon");
	DrawComponent *moonDrawer = CreateDraw(moonTexture, BlendAlpha, moonTransform);
	Collider *moonCollider = CreateCollider(16, moonTransform);
	Body *moonBody = CreateBody(10000, Vector2f(18, 0), 0, true, moonTransform, moonCollider);
	GameObject *moon = CreateGameObject();
	moon->SetTransform(moonTransform);
	moon->SetCollider(moonCollider);
	moon->SetBody(moonBody);
	moon->SetDrawComponent(moonDrawer);



	TransformComponent *spacecraftTransform = CreateTransform(Vector2f(640, 600), Vector2f(0.2, 0.2), 0, "Spacecraft", "player");
	Texture *spacecraftTexture = LoadTexture("textures/spacecraft1");
	DrawComponent *spacecraftDrawer = CreateDraw(spacecraftTexture, BlendAlpha, spacecraftTransform);
	Collider *spacecraftCollider = CreateCollider(8, spacecraftTransform);
	spacecraftCollider->AddToCollisionMask("particle");
	Body *spacecraftBody = CreateBody(10, Vector2f(20, 0), 50, true, spacecraftTransform, spacecraftCollider);
	spacecraftBody->AddToMask("particle");
	Spacecraft *spacecraft = CreateSpacecraft(100, 100, 100, 10, spacecraftTransform, spacecraftCollider, spacecraftBody);
	GameObject *ship = CreateGameObject();
	ship->SetTransform(spacecraftTransform);
	ship->SetCollider(spacecraftCollider);
	ship->SetDrawComponent(spacecraftDrawer);
	ship->SetBody(spacecraftBody);
	ship->SetSpacecraft(spacecraft);


	TransformComponent *teslaTransform = CreateTransform(Vector2f(550, 220), Vector2f(0.15, 0.15), 0, "Tesla", "spacecraft2");
	Texture *teslaTexture = LoadTexture("textures/tesla");
	DrawComponent *teslaDrawer = CreateDraw(teslaTexture, BlendAlpha, teslaTransform);
	Collider *teslaCollider = CreateCollider(8, teslaTransform);
	teslaCollider->AddToCollisionMask("particle");
	Body *teslaBody = CreateBody(10, Vector2f(-80, 150), 30, true, teslaTransform, teslaCollider);
	teslaBody->AddToMask("particle");
	Spacecraft *teslaCraft = CreateSpacecraft(100, 100, 100, 10, teslaTransform, teslaCollider, teslaBody);
	GameObject *tesla = CreateGameObject();
	tesla->SetTransform(teslaTransform);
	tesla->SetCollider(teslaCollider);
	tesla->SetDrawComponent(teslaDrawer);
	tesla->SetBody(teslaBody);
	tesla->SetSpacecraft(teslaCraft);


	TransformComponent *sateliteTransform = CreateTransform(Vector2f(640, 35), Vector2f(0.09, 0.09), 0, "Satelite", "spacecraft2");
	Texture *sateliteTexture = LoadTexture("textures/satelite1");
	DrawComponent *sateliteDrawer = CreateDraw(sateliteTexture, BlendAlpha, sateliteTransform);
	Collider *sateliteCollider = CreateCollider(8, sateliteTransform);
	sateliteCollider->AddToCollisionMask("particle");
	Body *sateliteBody = CreateBody(10, Vector2f(0, -3), 32, true, sateliteTransform, sateliteCollider);
	sateliteBody->AddToMask("particle");
	Spacecraft *sateliteCraft = CreateSpacecraft(100, 100, 100, 10, sateliteTransform, sateliteCollider, sateliteBody);
	GameObject *satelite = CreateGameObject();
	satelite->SetTransform(sateliteTransform);
	satelite->SetCollider(sateliteCollider);
	satelite->SetDrawComponent(sateliteDrawer);
	satelite->SetBody(sateliteBody);
	satelite->SetSpacecraft(sateliteCraft);


	TransformComponent *satelite2Transform = CreateTransform(Vector2f(540, 360), Vector2f(0.07, 0.07), 90, "Satelite", "spacecraft2");
	Texture *satelite2Texture = LoadTexture("textures/satelite3");
	DrawComponent *satelite2Drawer = CreateDraw(satelite2Texture, BlendAlpha, satelite2Transform);
	Collider *satelite2Collider = CreateCollider(6, satelite2Transform);
	satelite2Collider->AddToCollisionMask("particle");
	Body *satelite2Body = CreateBody(10, Vector2f(0, 31), -19, true, satelite2Transform, satelite2Collider);
	satelite2Body->AddToMask("particle");
	Spacecraft *satelite2Craft = CreateSpacecraft(100, 100, 100, 10, satelite2Transform, satelite2Collider, satelite2Body);
	GameObject *satelite2 = CreateGameObject();
	satelite2->SetTransform(satelite2Transform);
	satelite2->SetCollider(satelite2Collider);
	satelite2->SetDrawComponent(satelite2Drawer);
	satelite2->SetBody(satelite2Body);
	satelite2->SetSpacecraft(satelite2Craft);




	Texture *playButtonTexture = LoadTexture("textures/button_play");
	TransformComponent *playButtonTransform = CreateTransform(Vector2f(640, 600), Vector2f(0.7, 0.7), 0, "PlayButton", "button");
	DrawComponent *playButtonDrawer = CreateDraw(playButtonTexture, BlendAlpha, playButtonTransform);
	Button *playButtonController = CreateButton(128, 128, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), playButtonTransform, playButtonDrawer, 0, 2);
	GameObject *playButton = CreateGameObject();
	playButton->SetTransform(playButtonTransform);
	playButton->SetDrawComponent(playButtonDrawer);
	playButton->SetButtonController(playButtonController);

	Texture *exitButtonTexture = LoadTexture("textures/button_exit");
	TransformComponent *exitButtonTransform = CreateTransform(Vector2f(335, 600), Vector2f(0.3, 0.3), 0, "ExitButton", "button");
	DrawComponent *exitButtonDrawer = CreateDraw(exitButtonTexture, BlendAlpha, exitButtonTransform);
	Button *exitButtonController = CreateButton(128, 64, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), exitButtonTransform, exitButtonDrawer, 0, 0);
	GameObject *exitButton = CreateGameObject();
	exitButton->SetTransform(exitButtonTransform);
	exitButton->SetDrawComponent(exitButtonDrawer);
	exitButton->SetButtonController(exitButtonController);

	Texture *recordsButtonTexture = LoadTexture("textures/button_records");
	TransformComponent *recordsButtonTransform = CreateTransform(Vector2f(490, 600), Vector2f(0.3, 0.3), 0, "RecordsButton", "button");
	DrawComponent *recordsButtonDrawer = CreateDraw(recordsButtonTexture, BlendAlpha, recordsButtonTransform);
	Button *recordsButtonController = CreateButton(128, 64, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), recordsButtonTransform, recordsButtonDrawer, 0, 1);
	GameObject *recordsButton = CreateGameObject();
	recordsButton->SetTransform(recordsButtonTransform);
	recordsButton->SetDrawComponent(recordsButtonDrawer);
	recordsButton->SetButtonController(recordsButtonController);

	Texture *recordsWindowTexture = LoadTexture("textures/scores_window");
	TransformComponent *recordsWindowTransform = CreateTransform(Vector2f(640, 260), Vector2f(1, 1), 0, "RecordsWindow", "window");
	DrawComponent *recordsWindowDrawer = CreateDraw(recordsWindowTexture, BlendAlpha, recordsWindowTransform);
	TextWindow *recordsWindowController = CreateTextWindow(recordsWindowTransform, recordsWindowDrawer, 1);
	GameObject *recordsWindow = CreateGameObject();
	recordsWindow->SetTransform(recordsWindowTransform);
	recordsWindow->SetDrawComponent(recordsWindowDrawer);
	recordsWindow->SetTextWindowController(recordsWindowController);

	Texture *authorButtonTexture = LoadTexture("textures/button_author");
	TransformComponent *authorButtonTransform = CreateTransform(Vector2f(790, 600), Vector2f(0.3, 0.3), 0, "AuthorButton", "button");
	DrawComponent *authorButtonDrawer = CreateDraw(authorButtonTexture, BlendAlpha, authorButtonTransform);
	Button *authorButtonController = CreateButton(128, 64, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), authorButtonTransform, authorButtonDrawer, 0, 3);
	GameObject *authorButton = CreateGameObject();
	authorButton->SetTransform(authorButtonTransform);
	authorButton->SetDrawComponent(authorButtonDrawer);
	authorButton->SetButtonController(authorButtonController);

	Texture *authorWindowTexture = LoadTexture("textures/author_window");
	TransformComponent *authorWindowTransform = CreateTransform(Vector2f(640, 260), Vector2f(1, 1), 0, "AuthorWindow", "window");
	DrawComponent *authorWindowDrawer = CreateDraw(authorWindowTexture, BlendAlpha, authorWindowTransform);
	TextWindow *authorWindowController = CreateTextWindow(authorWindowTransform, authorWindowDrawer, 3);
	GameObject *authorWindow = CreateGameObject();
	authorWindow->SetTransform(authorWindowTransform);
	authorWindow->SetDrawComponent(authorWindowDrawer);
	authorWindow->SetTextWindowController(authorWindowController);

	Texture *rulesButtonTexture = LoadTexture("textures/button_rules");
	TransformComponent *rulesButtonTransform = CreateTransform(Vector2f(945, 600), Vector2f(0.3, 0.3), 0, "RulesButton", "button");
	DrawComponent *rulesButtonDrawer = CreateDraw(rulesButtonTexture, BlendAlpha, rulesButtonTransform);
	Button *rulesButtonController = CreateButton(128, 64, Color(150, 150, 150, 150), Color(200, 200, 200, 200), Color(255, 255, 255, 255), rulesButtonTransform, rulesButtonDrawer, 0, 4);
	GameObject *rulesButton = CreateGameObject();
	rulesButton->SetTransform(rulesButtonTransform);
	rulesButton->SetDrawComponent(rulesButtonDrawer);
	rulesButton->SetButtonController(rulesButtonController);

	Texture *rulesWindowTexture = LoadTexture("textures/rules_window");
	TransformComponent *rulesWindowTransform = CreateTransform(Vector2f(640, 260), Vector2f(1, 1), 0, "RulesWindow", "window");
	DrawComponent *rulesWindowDrawer = CreateDraw(rulesWindowTexture, BlendAlpha, rulesWindowTransform);
	TextWindow *rulesWindowController = CreateTextWindow(rulesWindowTransform, rulesWindowDrawer, 4);
	GameObject *rulesWindow = CreateGameObject();
	rulesWindow->SetTransform(rulesWindowTransform);
	rulesWindow->SetDrawComponent(rulesWindowDrawer);
	rulesWindow->SetTextWindowController(rulesWindowController);


	Texture *looseWindowTexture = LoadTexture("textures/loose_window");
	TransformComponent *looseWindowTransform = CreateTransform(Vector2f(640, 260), Vector2f(1, 1), 0, "Loose Window", "window");
	DrawComponent *looseWindowDrawer = CreateDraw(looseWindowTexture, BlendAlpha, looseWindowTransform);
	TextWindow *looseWindowController = CreateTextWindow(looseWindowTransform, looseWindowDrawer, 5);
	GameObject *looseWindow = CreateGameObject();
	looseWindow->SetTransform(looseWindowTransform);
	looseWindow->SetDrawComponent(looseWindowDrawer);
	looseWindow->SetTextWindowController(looseWindowController);


	Texture *particleTexture = LoadTexture("textures/particle1");
	TransformComponent *particleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.12, 0.12), 0, "Particle", "particle");
	DrawComponent *particleDrawer = CreateDraw(particleTexture, BlendAdd, particleTransform);
	Collider *particleCollider = new Collider(1, particleTransform);
	particleCollider->AddToCollisionMask("particle");
	Body *particleBody = new Body(Vector2f(0, 0), 1, 0, true, particleTransform, particleCollider);
	particleBody->AddToMask("particle");
	ParticleController *particleController = new ParticleController(60, Color(255, 255, 255, 255), Color(255, 100, 0, 0), true, 8, 1.01, 0, particleTransform, particleCollider, particleBody, particleDrawer);

	TransformComponent *particleSystemTransform = CreateTransform(Vector2f(640, 360), Vector2f(1, 1), 0, "ParticleSystem", "particle_system");
	ParticleSystem *particleSystemComponent = CreateParticleSystem(10, 0.2, 0, 0.5, 6, 0, Vector2f(1280, 720),true,  particleController, particleSystemTransform);
	FireController *particleSystemController = CreateFireController(particleSystemTransform, spacecraftTransform, particleSystemComponent);
	GameObject *particleSystem = CreateGameObject();
	particleSystem->SetTransform(particleSystemTransform);
	particleSystem->SetParticleSystem(particleSystemComponent);
	particleSystem->SetFireController(particleSystemController);




	Texture *fireParticleTexture = LoadTexture("textures/particle_fire2");
	TransformComponent *fireParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.05, 0.05), 0, "Fire particle", "particle");
	DrawComponent *fireParticleDrawer = CreateDraw(fireParticleTexture, BlendAdd, fireParticleTransform);
	Collider *fireParticleCollider = new Collider(1, fireParticleTransform);
	fireParticleCollider->AddToCollisionMask("particle");
	fireParticleCollider->AddToCollisionMask("player");
	Body *fireParticleBody = new Body(Vector2f(0, 0), 1, 0, true, fireParticleTransform, fireParticleCollider);
	fireParticleBody->AddToMask("particle");
	fireParticleBody->AddToMask("player");
	ParticleController *fireParticleController = new ParticleController(1, Color(255, 255, 255, 255), Color(255, 200, 150, 0), false, 0, 1.4, 0, fireParticleTransform, fireParticleCollider, fireParticleBody, fireParticleDrawer);

	TransformComponent *fireParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "FireParticleSystem", "particle_system");
	ParticleSystem *fireParticleSystemComponent = CreateParticleSystem(6, 0.3, 360, 0.5, 10, 0, Vector2f(8, 8),true, fireParticleController, fireParticleSystemTransform);
	FireController *fireController = CreateFireController(fireParticleSystemTransform, spacecraftTransform, fireParticleSystemComponent );
	GameObject *fireParticleSystem = CreateGameObject();
	fireParticleSystem->SetTransform(fireParticleSystemTransform);
	fireParticleSystem->SetParticleSystem(fireParticleSystemComponent);
	fireParticleSystem->SetFireController(fireController);

	TransformComponent *fireLightTransform = CreateTransform(Vector2f(300, 300), Vector2f(8, 8), 0, "FireLight", "light");
	DrawComponent *fireLightDrawer = CreateDraw(particleTexture, BlendAdd, fireLightTransform);
	fireLightDrawer->SetColor(Color(255, 155, 100, 200));
	FireLight *fireLightController = CreateFireLight(0.1, 2, fireLightTransform, fireLightDrawer, fireParticleSystemComponent);
	GameObject *fireLight = CreateGameObject();
	fireLight->SetTransform(fireLightTransform);
	fireLight->SetDrawComponent(fireLightDrawer);
	fireLight->SetFireLitghtController(fireLightController);



	Texture *smokeParticleTexture = LoadTexture("textures/particle_dust");
	TransformComponent *smokeParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.015, 0.015), 0, "Smoke particle", "particle");
	DrawComponent *smokeParticleDrawer = CreateDraw(smokeParticleTexture, BlendAlpha, smokeParticleTransform);
	Collider *smokeParticleCollider = new Collider(1, smokeParticleTransform);
	smokeParticleCollider->AddToCollisionMask("particle");
	smokeParticleCollider->AddToCollisionMask("player");
	Body *smokeParticleBody = new Body(Vector2f(0, 0), 1, 0, true, smokeParticleTransform, smokeParticleCollider);
	smokeParticleBody->AddToMask("particle");
	smokeParticleBody->AddToMask("player");
	ParticleController *smokeParticleController = new ParticleController(4, Color(250, 200, 150, 255), Color(0, 0, 0, 0), false, 0, 1.4, 0, smokeParticleTransform, smokeParticleCollider, smokeParticleBody, smokeParticleDrawer);

	TransformComponent *smokeParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "SmokeParticleSystem", "particle_system");
	ParticleSystem *smokeParticleSystemComponent = CreateParticleSystem(8, 0.4, 160, 0.5, 8, 0, Vector2f(16, 16), true, smokeParticleController, smokeParticleSystemTransform);
	FireController *smokeController = CreateFireController(smokeParticleSystemTransform, spacecraftTransform, smokeParticleSystemComponent );
	GameObject *smokeParticleSystem = CreateGameObject();
	smokeParticleSystem->SetTransform(smokeParticleSystemTransform);
	smokeParticleSystem->SetParticleSystem(smokeParticleSystemComponent);
	smokeParticleSystem->SetFireController(smokeController);



	TransformComponent *asteroidTransform = CreateTransform(Vector2f(1280, 100), Vector2f(0.2, 0.2), 90, "Asteroid", "asteroid");
	Texture *asteroidTexture = LoadTexture("textures/asteroid1");
	DrawComponent *asteroidDrawer = CreateDraw(asteroidTexture, BlendAlpha, asteroidTransform);
	Collider *asteroidCollider = CreateCollider(12, asteroidTransform);
	asteroidCollider->AddToCollisionMask("particle");
	Body *asteroidBody = CreateBody(10, Vector2f(-5, 2), -30, true, asteroidTransform, asteroidCollider);
	asteroidBody->AddToMask("particle");
	Asteroid *asteroidController = CreateAsteroidController(10, asteroidCollider, asteroidBody, asteroidTransform, asteroidDrawer);
	GameObject *asteroid = CreateGameObject();
	asteroid->SetTransform(asteroidTransform);
	asteroid->SetCollider(asteroidCollider);
	asteroid->SetDrawComponent(asteroidDrawer);
	asteroid->SetBody(asteroidBody);
	asteroid->SetAteroidController(asteroidController);


	TransformComponent *asteroidParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.015, 0.015), 0, "Asteroid particle", "particle");
	DrawComponent *asteroidParticleDrawer = CreateDraw(smokeParticleTexture, BlendAlpha, asteroidParticleTransform);
	Collider *asteroidParticleCollider = new Collider(1, asteroidParticleTransform);
	asteroidParticleCollider->AddToCollisionMask("particle");
	asteroidParticleCollider->AddToCollisionMask("asteroid");
	Body *asteroidParticleBody = new Body(Vector2f(0, 0), 1, 0, true, asteroidParticleTransform, asteroidParticleCollider);
	asteroidParticleBody->AddToMask("particle");
	asteroidParticleBody->AddToMask("asteroid");
	ParticleController *asteroidParticleController = new ParticleController(4, Color(255, 255, 255, 100), Color(0, 0, 0, 0), false, 0, 1.4, 0, asteroidParticleTransform, asteroidParticleCollider, asteroidParticleBody, asteroidParticleDrawer);
	

	TransformComponent *asteroidExplosionTransform = new TransformComponent(Vector2f(300, 300), Vector2f(1, 1), 0, "AsteroidExplosion", "particle_system");
	ParticleSystem *asteroidExplosionPSComponent = new ParticleSystem(32, 0.4, 160, 0.5, 24, 0, Vector2f(16, 16), true, asteroidParticleController, asteroidExplosionTransform);
	asteroidController->SetExplosion(asteroidExplosionPSComponent);


	TransformComponent *asteroidParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "AsteroidParticleSystem", "particle_system");
	ParticleSystem *asteroidParticleSystemComponent = CreateParticleSystem(12, 0.4, 160, 0.5, 8, 0, Vector2f(16, 16), true, asteroidParticleController, asteroidParticleSystemTransform);
	FireController *asteroidParticleSystemController = CreateFireController(asteroidParticleSystemTransform, asteroidTransform, asteroidParticleSystemComponent);
	GameObject *asteroidParticleSystem = CreateGameObject();
	asteroidParticleSystem->SetTransform(asteroidParticleSystemTransform);
	asteroidParticleSystem->SetParticleSystem(asteroidParticleSystemComponent);
	asteroidParticleSystem->SetFireController(asteroidParticleSystemController);




	TransformComponent *cometTransform = CreateTransform(Vector2f(0, 720), Vector2f(0.2, 0.2), 90, "Comet", "asteroid");
	Texture *cometTexture = LoadTexture("textures/comet");
	DrawComponent *cometDrawer = CreateDraw(cometTexture, BlendAlpha, cometTransform);
	Collider *cometCollider = CreateCollider(12, cometTransform);
	cometCollider->AddToCollisionMask("particle");
	Body *cometBody = CreateBody(10, Vector2f(15, -2), -30, true, cometTransform, cometCollider);
	cometBody->AddToMask("particle");
	Asteroid *cometController = CreateAsteroidController(10, cometCollider, cometBody, cometTransform, cometDrawer);
	GameObject *comet = CreateGameObject();
	comet->SetTransform(cometTransform);
	comet->SetCollider(cometCollider);
	comet->SetDrawComponent(cometDrawer);
	comet->SetBody(cometBody);
	comet->SetAteroidController(cometController);

	TransformComponent *cometParticleTransform = CreateTransform(Vector2f(0, 0), Vector2f(0.015, 0.015), 0, "Comet particle", "particle");
	DrawComponent *cometParticleDrawer = CreateDraw(smokeParticleTexture, BlendAdd, cometParticleTransform);
	Collider *cometParticleCollider = new Collider(1, cometParticleTransform);
	cometParticleCollider->AddToCollisionMask("particle");
	cometParticleCollider->AddToCollisionMask("asteroid");
	Body *cometParticleBody = new Body(Vector2f(0, 0), 1, 0, true, cometParticleTransform, cometParticleCollider);
	cometParticleBody->AddToMask("particle");
	cometParticleBody->AddToMask("asteroid");
	ParticleController *cometParticleController = new ParticleController(10, Color(55, 155, 255, 100), Color(255, 255, 255, 0), false, 0, 1.1, 0, cometParticleTransform, cometParticleCollider, cometParticleBody, cometParticleDrawer);


	TransformComponent *cometExplosionTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "CometExplosion", "particle_system");
	ParticleSystem *cometExplosionPSComponent = new ParticleSystem(32, 0.4, 160, 0.5, 24, 0, Vector2f(8, 8), true, cometParticleController, cometExplosionTransform);
	cometController->SetExplosion(cometExplosionPSComponent);

	TransformComponent *cometParticleSystemTransform = CreateTransform(Vector2f(300, 300), Vector2f(1, 1), 0, "CometParticleSystem", "particle_system");
	ParticleSystem *cometParticleSystemComponent = CreateParticleSystem(12, 0.4, 160, 0.5,  2, 0, Vector2f(8, 8), true, cometParticleController, cometParticleSystemTransform);
	FireController *cometParticleSystemController = CreateFireController(cometParticleSystemTransform, cometTransform, asteroidParticleSystemComponent);
	GameObject *cometParticleSystem = CreateGameObject();
	cometParticleSystem->SetTransform(cometParticleSystemTransform);
	cometParticleSystem->SetParticleSystem(cometParticleSystemComponent);
	cometParticleSystem->SetFireController(cometParticleSystemController);

	TransformComponent *cometLightTransform = CreateTransform(Vector2f(300, 300), Vector2f(5, 5), 0, "CometLight", "light");
	DrawComponent *cometLightDrawer = CreateDraw(particleTexture, BlendAdd, cometLightTransform);
	cometLightDrawer->SetColor(Color(100, 155, 255, 150));
	FireLight *cometLightController = CreateFireLight(0.08, 2, cometLightTransform, cometLightDrawer, cometParticleSystemComponent);
	GameObject *cometLight = CreateGameObject();
	cometLight->SetTransform(cometLightTransform);
	cometLight->SetDrawComponent(cometLightDrawer);
	cometLight->SetFireLitghtController(cometLightController);
}
#endif