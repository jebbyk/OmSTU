#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H
#include <SFML/Graphics.hpp>
using namespace sf;

std::list<Texture*> textures;
std::list<Texture*>::iterator texturesIterator;

Texture* LoadTexture(String path)
{
	Texture *c = new Texture();
	if (!c->loadFromFile(path + ".png"))
	{
		if (!c->loadFromFile(path + ".jpg"))
		{
			if (!c->loadFromFile(path + ".bmp"))
			{
				if (!c->loadFromFile(path + ".psd"))
				{
					Image img;
					img.create(50, 50, Color(100, 100, 100, 100));
					c->loadFromImage(img);
				}
			}
		}
	}
	c->setSmooth(true);
	return c;
}

void DeleteTexture(Texture *link)
{
	delete(link);
	link = NULL;
}
#endif
