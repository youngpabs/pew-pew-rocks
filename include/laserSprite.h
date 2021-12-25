

#ifndef LASERSPRITE_H
#define LASERSPRITE_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include "Sprite.h"

class LaserSprite : public Sprite {
public:
	void Animate();
	void Fire(vec2);
	bool follow = false;
	vec2 startLaser,target;
	time_t followStart = 0;
	float followDuration = 1;
};

#endif // LASERSPRITE_H

