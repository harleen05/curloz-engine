#include "entity/entity.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "physics/core.h"
#include "toml/toml.hpp"
#include "imgui/imgui.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>

Camera *Entity::camera;

glm::mat4 Entity::viewMatrix = generalInfo.viewMatrix;
glm::mat4 Entity::projectionMatrix = generalInfo.projectionMatrix;

DirectionalLight Entity::directionLight;
std::vector<PointLight> Entity::PointLights;
std::vector<std::unique_ptr<Entity> > Entity::entities;
// std::vector<Entity*> Entity::entities;

int Entity::numInfluencingLights = 8;
int Entity::numEntities = 0;

physics::ParticleForceRegistry Entity::registry;
particleGravity Entity::gravity = particleGravity(physics::Vector3(0.0f,-7.0f,0.0f));
particleDrag Entity::drag = particleDrag(0.01, 0.001);


toml::table Entity::config;
void Entity::loadEntities()
{
    config = toml::parse_file("assets/configs/entity.toml");

    auto config_entities = config["entity"].as_array();
    // int size = config_entities->size();
    
    for(int i=0; i < config_entities->size(); ++i)
    {
        auto config_entity = (*config_entities)[i].as_table();
        std::string path = (*config_entity)["path"].value_or("not found");
        std::cout<<"path of entity: "<<path<<std::endl;

        // std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(path);
        // Entity *newEntity = new Entity(path);
        entities.emplace_back(std::make_unique<Entity>(path));
    }

}
void Entity::saveConfig()
{
    auto config_entities = config["entity"].as_array();

    for(int i=0; i<entities.size(); ++i)
    {
        auto& entity = entities[i];
        auto config_entity = (*config_entities)[i].as_table();

        //update values in file

        std::cout<<"Saving Model"<<entity->name<<std::endl;
        //scale
        config_entity->insert_or_assign("scale", entity->scale);
        std::cout<<"Saved scale:"<<entity->scale<<std::endl;

        //position
        auto position_table = (*config_entity)["position"].as_table();
        position_table->insert_or_assign("x", entity->particle.position.x);
        position_table->insert_or_assign("y", entity->particle.position.y);
        position_table->insert_or_assign("z", entity->particle.position.z);
        std::cout<<"Saved position:"<<entity->particle.position.x<<" "<<entity->particle.position.y<<" "<<entity->particle.position.z<<std::endl;

        //rotation
        auto rotation_table = (*config_entity)["rotation"].as_table();
        rotation_table->insert_or_assign("x", entity->rotationMap["x"]);
        rotation_table->insert_or_assign("y", entity->rotationMap["y"]);
        rotation_table->insert_or_assign("z", entity->rotationMap["z"]);
        std::cout<<"Saved rotation:"<<entity->rotationMap["x"]<<" "<<entity->rotationMap["y"]<<" "<<entity->rotationMap["z"]<<std::endl;
    }
    std::ofstream file("assets/configs/entity.toml");
    file << config;
}

Entity::Entity(std::string modelPath):
    ourModel(Model(modelPath) )
{
    //general attributes

    ID = numEntities;
    auto config_entities = config["entity"].as_array();
    config_entity = (*config_entities)[ID].as_table();
    ++numEntities;

    int pos = modelPath.find(".");
    name = modelPath.substr(0,pos);


    this->scale = (*config_entity)["scale"].value_or(1.0f);

    rotationMap["x"] = (*config_entity)["rotation"]["x"].value_or(0.0f);
    rotationMap["y"] = (*config_entity)["rotation"]["y"].value_or(0.0f);
    rotationMap["z"] = (*config_entity)["rotation"]["z"].value_or(0.0f);
    rotation = glm::vec3(rotationMap["x"], rotationMap["y"], rotationMap["z"]);

    particle.setMass(5.0f);
    particle.position = physics::Vector3(
            (*config_entity)["position"]["x"].value_or(0.0f),
            (*config_entity)["position"]["y"].value_or(0.0f),
            (*config_entity)["position"]["z"].value_or(0.0f)
            );
    this->setPosition(glm::vec3(particle.position.x, particle.position.y, particle.position.z));
    


    //Finally push back to static vector
    registry.add(&this->particle, &gravity);
    registry.add(&this->particle, &drag);
    // entities.push_back(this); 
}

float Entity::distanceEntityAndLight(physics::Vector3 entityPos, glm::vec3 lightPos)
{
    return  (entityPos.x - lightPos.x) * (entityPos.x - lightPos.x)
                        + 
            (entityPos.y - lightPos.y) * (entityPos.y - lightPos.y)
                        + 
            (entityPos.z - lightPos.z) * (entityPos.z - entityPos.z) ;
}



void Entity::sortNearestLights(std::vector<PointLight> &PointLights)
{
    std::sort(PointLights.begin(), PointLights.end(), [this](PointLight &pre, PointLight &post){
            return distanceEntityAndLight(particle.position, pre.position) < distanceEntityAndLight(particle.position, post.position);
    });
    nearestLights.clear();
    for(int i=0; i<numInfluencingLights; ++i)
    {
        if(i<PointLights.size()) this->nearestLights.push_back(PointLights[i]);
        else break;
    }
}

void Entity::setPosition(glm::vec3 newPosition)
{
    // setting the position finally
    particle.position = physics::Vector3(newPosition.x, newPosition.y, newPosition.z);

    //Call sorting function here
    sortNearestLights(PointLights);
}
glm::vec3 Entity::getPosition()
{
    return glm::vec3(particle.position.x , particle.position.y, particle.position.z);
}

void Entity::setScale(float scale)
{
    this->scale = scale;
}
float Entity::getScale()
{
    return scale;
}

void Entity::setRotation(glm::vec3 axis, float angle)
{
    // setting the rotation finally
    if(axis.x == 1.0f) rotationMap["x"] = angle;
    if(axis.y == 1.0f) rotationMap["y"] = angle;    
    if(axis.z == 1.0f) rotationMap["z"] = angle;    

    rotation = glm::vec3(rotationMap["x"], rotationMap["y"], rotationMap["z"]);
}

glm::vec3 Entity::getRotation()
{
    return rotation; 
}
    
void Entity::update(float deltaTime)
{
    // particle.integrate(deltaTime);
    if(particle.position.y <= 1.0f)
    {
        // particle.velocity.y = 0.0f;
    }




    shader.use();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(particle.position.x, particle.position.y, particle.position.z));
    for(auto it : rotationMap)
    {
        if(it.first == "x") modelMatrix = glm::rotate(modelMatrix, glm::radians(it.second), glm::vec3(1.0f,0.0f,0.0f));
        if(it.first == "y") modelMatrix = glm::rotate(modelMatrix, glm::radians(it.second), glm::vec3(0.0f,1.0f,0.0f));
        if(it.first == "z") modelMatrix = glm::rotate(modelMatrix, glm::radians(it.second), glm::vec3(0.0f,0.0f,1.0f));
    }
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrix) ) ) );
    shader.setMat4("model", modelMatrix);
    shader.setMat4("transform", generalInfo.projectionMatrix * generalInfo.viewMatrix * modelMatrix);
    


    shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.setVec3("dirLight.ambient", glm::vec3(0.1f));
    shader.setVec3("dirLight.diffuse", glm::vec3(0.1f));
    shader.setVec3("dirLight.specular", glm::vec3(0.2f));
    shader.setInt("numInfluencingLights", numInfluencingLights);

    int i=0;
    // std::cout<<"global vector lights size: "<<PointLights.size()<<std::endl;
    // std::cout<<"nearest lights size: "<<nearestLights.size()<<std::endl;
    for(auto it = nearestLights.begin(); it!= nearestLights.end(); ++it)
    {
        // std::cout<<"x: "<<it->position.x<<" z: "<<it->position.z<<std::endl;     
        shader.setVec3("pointLights["+ std::to_string(i)+ "].position", it->position);
        shader.setVec3("pointLights["+ std::to_string(i)+ "].ambient", it->ambient);
        // std::cout<<"r: "<<it->ambient.x<<"y: "<<it->ambient.y<<" z: "<<it->ambient.z<<std::endl;     
        shader.setVec3("pointLights["+ std::to_string(i)+ "].diffuse", it->diffuse);
        // std::cout<<"r: "<<it->diffuse.x<<"y: "<<it->diffuse.y<<" z: "<<it->diffuse.z<<std::endl;     
        shader.setVec3("pointLights["+ std::to_string(i)+ "].specular", it->specular);
        // std::cout<<"r: "<<it->specular.x<<"y: "<<it->specular.y<<" z: "<<it->specular.z<<std::endl;     
        shader.setFloat("pointLights["+ std::to_string(i)+ "].constant", 1.0f);
        shader.setFloat("pointLights["+ std::to_string(i)+ "].linear", 0.09f);
        shader.setFloat("pointLights["+ std::to_string(i)+ "].quadratic", 0.032f);
        renderCube(0.2f, it->diffuse, it->position, shader);
        ++i; 
    }
    
    shader.setInt("numLights",i);
    shader.setVec3("viewPos", generalInfo.camera.Position);

    shader.setFloat("material.shininess", 128.0f);
    ourModel.Draw(shader);

}


void Entity::drawImGuiMenu()
{
    if(!generalInfo.camera.shouldMove) 
    {

        ImGui::Begin("Entities");

        if(ImGui::Button("Save"))
        {
            saveConfig();
            std::cout<<"Saved succesfully"<<std::endl;
        }
        for(int i=0; i<entities.size(); ++i)
        {
            ImGui::PushID(i);
            if(ImGui::CollapsingHeader(entities[i]->name.c_str()))
            {
               //setting position
               glm::vec3 objectPosArr = entities[i]->getPosition();
               ImGui::SliderFloat3("Object Position", &objectPosArr.x, -50.0f, 50.0f);
               entities[i]->setPosition(objectPosArr);

               //setting rotation
               glm::vec3 objectRotArr = entities[i]->getRotation();
               ImGui::SliderFloat3("Object Rotation", &objectRotArr.x, -180.0f, 180.0f);
               entities[i]->setRotation(glm::vec3(1.0f,0.0f,0.0f), objectRotArr.x);
               entities[i]->setRotation(glm::vec3(0.0f,1.0f,0.0f), objectRotArr.y);
               entities[i]->setRotation(glm::vec3(0.0f,0.0f,1.0f), objectRotArr.z);

               //setting scale
               float scale = entities[i]->getScale();
               ImGui::SliderFloat("Object Scale", &scale, 0.0f,10.0f);
               entities[i]->setScale(scale);

            }
            ImGui::PopID();
            ImGui::Separator();
        }
        ImGui::End();

    }

}
