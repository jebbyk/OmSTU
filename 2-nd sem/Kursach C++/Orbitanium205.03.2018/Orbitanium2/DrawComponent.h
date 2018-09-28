#ifndef DRAW_COMPONENT_H
#define DRAW_COMPONENT_H
#include "TransformComponent.h"

class DrawComponent {
	Texture *texture;
	Sprite sprite;
	Vector2f origin;//cdenter of the sprite
	BlendMode blendMode;
	TransformComponent *transform;
	bool destroy = false;
	Vector2f curentPosition;
public:
	DrawComponent() {}
	DrawComponent(Texture *_texture, BlendMode _blendMode, TransformComponent *_transform)
	{
		texture = _texture;
		blendMode = _blendMode;
		transform = _transform;
		sprite.setTexture(*texture, true);
		sprite.setScale(transform->GetScale());
		sprite.setRotation(transform->GetRotation());
		sprite.setPosition(transform->GetPosition());
		origin.x = sprite.getTextureRect().width / 2;
		origin.y = sprite.getTextureRect().height / 2;//setting origin to center of loaded image
		sprite.setOrigin(origin);
	}

	void SetTexture(Texture *_texture){texture = _texture;}
	void SetOrigin(Vector2f _origin){origin = _origin;}
	void SetBlendMode(BlendMode _blendMode){blendMode = _blendMode;}
	void SetTransform(TransformComponent *_transform){transform = _transform;}
	void SetColor(Color _color){sprite.setColor(_color);}
	void Destroy(){destroy = true;}

	Texture *GetTexture(){return texture;}
	Vector2f GetOrigin(){return origin;}
	BlendMode GetBlendMode(){return blendMode;}
	TransformComponent *GetTransform(){return transform;}
	Color GetColor(){return sprite.getColor();}
	bool GetDestroy(){return destroy;}

	void Draw(RenderWindow &window)
	{
		if (!transform->GetDestroy())
		{
			curentPosition = transform->GetPosition();
			if (curentPosition.x > view.getCenter().x - 200 - view.getSize().x/2.0f && curentPosition.y > view.getCenter().y - 200 - view.getSize().y / 2.0f && curentPosition.x < view.getCenter().x + 200 + view.getSize().x / 2.0f && curentPosition.y < view.getCenter().y + 200 + view.getSize().y / 2.0f)
			{
				sprite.setPosition(transform->GetPosition());
				sprite.setScale(transform->GetScale());
				sprite.setRotation(transform->GetRotation());
				window.draw(sprite, blendMode);

			}
		}
	}
};

std::list<DrawComponent*> drawings;
std::list<DrawComponent*>::iterator drawingsIterator;

DrawComponent* CreateDraw(Texture *_texture, BlendMode _blendMode, TransformComponent *_transform)
{
	DrawComponent *c = new DrawComponent(_texture, _blendMode, _transform);
	drawings.push_back(c);
	return c;
}
#endif