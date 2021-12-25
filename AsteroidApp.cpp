// AstroidApp.cpp Main Appliation For The Game
// Copywrite 2020 
// Michael Pablo

#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>
#include "Draw.h"
#include "GLXtras.h"
#include "Sprite.h"
#include "laserSprite.h"
#include "AsteroidSprite.h"
#include <cmath>
#include <string>
#include <cstdlib>
#define PI 3.14159265


int winWidth = 800, winHeight = 800, loadSize = 5, shotsLeft = 5, currLives = 2, spawnRate = 50, spawn , totalWaveSpawn = 5, currAsteroid;
bool started = false, waveStart = false, isRoundOver = false, win = false, lose = false;
const int maxClip = 10;
const int totalAsteroids = 15;
// Objects
Sprite backgroundSprite, earthSprite, reticle, startScreen, nextRound, winScreen, loseScreen, lives[3];
LaserSprite laserClip[maxClip];
AsteroidSprite asteroid[totalAsteroids];
vec2 center(400, 400), startLaser;


void Display() {
	backgroundSprite.Display();
	for (int i = 0; i < maxClip; i++) 
		laserClip[i].Display();
	earthSprite.Display();
	for(int i = totalWaveSpawn; i >= 0; i--)
		if (asteroid[i].follow) asteroid[i].Display();
	reticle.Display();
	for (int i = currLives; i >= 0; i--)
		lives[i].Display();
	if(!started) startScreen.Display();
	if (isRoundOver) {
		nextRound.SetPosition(vec2(0.0f, 0.0f));
		nextRound.Display();
	}
	if (lose) loseScreen.Display();
	if (win) winScreen.Display();
	glFlush();
}

void spriteInitialization() {
	// read background, foreground, mat textures, and setting origin for projectiles
	std::string dir = "assets/";
	std::string background = dir + "space.jpg", earthPNG = dir + "earthR.png", mat = dir + "Circle-Mat.tga", reticle1PNG = dir + "Reticle_1.png", reticle1TGA = dir + "Reticle_1.tga";
	std::string laserPNG = dir + "Laser.png", laserTGA = dir + "Laser-Mat.tga";
	std::string astroidPNG = dir + "Asteroid.png", astroidTGA = dir + "Asteroid-Mat.tga";
	std::string splashScreen = dir + "Start_SplashScreen.png", nextRoundScreen = dir + "NextRound.png", nextRoundMAT = dir + "NextRoundMat.tga"; 
	std::string loseSplash = dir + "Ded.png", winSplash = dir + "Win.png";
	reticle.Initialize(reticle1PNG.c_str(), reticle1TGA.c_str());
	reticle.SetScale(vec2(0.05f,0.05f));
	lives[0].Initialize(earthPNG.c_str(), mat.c_str());
	lives[0].SetScale(0.075f);
	lives[0].SetPosition(vec2(-0.85, -0.9));
	lives[1].Initialize(earthPNG.c_str(), mat.c_str());
	lives[1].SetScale(0.075f);
	lives[1].SetPosition(vec2(-0.7, -0.9));
	lives[2].Initialize(earthPNG.c_str(), mat.c_str());
	lives[2].SetScale(0.075f);
	lives[2].SetPosition(vec2(-0.55, -0.9));
	for (int i = 0; i < maxClip; i++) {
		laserClip[i].Initialize(laserPNG.c_str(), laserTGA.c_str());
		laserClip[i].SetScale(vec2(0.1f,0.1f));
		laserClip[i].position = vec2(0.5f, 0.5f);
		startLaser = laserClip[i].GetPosition();
	}
	for (int i = 0; i < totalAsteroids; i++) {
		asteroid[i].Initialize(astroidPNG.c_str(), astroidTGA.c_str());
		asteroid[i].SetScale(vec2(0.1f, 0.1f));
		asteroid[i].SetPosition(vec2(1.5f, 1.5f));
	}
	startScreen.Initialize(splashScreen.c_str());
	backgroundSprite.Initialize(background.c_str(), NULL, 1);
	earthSprite.Initialize(earthPNG.c_str(), mat.c_str());
	earthSprite.SetScale(0.25);
	nextRound.Initialize(nextRoundScreen.c_str(), nextRoundMAT.c_str());
	loseScreen.Initialize(loseSplash.c_str(), nextRoundMAT.c_str());
	winScreen.Initialize(winSplash.c_str(), nextRoundMAT.c_str());
}

void asteroidSpawn() {

	if (clock() % 100 == 0) {
		spawn = rand() % 100 + 1;
		if (currAsteroid >= 0 && spawn < spawnRate) {
			asteroid[currAsteroid].SetStartTime(clock());
			asteroid[currAsteroid].StartPoint();
			currAsteroid--;
		}
	}
}

void waveControl() {
	if (!waveStart) {
		currAsteroid = totalWaveSpawn;
		waveStart = !waveStart;
		//printf("\nSTART");
	}
	if (currAsteroid == -1 && asteroid[0].collided) {
		waveStart = !waveStart;
		isRoundOver = true;
		//printf("\nWAVE END");
		if (totalWaveSpawn < totalAsteroids) totalWaveSpawn += 5;
		else if (totalWaveSpawn == totalAsteroids && spawnRate < 50) spawnRate += 10;
		else win = true;
	}
}

void Resize(GLFWwindow* w, int width, int height) { glViewport(0, 0, winWidth = width, winHeight = height);}

// Controls

void MouseButton(GLFWwindow *w, int butn, int action, int mods) {
	// begin animation	
	if (!started && action == GLFW_PRESS) {
		for (int i = 0; i < winHeight; i++) {
			startScreen.SetPosition(vec2(0.0f, 2.0f));
		}
		started = true;
	}
	else if (isRoundOver && action == GLFW_PRESS) {
		for (int i = 0; i < winHeight; i++) {
			nextRound.SetPosition(vec2(0.0f, 2.0f));
		}
		isRoundOver = false;
	}
	else {
		if (action == GLFW_PRESS && shotsLeft != 0) {
			double x, y;
			glfwGetCursorPos(w, &x, &y);
			y = winHeight - y; // invert y for upward-increasing screen space
			vec2 mouse(2 * x / winWidth - 1, 2 * y / winHeight - 1);
			float slope = (mouse.y / mouse.x) * -1;
			if (abs(mouse.x) > abs(mouse.y)) { // Find which coordinate is closer to an edge of the screen
				if (mouse.x > 0)
					mouse.x = 1.1f;
				else
					mouse.x = -1.1f;
				mouse.y = (mouse.x * slope) * -1;
			}
			else {
				if (mouse.y > 0)
					mouse.y = 1.1f;
				else
					mouse.y = -1.1f;
				mouse.x = (mouse.y / slope) * -1;
			}
			
			laserClip[shotsLeft].Fire(mouse);
			shotsLeft--;
		}
			
	}
}

void Keyboard(GLFWwindow*w, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		shotsLeft = loadSize;
	}
}

void MouseMove(GLFWwindow *w, double x, double y) {
	// possibly drag target
	y = winHeight-y;
	vec2 mouse(2*x/winWidth-1, 2*y/winHeight-1); 
	reticle.SetPosition(mouse);
}

int main(int ac, char **av) {
	// init app window and GL context
	glfwInit();
	GLFWwindow *w = glfwCreateWindow(winWidth, winHeight, "Asteroids!", NULL, NULL);
	glfwSetWindowPos(w, 100, 100);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	spriteInitialization();
	//printf("Usage: Use the mouse to aim and shoot your laser!\n\nUse the R key to reload once you are out of lasers\n");
	// callbacks
	glfwSetMouseButtonCallback(w, MouseButton);
	glfwSetCursorPosCallback(w, MouseMove);
	glfwSetWindowSizeCallback(w, Resize);
	glfwSetKeyCallback(w, Keyboard);
	// event loop
	glfwSwapInterval(1);
	srand((unsigned int)time(NULL));
	while (!glfwWindowShouldClose(w)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(.5f, .5f, .5f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		waveControl();
		if (started && !isRoundOver && !win && !lose) {
			asteroidSpawn();
			for (int i = loadSize; i >= 0; i--) {
				laserClip[i].Animate();
			}
			for (int i = totalWaveSpawn; i >= 0; i--) {
				asteroid[i].earthCollision(earthSprite, currLives);
				asteroid[i].Animate();
				for (int j = totalWaveSpawn; j >= 0; j--) {
					asteroid[j].laserCollision(laserClip[i]);
				}
			}
			if (currLives == -1) lose = true;
		}
		Display();
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	// terminate
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	backgroundSprite.Release();
	earthSprite.Release();
	for (int i = 0; i < maxClip; i++)
		laserClip[i].Release();
	reticle.Release();
	startScreen.Release();
	
	glfwDestroyWindow(w);
	glfwTerminate();
}
