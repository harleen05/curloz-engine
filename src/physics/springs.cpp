#include "physics/springs.h"

//normal spring
particleSpring::particleSpring(Particle *other, real springConstant, real restLength) : 
    other(other), springConstant(springConstant), restLength(restLength)
{

}

void particleSpring::updateForce(Particle *particle, real duration)
{
    //Calculate vector of spring
    Vector3 force;
    force = particle->position;
    force -= other->position;

    //Calculate magnitude of force
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    //Calculate final force and apply it
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}


//Anchored spring
particleAnchoredSpring::particleAnchoredSpring(Vector3 anchor, real springConstant, real restLength) :
    anchor(anchor), springConstant(springConstant), restLength(restLength)
{

}

void particleAnchoredSpring::updateForce(Particle *particle, real duration) 
{
    Vector3 force;    
    force = particle->position;
    force -= anchor;

    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

//Bungee spring
particleBungee::particleBungee(Particle *other, real springConstant, real restLength) :
    other(other), springConstant(springConstant), restLength(restLength)
{
    
}

void particleBungee::updateForce(Particle *particle, real duration) 
{
    //calculate vector of spring
    Vector3 force;
    force = particle->position;
    force -= other->position;
    
    //check if bungee is compressed
    real magnitude = force.magnitude();
    if(magnitude < restLength) return;

    //calculate magnitude of force
    magnitude = springConstant * (magnitude - restLength);

    //calculate final force
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}


