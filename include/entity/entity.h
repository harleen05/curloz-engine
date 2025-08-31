#pragma once
#include "physics/core.h"
#include "shader/shaders.h"
#include "loader/model.h"
#include "camera/camera.h"
#include "renderer/lighting.h"
#include "renderer/renderCube.h"
#include "global/general.h"
#include "physics/particle.h"
#include "physics/forceGen.h"
#include "toml/toml.hpp"
#include <map>
#include <memory>
#include <vector>


class Entity
{
    public:
        static toml::table config;
        static void loadEntities();
        static void saveConfig();

        static void drawImGuiMenu();

        static Camera *camera;

        static glm::mat4 viewMatrix;
        static glm::mat4 projectionMatrix;
        
        static DirectionalLight directionLight;
        static std::vector<PointLight> PointLights;
        static int numInfluencingLights;
        static float distanceEntityAndLight(physics::Vector3 entityPos, glm::vec3 lightPos);

        static int numEntities;
        static std::vector<std::unique_ptr<Entity> > entities;

        static physics::ParticleForceRegistry registry;
        static particleGravity gravity;
        static particleDrag drag;

    private:

        int ID;
        toml::table *config_entity;
        float scale;
        glm::vec3 rotation;
        glm::mat4 modelMatrix;
        Model ourModel;
        // Shader &shader;
        std::map<std::string, float> rotationMap;

        std::vector<PointLight> nearestLights;
        void sortNearestLights(std::vector<PointLight> &PointLights);

        Shader shader = Shader("assets/shaders/assimpShader.vert","assets/shaders/assimpShader.frag");

    public:
        Entity(std::string modelPath);
        Particle particle = Particle(physics::Vector3(0.0f,5.0f,0.0f));
        std::string name;
        void setPosition(glm::vec3 newPosition);
        glm::vec3 getPosition();
        void setRotation(glm::vec3 axis, float angle);
        glm::vec3 getRotation();
        void setScale(float scale);
        float getScale();

        void update(float deltaTime);
};
