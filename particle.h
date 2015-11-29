 // SAMPLE_SOLUTION
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "point.h"

extern bool imageLoaded;
extern unsigned char* image;
extern int imageWidth;
extern int imageHeight;

class Particle{
public:
	double m;
	double vx;
	double vy;
	double vz;
	double x;
	double y;
	double z;
	double life;

	Particle(double mass,double px,double py,double pz,double l){
		this->m = mass;
		this->x = px;
		this->y = py;
		this->z = pz;
		vx = 0;
		vy = 0;
		vz = 0;
		life = l;
	};

	Particle(Particle* p){
		this->m = p->m;
		this->x = p->x;
		this->y = p->y;
		this->z = p->z;
		this->vx = p->vx;
		this->vy = p->vy;
		this->vz = p->vz;
		this->life = p->life;
	}

	//read a image as the particle
	void setImage(const char *fname, int& width, int& height);

	void setInitialVelocity(double vx,double vy,double vz);

	void drawParticle();
};

#endif