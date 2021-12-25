// Lights.h

#ifndef LIGHTS_HDR
#define LIGHTS_HDR

#include "Camera.h"
#include "VecMat.h"
#include "Widgets.h"
#include <vector>

using std::vector;

class Light {
public:
	vec3 p;  // light location
	int cid; // color palette index
	Light(vec3 p = vec3(0,0,0), int cid = 0) : p(p), cid(cid) { }
};

class Lights {
public:
	Lights() { } //picked = NULL; }
	Lights(vec3 p) { lights.resize(1, p); }
	Lights(vec3 p1, vec3 p2) { lights.resize(2); lights[0] = p1; lights[1] = p2; }
	Lights(vec3 p1, vec3 p2, vec3 p3) { lights.resize(3); lights[0] = p1; lights[1] = p2; lights[2] = p3; }
	Lights(vector<vec3> points) {
		int npoints = points.size();
		lights.resize(npoints);
		for (int i = 0; i < npoints; i++)
			lights[i] = Light(points[i]);
	}
	int NLights() { return lights.size(); }
	vector<Light> lights;
	Light *picked = NULL;
	Mover mover;
	void Transform(mat4 view, vector<vec3> &xLights);
	void TransformSetColors(mat4 view, vector<vec3> &xLights, vector<vec3> &colors);
	void Display(void *hover);
	Light *MouseOver(int x, int y, mat4 fullview);
	bool MouseDown(int x, int y, mat4 modelview, mat4 persp);
	void MouseDrag(int x, int y, mat4 modelview, mat4 persp);
	void ChangeColor();
	void AddLight();
	void DeleteLight();
	void Save(const char *filename);
	bool Read(const char *filename);
};

#endif
