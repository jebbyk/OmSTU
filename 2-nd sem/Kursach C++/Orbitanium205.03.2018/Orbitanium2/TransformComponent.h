#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include <SFML/Graphics.hpp>
#include <string.h>
#include <list>
using namespace sf;

bool loadNext = false;
bool loadMain = false;
int levelNumber = 0;
bool saveScore = false;
bool readScores = false;
int points;
std::string playerName = "";
String _playerName = "Player1";

float deltaTime = 0;
float gameSpeed = 1;
Vector2f mousePosition;

class Score {
public:
	std::string pName;
	int pPoints;
	Score(std::string _name, int _points)
	{
		pName = _name;
		pPoints = _points;
	}
};
std::list<Score*> scores;
std::list<Score*>::iterator scoresIterator;

class TransformComponent {
	Vector2f position, scale, previousPosition;
	float rotation;
	String name, tag;
	bool destroy  = false;

public:
	TransformComponent() { }
	TransformComponent(Vector2f _position, Vector2f _scale, float _rotation, String _name, String _tag)
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
		name = _name;
		tag = _tag;
		destroy = false;
	}

	void SetPosition(Vector2f _position)
	{
		position = _position;
	}
	void SetRotation(float _rotation){rotation = _rotation;}
	void SetScale(Vector2f _scale){scale = _scale;}
	void SetName(String _name){name = _name;}
	void SetTag(String _tag){tag = _tag;}
	void Destroy(){destroy = true;}

	Vector2f GetPosition(){return position;}
	Vector2f GetScale(){return scale;}
	float GetRotation(){return rotation;}
	String GetName(){return name;}
	String GetTag(){return tag;}
	bool GetDestroy(){return destroy;}
	void Move(Vector2f _vector){SetPosition(position + (_vector*deltaTime));}
	void Rotate(float _angularSpeed){rotation += _angularSpeed * deltaTime;}
};

std::list<TransformComponent*> transforms;
std::list<TransformComponent*>::iterator transformsIterator;

TransformComponent* CreateTransform(Vector2f _position, Vector2f _scale, float _rotation, String _name, String _tag)
{
	TransformComponent *c = new TransformComponent(_position, _scale, _rotation, _name, _tag);
	transforms.push_back(c);
	return c;
}
#endif