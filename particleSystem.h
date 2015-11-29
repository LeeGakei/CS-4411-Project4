/***********************
 * ParticleSystem class
 ***********************/

/**
 * The particle system class simply "manages" a collection of particles.
 * Its primary responsibility is to run the simulation, evolving particles
 * over time according to the applied forces using Euler's method.
 * This header file contains the functions that you are required to implement.
 * (i.e. the rest of the code relies on this interface)
 * In addition, there are a few suggested state variables included.
 * You should add to this class (and probably create new classes to model
 * particles and forces) to build your system.
 */

#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "vec.h"
#include "particle.h"
#include<vector>

class CacheStructure;

class ParticleSystem {

public:



	/** Constructor **/
	ParticleSystem();


	/** Destructor **/
	virtual ~ParticleSystem();

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawParticles(float t);

	// This fxn should save the configuration of all particles
	// at current time t.
	virtual void bakeParticles(float t);

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	virtual void computeForcesAndUpdateParticles(float t);

	// This function should reset the system to its initial state.
	// When you need to reset your simulation, PLEASE USE THIS FXN.
	// It sets some state variables that the UI requires to properly
	// update the display.  Ditto for the following two functions.
	virtual void resetSimulation(float t);

	// This function should start the simulation
	virtual void startSimulation(float t);

	// This function should stop the simulation
	virtual void stopSimulation(float t);

	// This function should clear out your data structure
	// of baked particles (without leaking memory).
	virtual void clearBaked();	

	void applyForce(double fx,double fy,double fz);

	void addParticleStartingAt(double x, double y, double z);

	// These accessor fxns are implemented for you
	float getBakeStartTime() { return bake_start_time; }
	float getBakeEndTime() { return bake_end_time; }
	float getBakeFps() { return bake_fps; }
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }



protected:
	


	/** Some baking-related state **/
	float bake_fps;						// frame rate at which simulation was baked
	float bake_start_time;				// time at which baking started 
										// These 2 variables are used by the UI for
										// updating the grey indicator 
	float bake_end_time;				// time at which baking ended

	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)

	int startSize = 10;					//The initial size of particles when emitted
	double positionDeviation = 1;		//start point deviation range
	double emissionRate = 20;			//particles emitted per second
	int maxParticles = 100;
	double lifetime = 3;				//lifetime of particle

	double lastTime = 0;					//the last currTime

	//position
	double x = 0;
	double y = 0;
	double z = 0;

	//force
	double fx = 0;
	double fy = 0;					//gravity direction
	double fz = 0;					//shooting direction

	Particle *p;						//only one particle now
	vector<Particle*> particleList;

	vector<CacheStructure> cache;
};

class CacheStructure{
public :
	float time;
	vector<Particle> particleList;
	CacheStructure(vector<Particle*> list,float t){
		this->time = t;
		for (int i = 0; i < list.size(); i++){
			particleList.push_back(Particle(list[i]));
		}
	}
};


#endif	// __PARTICLE_SYSTEM_H__
