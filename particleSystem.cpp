#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerapp.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	// TODO
}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	delete p;

	for (int i = 0; i < particleList.size(); i++){
		delete particleList[i];
	}
}


/******************
 * Simulation fxns
 ******************/

double const INITIAL_Z_VELECITY = 20;

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	this->applyForce(0, -9.8, -9.8);
	// TODO

	//generate some random particles and store them.
	for (int i = 0; i < startSize; i++){
		double ramdomx = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + x;
		double ramdomy = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + y;
		double ramdomz = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + z;
		Particle *p = new Particle(1, ramdomx, ramdomy, ramdomz,lifetime);
		p->setInitialVelocity(0, 0, INITIAL_Z_VELECITY);
		particleList.push_back(p);
	}


	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	resetSimulation(t);

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO
	this->clearBaked();
	particleList.clear();
	this->fx = 0;
	this->fy = 0;
	this->fz = 0;
	this->x = 0;
	this->y = 0;
	this->z = 0;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if (!simulate) return;

	double interval = t - lastTime;

	if (lastTime != 0 && particleList.size() < maxParticles){
		//add new particles
		double seed = (double)rand() / (RAND_MAX);
		if (interval * emissionRate > 1){
			int number = (int)(interval * emissionRate);
			for (int i = 0; i < number; i++){
				double ramdomx = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + x;
				double ramdomy = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + y;
				double ramdomz = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + z;
				Particle *p = new Particle(1, ramdomx, ramdomy, ramdomz, lifetime);
				p->setInitialVelocity(0, 0, INITIAL_Z_VELECITY);
				particleList.push_back(p);
			}
		}
		else{
			if (seed < interval * emissionRate){
				double ramdomx = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + x;
				double ramdomy = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + y;
				double ramdomz = (((double)rand() / (RAND_MAX)) - 0.5) * positionDeviation + z;
				Particle *p = new Particle(1, ramdomx, ramdomy, ramdomz, lifetime);
				p->setInitialVelocity(0, 0, INITIAL_Z_VELECITY);
				particleList.push_back(p);
			}
		}
	}

	//change the current state of the particles
	for (int i = 0; i < particleList.size(); i++){
		Particle *p = particleList[i];

		//position and velocity
		p->x += p->vx * interval;
		p->y += p->vy * interval;
		p->z += p->vz * interval;
		p->vx += fx / p->m * interval;
		p->vy += fy / p->m * interval;
		p->vz += fz / p->m * interval;

		//lifetime
		p->life -= interval;
	}

	//remove out-date particles
	vector<Particle*>::iterator itor;
	for (itor = particleList.begin(); itor != particleList.end();){
		if ((*itor)->life <= 0){
			delete *itor;
			itor = particleList.erase(itor);
		}
		else{
			itor++;
		}
	}

	bakeParticles(t);
	// TODO
	lastTime = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if (!simulate) return;
	//std::cout << "drawing!" << "\n";

	for (int i = 0; i < cache.size(); i++){
		if (abs(t - cache[i].time) < 0.01){
			vector<Particle> particleList = cache[i].particleList;
			for (int j = 0; j < particleList.size(); j++){
				particleList[j].drawParticle();
			}
			return;
		}

		
	}
	std::cout << "error in ParticleSystem::drawParticles";
	// TODO
}

/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	cache.push_back(CacheStructure(particleList,t));
	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	cache.clear();
	// TODO
}

void ParticleSystem::applyForce(double fx, double fy, double fz){
	this->fx += fx;
	this->fy += fy;
	this->fz += fz;
}

void ParticleSystem::addParticleStartingAt(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}



