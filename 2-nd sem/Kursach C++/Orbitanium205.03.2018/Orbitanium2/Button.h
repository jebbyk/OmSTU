#ifndef BUTTON_H
#define BUTTON_H

#include "DrawComponent.h"
#include "view.h"
#include "UIController.h"

class Button {
	Vector2f relativePosition;
	float width;
	float heigth;
	Color normalColor;
	Color onCursorColor;
	Color onClickColor;
	TransformComponent *transform;
	DrawComponent *drawer;
	int openingMenuNum;
	int selfMenuNum;
	bool destroy;
public:
	Button(){}
	Button(float w, float h, Color _normalColor, Color _onCursorColor, Color _onClickColor, TransformComponent *_transform, DrawComponent *_drawer, int _openingMenu, int _selfMenu)
	{
		width = w;
		heigth = h;
		normalColor = _normalColor;
		onCursorColor = _onCursorColor;
		onClickColor = _onClickColor;
		transform = _transform;
		relativePosition = transform->GetPosition();
		drawer = _drawer;
		openingMenuNum = _openingMenu;
		selfMenuNum = _selfMenu;
		destroy = false;
	}

	bool IsUnderCursor()
	{
		bool b = false;
		if (mousePosition.x  > transform->GetPosition().x - view.getCenter().x + view.getSize().x/2.0f - width / 2)
		{
			if (mousePosition.x < transform->GetPosition().x - view.getCenter().x + view.getSize().x / 2.0f + width / 2)
			{
				if (mousePosition.y > transform->GetPosition().y - view.getCenter().y + view.getSize().y / 2.0f - heigth / 2)
				{
					if (mousePosition.y < transform->GetPosition().y - view.getCenter().y + view.getSize().y / 2.0f + width / 2)
					{
						b = true;
					}
				}
			}
		}
		return b;
	}

	void Update(RenderWindow &window)
	{
		transform->SetPosition(view.getCenter() - (view.getSize()/2.0f) + relativePosition);
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { curentSelectedMenu = -1; }
		
		if (IsUnderCursor() == true)
		{
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{ 
				if (transform->GetName() == "ExitButton"){window.close();}
				if (transform->GetName() == "RecordsButton")
				{
					curentSelectedMenu = 1;
					if (readScores == false)
					{
						readScores = true;
					}
				}
				if (transform->GetName() == "AuthorButton"){curentSelectedMenu = 3;}
				if (transform->GetName() == "RulesButton"){curentSelectedMenu = 4;}
				if (transform->GetName() == "PlayButton")
				{
					loadNext = true;
					points = 0;
				}
				if (transform->GetName() == "BackButton")
				{
					printf("aaaaaaaaaaaaaaaaaaaaaaaaa\n");
					loadMain = true;
				}
			}
			else
			{ 
				if (selfMenuNum != curentSelectedMenu)
				{
					curentMarkedButton = selfMenuNum;
				}
			}
		}
		
		if (curentSelectedMenu == selfMenuNum)
		{
			drawer->SetColor(onClickColor);
		}
		else {
			if (curentMarkedButton == selfMenuNum)
			{
				drawer->SetColor(onCursorColor);
			}
			else {
				drawer->SetColor(normalColor);
			}
		}

	}

	void Destroy()
	{
		destroy = true;
	}
	bool GetDestroy()
	{
		return destroy;
	}
};

std::list<Button*> buttons;
std::list<Button*>::iterator buttonsIterator;

Button* CreateButton(float w, float h, Color _normalColor, Color _onCursorColor, Color _onClickColor, TransformComponent *_transform, DrawComponent *_drawer, int _openingMenu, int _selfMenu)
{
	Button *c = new Button(w, h, _normalColor, _onCursorColor, _onClickColor, _transform, _drawer, _openingMenu, _selfMenu);
	buttons.push_back(c);
	return c;
}
#endif