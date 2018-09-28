#ifndef DRAWN_H
#define DRAWN_H
#include "Transform.h"
using namespace sf;

class DrawComponent {
public:
	Texture * texture;
	Sprite sprite;
	BlendMode blendMode;
	Vector2f origin;
	Transform *transform;

	void SetTexture(Texture *tex);

	void SetOrigin(Vector2f _orig);

	void SetBlendMode(BlendMode blMd);
	
	DrawComponent() {}
	DrawComponent(Texture *_tex, BlendMode _blMd)
	{
		SetTexture(_tex);
		SetOrigin(Vector2f(sprite.getTextureRect().width/2.0f, sprite.getTextureRect().height/2));
		SetBlendMode(_blMd);
	}
	
	void SetTexture(Texture *tex)
	{
		texture = tex;
		sprite.setTexture(*texture);
	}

	void SetOrigin(Vector2f _orig)
	{
		origin = _orig;
		sprite.setOrigin(origin);
	}

	void SetBlendMode(BlendMode blMd)
	{
		blendMode = blMd;
	}



	void Draw(RenderWindow &window)
	{
		window.draw(sprite);
	}
};

std::list<DrawComponent*> draws;
std::list<DrawComponent*>::iterator drawsIT;

void CreateDrawn(Texture *_tex, BlendMode _blMd)
{
	DrawComponent *c = new DrawComponent(_tex, _blMd);
	draws.push_back(c);
}

#endif