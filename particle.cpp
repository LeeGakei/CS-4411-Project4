 // SAMPLE_SOLUTION
#include "particle.h"
#include "bitmap.h"
#include <FL/gl.h>
#include "modelerdraw.h"

bool imageLoaded = false;
unsigned char* image = NULL;
int imageWidth;
int imageHeight;

void Particle::setImage(const char *fname, int& width, int& height){
	if (!imageLoaded){
		image = readBMP(fname, width, height);
		imageHeight = height;
		imageWidth = width;
		imageLoaded = true;
	}

}

void Particle::drawParticle(){
	glPushMatrix();
	std::cout << z << " ";
	glTranslated(x, y, z);
	drawSphere(0.1);
	glPopMatrix();
}

void Particle::setInitialVelocity(double vx, double vy, double vz){
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
}
