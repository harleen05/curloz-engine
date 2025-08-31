#pragma once
#include "forceGen.h"

using namespace physics;

//A normal spring 
class particleSpring : public ParticleForceGenerator 
{
    private:
        //Stores pointer to other end of spring
        Particle *other;

        //Holds the spring constant
        real springConstant;

        //Holds the normal unstretched/compressed length of spring
        real restLength;

    public:
        //Create spring with given parameters
        particleSpring(Particle *other, real springConstant, real restLength);

        //update function 
        void updateForce(Particle *particle, real duration) override;

};

//A type of spring where one end is attatched properly
class particleAnchoredSpring : public ParticleForceGenerator 
{
    private:
        //location of anchored/fixed end of spring
        Vector3 anchor;

        //holds the spring constant
        real springConstant;

        //holds the rest length of spring
        real restLength;

    public:
        //create our anchored spring
        particleAnchoredSpring(Vector3 anchor, real springConstant, real restLength);

        //update function
        void updateForce(Particle *particle, real duration) override;
};

//A force generator, that only acts when extended(not compressed)
class particleBungee : public ParticleForceGenerator 
{
    private:
        //Particle at other end of spring
        Particle *other;

        //holds the spring constant
        real springConstant;

        //original length
        real restLength;

    public:
        //our bungee spring
        particleBungee(Particle *other, real springConstant, real restLength);

        //update function
        void updateForce(Particle *particle, real duration) override;

};
