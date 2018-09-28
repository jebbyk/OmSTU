#pragma warning(disable : 4996)

#include <stdio.h>
#include <iostream>
#include "MenuLevle.h"
#include "level1.h"
#include "view.h"
#include "sstream"
#include <fstream>
#include <iostream>
#include <string>

void MemClean()
{
	for (transformsIterator = transforms.begin(); transformsIterator != transforms.end();)
	{
		if ((*transformsIterator)->GetDestroy() == true)
		{
			delete(*transformsIterator);
			transformsIterator = transforms.erase(transformsIterator);
		}
		else { ++transformsIterator; }
	}
	for (collidersIterator = colliders.begin(); collidersIterator != colliders.end();)
	{
		if ((*collidersIterator)->GetDestroy() == true)
		{
			delete(*collidersIterator);
			collidersIterator = colliders.erase(collidersIterator);
		}
		else { ++collidersIterator; }
	}

	for (bodiesIterator = bodies.begin(); bodiesIterator != bodies.end();)
	{
		if ((*bodiesIterator)->GetDestroy() == true)
		{
			delete(*bodiesIterator);
			bodiesIterator = bodies.erase(bodiesIterator);
		}
		else { ++bodiesIterator; }
	}
	for (drawingsIterator = drawings.begin(); drawingsIterator != drawings.end();)
	{
		if ((*drawingsIterator)->GetDestroy() == true)
		{
			delete(*drawingsIterator);
			drawingsIterator = drawings.erase(drawingsIterator);
		}
		else { ++drawingsIterator; }
	}
	for (spacecraftsIterator = spacecrafts.begin(); spacecraftsIterator != spacecrafts.end();)
	{
		if ((*spacecraftsIterator)->GetDestroy() == true)
		{
			delete(*spacecraftsIterator);
			spacecraftsIterator = spacecrafts.erase(spacecraftsIterator);
		}
		else { ++spacecraftsIterator; }
	}
	for (particlesIterator = particles.begin(); particlesIterator != particles.end();)
	{
		if ((*particlesIterator)->GetDestroy() == true)
		{
			delete(*particlesIterator);
			particlesIterator = particles.erase(particlesIterator);
		}
		else { ++particlesIterator; }
	}
	for (particleSystemsIt = particleSystems.begin(); particleSystemsIt != particleSystems.end(); )
	{
		if ((*particleSystemsIt)->GetDestroy() == true)
		{
			delete(*particleSystemsIt);
			particleSystemsIt = particleSystems.erase(particleSystemsIt);
		}
		else { ++particleSystemsIt; }
	}
	for (fireControllersIt = fireControllers.begin(); fireControllersIt != fireControllers.end(); )
	{
		if ((*fireControllersIt)->GetDestroy() == true)
		{
			delete(*fireControllersIt);
			fireControllersIt = fireControllers.erase(fireControllersIt);
		}
		else { ++fireControllersIt; }
	}
	for (asteroidControllersIt = asteroidControllers.begin(); asteroidControllersIt != asteroidControllers.end(); )
	{
		if ((*asteroidControllersIt)->GetDestroy() == true)
		{
			delete(*asteroidControllersIt);
			asteroidControllersIt = asteroidControllers.erase(asteroidControllersIt);
		}
		else { ++asteroidControllersIt; }
	}
	for (explosionsIterator = explosions.begin(); explosionsIterator != explosions.end();)
	{
		if ((*explosionsIterator)->GetDestroy() == true)
		{
			delete(*explosionsIterator);
			explosionsIterator = explosions.erase(explosionsIterator);
		}
		else { ++explosionsIterator; }
	}
	for (backgroundsIt = backgroundControllers.begin(); backgroundsIt != backgroundControllers.end();)
	{
		if ((*backgroundsIt)->GetDestroy() == true)
		{
			delete(*backgroundsIt);
			backgroundsIt = backgroundControllers.erase(backgroundsIt);
		}
		else { ++backgroundsIt; }
	}
	for (buttonsIterator = buttons.begin(); buttonsIterator != buttons.end();)
	{
		if ((*buttonsIterator)->GetDestroy() == true)
		{
			delete(*buttonsIterator);
			buttonsIterator = buttons.erase(buttonsIterator);
		}
		else { ++buttonsIterator; }
	}
	for (windowsIterator = textWindows.begin(); windowsIterator != textWindows.end();)
	{
		if ((*windowsIterator)->GetDestroy() == true)
		{
			delete(*windowsIterator);
			windowsIterator = textWindows.erase(windowsIterator);
		}
		else { ++windowsIterator; }
	}
	for (objectsIterator = objects.begin(); objectsIterator != objects.end();)
	{
		if ((*objectsIterator)->GetDestroy() == true)
		{
			delete(*objectsIterator);
			objectsIterator = objects.erase(objectsIterator);
		}
		else { ++objectsIterator; }
	}
	for (fireLightsIterator = fireLights.begin(); fireLightsIterator != fireLights.end();)
	{
		if ((*fireLightsIterator)->GetDestroy() == true)
		{
			delete(*fireLightsIterator);
			fireLightsIterator = fireLights.erase(fireLightsIterator);
		}
		else { ++fireLightsIterator; }
	}
	for (playersIterator = players.begin(); playersIterator != players.end();)
	{
		if ((*playersIterator)->GetDestroy() == true)
		{
			delete(*playersIterator);
			playersIterator = players.erase(playersIterator);
		}
		else { ++playersIterator; }
	}
	for (engineParticleControllersIt = engineParticleControllers.begin(); engineParticleControllersIt != engineParticleControllers.end(); )
	{
		if ((*engineParticleControllersIt)->GetDestroy() == true) 
		{
			delete(*engineParticleControllersIt);
			engineParticleControllersIt = engineParticleControllers.erase(engineParticleControllersIt);
		}
		else { ++engineParticleControllersIt; }
	}
	for (bulletsIterator = bullets.begin(); bulletsIterator != bullets.end();)
	{
		if ((*bulletsIterator)->GetDestroy() == true)
		{
			delete(*bulletsIterator);
			bulletsIterator = bullets.erase(bulletsIterator);
		}
		else { ++bulletsIterator; }
	}
	for (weaponsIterator = weapons.begin(); weaponsIterator != weapons.end();)
	{
		if ((*weaponsIterator)->GetDestroy() == true)
		{
			delete(*weaponsIterator);
			weaponsIterator = weapons.erase(weaponsIterator);
		}
		else { ++weaponsIterator; }
	}
	for (weaponLightsIterator = weaponLights.begin(); weaponLightsIterator != weaponLights.end();)
	{
		if ((*weaponLightsIterator)->GetDestroy() == true)
		{
			delete(*weaponLightsIterator);
			weaponLightsIterator = weaponLights.erase(weaponLightsIterator);
		}
		else { ++weaponLightsIterator; }
	}
	for (explosionLightsIterator = explosionLights.begin(); explosionLightsIterator != explosionLights.end();)
	{
		if ((*explosionLightsIterator)->GetDestroy() == true)
		{
			delete(*explosionLightsIterator);
			explosionLightsIterator = explosionLights.erase(explosionLightsIterator);
		}
		else { ++explosionLightsIterator; }
	}
	for (asteroidSpawnersIt = asteroidSpawners.begin(); asteroidSpawnersIt != asteroidSpawners.end();)
	{
		if ((*asteroidSpawnersIt)->GetDestroy() == true)
		{
			delete(*asteroidSpawnersIt);
			asteroidSpawnersIt = asteroidSpawners.erase(asteroidSpawnersIt);
		}
		else { ++asteroidSpawnersIt; }
	}
}
void UnloadLevel()
{
	for (texturesIterator = textures.begin(); texturesIterator != textures.end();)
	{
		delete(*texturesIterator);
		texturesIterator = textures.erase(texturesIterator);
	}
	for (transformsIterator = transforms.begin(); transformsIterator != transforms.end();)
	{
			delete(*transformsIterator);
			transformsIterator = transforms.erase(transformsIterator);
	}
	for (collidersIterator = colliders.begin(); collidersIterator != colliders.end();)
	{
			delete(*collidersIterator);
			collidersIterator = colliders.erase(collidersIterator);
	}

	for (bodiesIterator = bodies.begin(); bodiesIterator != bodies.end();)
	{
			delete(*bodiesIterator);
			bodiesIterator = bodies.erase(bodiesIterator);
	}
	for (drawingsIterator = drawings.begin(); drawingsIterator != drawings.end();)
	{
			delete(*drawingsIterator);
			drawingsIterator = drawings.erase(drawingsIterator);
	}
	for (spacecraftsIterator = spacecrafts.begin(); spacecraftsIterator != spacecrafts.end();)
	{
			delete(*spacecraftsIterator);
			spacecraftsIterator = spacecrafts.erase(spacecraftsIterator);
	}
	for (particlesIterator = particles.begin(); particlesIterator != particles.end();)
	{
			delete(*particlesIterator);
			particlesIterator = particles.erase(particlesIterator);
	}
	for (particleSystemsIt = particleSystems.begin(); particleSystemsIt != particleSystems.end(); )
	{
			delete(*particleSystemsIt);
			particleSystemsIt = particleSystems.erase(particleSystemsIt);
	}
	for (fireControllersIt = fireControllers.begin(); fireControllersIt != fireControllers.end(); )
	{
			delete(*fireControllersIt);
			fireControllersIt = fireControllers.erase(fireControllersIt);
	}
	for (asteroidControllersIt = asteroidControllers.begin(); asteroidControllersIt != asteroidControllers.end(); )
	{
			delete(*asteroidControllersIt);
			asteroidControllersIt = asteroidControllers.erase(asteroidControllersIt);
	}
	for (explosionsIterator = explosions.begin(); explosionsIterator != explosions.end();)
	{
			delete(*explosionsIterator);
			explosionsIterator = explosions.erase(explosionsIterator);
	}
	for (backgroundsIt = backgroundControllers.begin(); backgroundsIt != backgroundControllers.end();)
	{
			delete(*backgroundsIt);
			backgroundsIt = backgroundControllers.erase(backgroundsIt);
	}
	for (buttonsIterator = buttons.begin(); buttonsIterator != buttons.end();)
	{
			delete(*buttonsIterator);
			buttonsIterator = buttons.erase(buttonsIterator);
	}
	for (windowsIterator = textWindows.begin(); windowsIterator != textWindows.end();)
	{
			delete(*windowsIterator);
			windowsIterator = textWindows.erase(windowsIterator);
	}
	for (objectsIterator = objects.begin(); objectsIterator != objects.end();)
	{
			delete(*objectsIterator);
			objectsIterator = objects.erase(objectsIterator);
	}
	for (fireLightsIterator = fireLights.begin(); fireLightsIterator != fireLights.end();)
	{
			delete(*fireLightsIterator);
			fireLightsIterator = fireLights.erase(fireLightsIterator);
	}
	for (playersIterator = players.begin(); playersIterator != players.end();)
	{
			delete(*playersIterator);
			playersIterator = players.erase(playersIterator);
	}
	for (engineParticleControllersIt = engineParticleControllers.begin(); engineParticleControllersIt != engineParticleControllers.end(); )
	{
			delete(*engineParticleControllersIt);
			engineParticleControllersIt = engineParticleControllers.erase(engineParticleControllersIt);
	}
	for (bulletsIterator = bullets.begin(); bulletsIterator != bullets.end();)
	{
			delete(*bulletsIterator);
			bulletsIterator = bullets.erase(bulletsIterator);
	}
	for (weaponsIterator = weapons.begin(); weaponsIterator != weapons.end();)
	{
			delete(*weaponsIterator);
			weaponsIterator = weapons.erase(weaponsIterator);
	}
	for (weaponLightsIterator = weaponLights.begin(); weaponLightsIterator != weaponLights.end();)
	{
			delete(*weaponLightsIterator);
			weaponLightsIterator = weaponLights.erase(weaponLightsIterator);
	}
	for (explosionLightsIterator = explosionLights.begin(); explosionLightsIterator != explosionLights.end();)
	{
		delete(*explosionLightsIterator);
		explosionLightsIterator = explosionLights.erase(explosionLightsIterator);
	}
	for (asteroidSpawnersIt = asteroidSpawners.begin(); asteroidSpawnersIt != asteroidSpawners.end();)
	{
			delete(*asteroidSpawnersIt);
			asteroidSpawnersIt = asteroidSpawners.erase(asteroidSpawnersIt);
	}
}

int main()
{
	Texture *cursorTexture = LoadTexture("textures/cursor");
	Sprite cursorSprite;
	cursorSprite.setTexture(*cursorTexture);
	cursorSprite.setScale(0.15, 0.15);

	int count = 0;
	float showStatTimer = 0;
	float neededFPS = 67;
	float frameUpdateTimer = 0;
	Clock deltaClock;
	Clock frameClock;
	int frame = 0;
	RenderWindow window(VideoMode(1280, 720), "Orbitanium");
	window.setMouseCursorVisible(false);
	LoadLevel_0();
	view.reset(FloatRect(0, 0, 1280, 720));

	Font font;
	font.loadFromFile("fonts/rus_eng.ttf");
	Text text("", font, 20);
	text.setColor(Color::White);
	text.setStyle(Text::Bold | Text::Underlined);

	Text nameText("", font, 48);
	text.setColor(Color::White);
	text.setStyle(Text::Bold);
	
	while (window.isOpen())
	{
		frameUpdateTimer += frameClock.getElapsedTime().asSeconds();
		frameClock.restart();
	
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == sf::Event::TextEntered && curentSelectedMenu == 5)
			{
				if (event.text.unicode < 128)
				{
					playerName += static_cast<char>(event.text.unicode);
					nameText.setString(playerName);
				}

			}
		}

		deltaTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		if (loadNext == true)
		{
			curentSelectedMenu = -1;
			UnloadLevel();
			LoadLevel_1();
			loadNext = false;
			levelNumber = 1;
		}
		if (loadMain)
		{
			UnloadLevel();
			LoadLevel_0();
			loadMain = false;
			levelNumber = 0;
		}

		showStatTimer += deltaTime;
		frame++;
		frameUpdateTimer = 0;

		mousePosition.x = Mouse::getPosition(window).x;
		mousePosition.y = Mouse::getPosition(window).y;
		cursorSprite.setPosition(mousePosition + view.getCenter() - view.getSize()/2.0f);
			
		window.clear();

		if (TryFindWithTag("player") == true)
		{
			SetViewTargetPoint(FindGameObjectWithTag("player")->GetTransformComponent()->GetPosition());
		}
		UpdateView();
		window.setView(view);

		count = 0;
		if (showStatTimer > 1)
		{
			showStatTimer = 0;
			for (objectsIterator = objects.begin(); objectsIterator != objects.end(); ++objectsIterator)
			{
				count++;
				(*objectsIterator)->Update(window);
			}
		}
		else {
			for (objectsIterator = objects.begin(); objectsIterator != objects.end(); ++objectsIterator)
			{
				count++;
				(*objectsIterator)->Update(window);
			}
		}
		window.draw(cursorSprite);

		UpdateInstancer();

		for (objectsIterator = objects.begin(); objectsIterator != objects.end(); ++objectsIterator)
		{
			if ((*objectsIterator)->GetTransformComponent()->GetDestroy())
			{
				(*objectsIterator)->Destroy();
			}
		}

		MemClean();

		if (levelNumber == 1)
		{

			std::ostringstream playerScoreString;  
			playerScoreString << points;	
			text.setString("Points:" + playerScoreString.str());
			text.setPosition(view.getCenter().x + 450, view.getCenter().y - 300);
			text.setCharacterSize(24);
			window.draw(text);
		}

		if (levelNumber == 0 )
		{ 
			if (curentSelectedMenu == 5)
			{
				std::ostringstream playerScoreString;   
				playerScoreString << points;	
				text.setString(playerScoreString.str());
				text.setPosition(view.getCenter().x - 100, view.getCenter().y - 200);
				text.setCharacterSize(48);

				nameText.setPosition(view.getCenter().x - 100, view.getCenter().y);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					saveScore = true;
					curentSelectedMenu = -1;
				}

				window.draw(text);//рисую этот текст
				window.draw(nameText);
			}

			if (curentSelectedMenu == 1)
			{
				if (readScores == true)
				{
					for (scoresIterator = scores.begin(); scoresIterator != scores.end();)
					{
						delete(*(scoresIterator));
						scoresIterator = scores.erase(scoresIterator);
					}

					readScores = false;
					std::ifstream fin("scores.txt");
					std::string readedName;
					std::string readedPoints;
					int c = 0;
					while (std::getline(fin, readedPoints) && std::getline(fin, readedName))
					{
						c++;
						if (scores.begin() == scores.end())
						{
							
							scores.push_back(new Score(readedName, std::stoi(readedPoints)));
						}
						else {
							for (scoresIterator = scores.begin(); scoresIterator != scores.end(); )
							{
								if ((*scoresIterator)->pPoints > std::stoi(readedPoints))
								{
									scoresIterator++;
								}
								else {
									scores.insert(scoresIterator, new Score(readedName, std::stoi(readedPoints)));
									break;
								}
							}
						}
					}
				}
				int i = 0;
				Vector2f listStartPosition;
				int listStep = 24;
				for (scoresIterator = scores.begin(); scoresIterator != scores.end(); scoresIterator++)
				{
					if (i < 10)
					{
						i++;

						Text nText("", font, 12);
						nText.setColor(Color::White);
						nText.setStyle(Text::Bold );
							
						std::ostringstream playerNameString;
						playerNameString << i << ". " + (*scoresIterator)->pName;
						nText.setString( playerNameString.str());
						nText.setPosition(view.getCenter().x - 300, view.getCenter().y - 200 + (listStep * i));
						window.draw(nText);

						Text pText("", font, 12);
						pText.setColor(Color::White);
						pText.setStyle(Text::Bold);

						std::ostringstream playerPointsString;
						playerPointsString << (*scoresIterator)->pPoints;
						pText.setString(playerPointsString.str());
						pText.setPosition(view.getCenter().x , view.getCenter().y - 200 + (listStep * i));
						window.draw(pText);
					}
					else { break; }
				}
			}
			else { 
				for (scoresIterator = scores.begin(); scoresIterator != scores.end();)
				{
					delete(*(scoresIterator));
					scoresIterator = scores.erase(scoresIterator);
				}
			}
		}

		if (saveScore)
		{
			saveScore = false;
			std::ofstream fout("scores.txt", std::ios_base::app);
			fout << points << std::endl;
			fout << playerName << std::endl;
			playerName = "";
			nameText.setString("");
			fout.close();
		}

		window.display();
		
	}
	return 0;
}