#ifndef EXPLOSION_H
#define EXPLOSION_H

class ExplosionController {
	TransformComponent *t;
	float health;
	bool destroy;
public:
	ExplosionController() {}
	ExplosionController(float _health, TransformComponent *_t)
	{
		t = _t;
		health = _health;
	}

	void SetTransform(TransformComponent *_t){t = _t;}
	void SetHeatlh(float _health){health = _health;}
	void Destroy(){destroy = true;} 

	TransformComponent *GetTransform() { return t; }
	float GetHealth(){return health;}
	bool GetDestroy(){return destroy;}

	void Update()
	{
		health -= deltaTime;
		if (health <= 0){t->Destroy();}
	}
};


std::list<ExplosionController*> explosions;
std::list<ExplosionController*>::iterator explosionsIterator;

ExplosionController *CreateExplosion(float _health, TransformComponent *_t)
{
	ExplosionController *c = new ExplosionController(_health, _t);
	explosions.push_back(c);
	return c;
}
#endif