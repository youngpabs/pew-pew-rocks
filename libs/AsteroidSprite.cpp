#include "AsteroidSprite.h"
#include <iostream>
#include "Sprite.h"
#include <cmath>
#include <time.h>
#include <glad.h>
#include <cstdlib>
using namespace std;

void AsteroidSprite::SetStartTime(time_t startT) { followStart = startT; }

void AsteroidSprite::Animate()
{
	float dt = (float)((float)(clock() - followStart) / CLOCKS_PER_SEC);
	if (follow && dt < followDuration) {
		float t = dt / followDuration;
		SetPosition(start + t * (earth - start));
		rotation += 1;
	}
	else {
		follow = false;
		SetPosition(start); // Reset Asteroid To Center Once Off Screen
	}
}

float Random(float min, float max) {
	float f = (float)(rand() % 1000);
	return min + (f / 1000) * (max - min);
}

void AsteroidSprite::StartPoint()
{
	srand((unsigned int)clock());
	int coord = rand() % 2;
	if (coord % 2 == 0){
		start = vec2(Random(1.1f, 1.2f), Random(0.0f, 1.1f));
	} else {
		start = vec2(Random(0.0f, 1.1f), Random(1.1f, 1.2f));
	}
	float xSign = Random(-1.0f, 1.0f), ySign = Random(-1.0f, 1.0f);
	if (xSign < 0) start.x *= -1; if (ySign < 0) start.y *= -1;
	SetPosition(start);
	startDistanceToEarth = length(earth - start);
	follow = true;
	collided = false;
}

void AsteroidSprite::laserCollision(LaserSprite &laser) {
	
	if (Intersect(laser) && follow && laser.position.x != 0.0f) {
		laserCollisionPause++;
		if (laserCollisionPause > 25) {
			//printf("\nLaser Collision");
			laser.SetPosition(vec2(0.0f, 0.0f));
			laser.follow = false;
			SetPosition(start);
			follow = false;
			collided = true;
			laserCollisionPause = 0;
		}
	}
}
void AsteroidSprite::earthCollision(Sprite &earth, int &lives) {
	if (Intersect(earth) && follow) {
		earthCollisionPause++;
		if (Intersect(earth) && earthCollisionPause > 100){
			//printf("\nEarth Collision");
			follow = false;
			collided = true;
			SetPosition(start);
			earthCollisionPause = 0;
			lives--;
		}
	} 
}

