#ifndef WINDOW_H
#define WINDOW_H

#include "UIController.h"
#include "DrawComponent.h"
#include "view.h"

class TextWindow {
	Vector2f relativePosition;
	TransformComponent *transform;
	DrawComponent *drawer;
	int selfMenuNum;
	bool destroy;
public:
	TextWindow() {}
	TextWindow(TransformComponent *_transform, DrawComponent *_drawer, int _selfMenu)
	{
		transform = _transform;
		relativePosition = transform->GetPosition();
		drawer = _drawer;
		selfMenuNum = _selfMenu;
		destroy = false;
	}

	void Update(RenderWindow &window)
	{
		transform->SetPosition(view.getCenter() - (view.getSize() / 2.0f) + relativePosition);
		if (curentSelectedMenu == selfMenuNum)
		{
			drawer->SetColor(Color(255,255,255,255));
		}
		else {
			drawer->SetColor(Color(255, 255, 255, 0));
		}
	}
	void Destroy(){destroy = true;}
	bool GetDestroy(){return destroy;}
};

std::list<TextWindow*> textWindows;
std::list<TextWindow*>::iterator windowsIterator;

TextWindow *CreateTextWindow(TransformComponent *_transform, DrawComponent *_drawer, int _selfMenu)
{
	TextWindow *c = new TextWindow(_transform, _drawer, _selfMenu);
	textWindows.push_back(c);
	return c;
}
#endif