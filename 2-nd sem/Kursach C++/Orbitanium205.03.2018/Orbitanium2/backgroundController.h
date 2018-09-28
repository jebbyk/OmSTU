#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "view.h"
#include "TransformComponent.h"

class BackgroundController {
	Vector2f positionOnScreen;
	Vector2f viewPosition;
	TransformComponent *transform;
	bool destroy;
public:
	BackgroundController() {}
	BackgroundController( TransformComponent *t)
	{
		transform = t;
		destroy = false;
		positionOnScreen = transform->GetPosition();
	}

	void SetTransform(TransformComponent *t){transform = t;}
	void Destroy(){destroy = true;}
	void SetPositionOnScreen(Vector2f _p){positionOnScreen = _p;}

	TransformComponent *GetTransform(){return transform;}
	bool GetDestroy(){return destroy;}
	Vector2f GetScreenPostition(){return positionOnScreen;}

	void Update()
	{
		viewPosition = view.getCenter();
		viewPosition.x -= view.getSize().x / 2;
		viewPosition.y -= view.getSize().y / 2;
		transform->SetPosition(viewPosition + positionOnScreen);
	}
};

std::list<BackgroundController*> backgroundControllers;
std::list<BackgroundController*>::iterator backgroundsIt;

BackgroundController *CreateBackgroundController(TransformComponent *t)
{
	BackgroundController *c = new BackgroundController(t);
	backgroundControllers.push_back(c);
	return c;
}
#endif