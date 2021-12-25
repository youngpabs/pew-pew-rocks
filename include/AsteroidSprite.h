#ifndef ASTEROIDSPRITE_H
#define ASTEROIDSPRITE_H
#include <glad.h>
#include <GLFW/glfw3.h>
#include "laserSprite.h"
#include "Sprite.h"
#include <time.h>
class AsteroidSprite : public Sprite{
public: 
	void SetStartTime(time_t);
	void Animate();
	void StartPoint();
	void laserCollision(LaserSprite&);
	void earthCollision(Sprite&, int&);
	bool collided = false, follow = false;
	vec2 earth = vec2(0.0f,0.0f), start;
	time_t followStart;
	float followDuration = 5.0f;
	float startDistanceToEarth = 0.0f;
	int wave = 1, speedMultiplier = 1, earthCollisionPause = 0, laserCollisionPause = 0;
};
#endif 