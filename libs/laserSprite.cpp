// laserSprite.cpp
// Class For Individual Laser Sprite Usage

#include "laserSprite.h"
#include <iostream>
#include "Sprite.h"
#include <cmath>
#include <time.h>
#include <glad.h>
#define PI 3.14159265


void LaserSprite::Fire(vec2 mouse) {
	follow = true;
	followStart = clock();
	double x, y, theta;
	int winHeight = 800; winHeight = 800;
	target = mouse;
	x = mouse.x; y = mouse.y;
	theta = atan(y / x) * 180 / PI;
	if (x < 0)
		theta += 180;
	rotation = (float) theta;
	//printf("\n\nangle: %f\n\nx:%f y:%f", theta,x,y);
}

void LaserSprite::Animate() {
	// if sprite following target, compute new sprite position
	float dt = (float)((float)(clock() - followStart) / CLOCKS_PER_SEC);
	if (follow && dt < followDuration) {
		float t = dt / followDuration;
		SetPosition(startLaser + t * (target - startLaser));
	}
	else {
		follow = false;
		SetPosition(startLaser); // Reset Laser To Center Once Off Screen
	}
}
