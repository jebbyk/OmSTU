#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "spacecraft.h"
#include "view.h"

class PlayerController {
	Spacecraft *spacecraft;
	TransformComponent *transform;
	int controllPreset;
	bool destroy;
public:
	PlayerController(){}
	PlayerController(int _controllPreset, Spacecraft *_spacecraft, TransformComponent *_transform)
	{
		controllPreset = _controllPreset;
		spacecraft = _spacecraft;
		transform = _transform;
		destroy = false;
	}

	void SetControllPreset(int _controllPreset){controllPreset = _controllPreset;}
	void SetSpacecraft(Spacecraft *_spacecraft){spacecraft = _spacecraft;}
	void SetTransform(TransformComponent *_transform){transform = _transform;}

	int GetContorllPreset(){return controllPreset;}
	Spacecraft *GetSpacecraft(){return spacecraft;}
	TransformComponent *GetTransform(){return transform;}
	bool GetDestroy(){return destroy;}

	void Destroy(){destroy = true;}

	void Update()
	{
		Vector2f v = mousePosition + view.getCenter() - view.getSize() / 2.0f - transform->GetPosition();
		float vMod = v.x*v.x + v.y*v.y;
		v = v / vMod;
		transform->SetRotation(-((atan2(v.x, v.y) * 180) / 3.1416) + 180);
		spacecraft->SetThrotle(sf::Keyboard::isKeyPressed(sf::Keyboard::W));
		spacecraft->SetShoot(Mouse::isButtonPressed(sf::Mouse::Left));
	}
};

std::list<PlayerController*> players;
std::list<PlayerController*>::iterator playersIterator;

PlayerController *CreatePlayer(int _controllPreset, Spacecraft *_spacecraft, TransformComponent *_transform)
{
	PlayerController *c = new PlayerController(_controllPreset, _spacecraft, _transform);
	players.push_back(c);
	return c;
}
#endif