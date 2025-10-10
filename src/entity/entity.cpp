#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include "entity/entity.h"
#include "global/globalEntity.h"
#include "json/json.hpp"
#include <unordered_map>
#include "global/globalStatus.h"
#include "imgui/imgui.h"

using json = nlohmann::json;


std::string Entity::entityConfigFile;
std::vector<std::unique_ptr<renderStatic>> Entity::p_renderStatic;

void Entity::init()
{
        entityConfigFile = global::Entity::getConfigFile();
        
        renderStatic::init();

        loadEntities();
}
void Entity::loadEntities()
{
        std::ifstream file(entityConfigFile);
        if (!file.is_open())
        {
                std::cerr << "Could not open entity config file" << std::endl;
                return ;
        }
        json data = json::parse(file);
        file.close();

        if (!data.contains("entities")) 
        {
                std::cout << "Entity config file is empty" << std::endl;
                data["entities"] = json::array();
                std::ofstream oFile(entityConfigFile);
                oFile << data.dump(4);
                oFile.close();
                return;
        }

        for (auto &entities : data["entities"])
        {
                std::string type = entities["type"];
                std::string name = entities["name"];

                glm::vec3 position;
                glm::vec3 rotation;
                float scale;
                std::string modelPath;
                if (entities.contains("position"))
                {
                        position.x = entities["position"][0];
                        position.y = entities["position"][1];
                        position.z = entities["position"][2];
                }

                if (entities.contains("rotation"))
                {
                        rotation.x = entities["rotation"][0];
                        rotation.y = entities["rotation"][1];
                        rotation.z = entities["rotation"][2];
                }

                if (entities.contains("scale"))
                {
                        scale = entities["scale"];
                }

                if (entities.contains("model"))
                {
                        modelPath = entities["model"];
                }

                if(type == "renderStatic")
                {
                        p_renderStatic.emplace_back(std::make_unique<renderStatic>(modelPath)); 
                        ++renderStatic::m_Count;

                        auto &p_currentEntity = p_renderStatic.back();
                        p_currentEntity->setName(name);
                        p_currentEntity->setPosition(position);
                        p_currentEntity->setRotation(rotation);
                        p_currentEntity->setScale(scale);

                }

        }
}

void Entity::saveConfig()
{
        std::ifstream fileOpen(entityConfigFile);
        if (!fileOpen.is_open())
        {
                std::cerr << "Could not open entity config file" << std::endl;
                return ;
        }
        json data = json::parse(fileOpen);
        fileOpen.close();

        std::unordered_map<std::string, json*> entityMap;
        for (auto &entity : data["entities"])
        {
                entityMap[entity["name"]] = &entity;
        }

        for (int i=0; i<p_renderStatic.size(); ++i)
        {
                auto it = entityMap.find(p_renderStatic[i]->getName());
                
                if (it != entityMap.end())
                {
                        json &entity = *(it->second);
                        entity["position"][0] = p_renderStatic[i]->getPosition().x;
                        entity["position"][1] = p_renderStatic[i]->getPosition().y;
                        entity["position"][2] = p_renderStatic[i]->getPosition().z;

                        entity["rotation"][0] = p_renderStatic[i]->getRotation().x;
                        entity["rotation"][1] = p_renderStatic[i]->getRotation().y;
                        entity["rotation"][2] = p_renderStatic[i]->getRotation().z;

                        entity["scale"] = p_renderStatic[i]->getScale();
                }
        }


        std::ofstream fileClose(entityConfigFile);

        fileClose << data.dump(4);
        fileClose.close();

}

void Entity::update(float deltaTime)
{
        if (global::Status::Mode == global::MODE::EDIT_MODE)
        {
                ImGui::Begin("Entity");
                ImGui::Separator();
                ImGui::Separator();
                if (ImGui::Button("Click Here to Save"))
                {
                        saveConfig();
                        std::cout << "Saved Succesfully" << std::endl;
                }
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
                ImGui::CollapsingHeader("Render Static");
                
                for (int i=0; i<p_renderStatic.size(); ++i)          
                {                                                            
                        ImGui::PushID(i);

                        p_renderStatic[i]->update(deltaTime);        
                                                                     

                        ImGui::PopID();  
                        ImGui::Separator();
                        ImGui::Separator();
                }                

               
                ImGui::End();
        
        }
        else if (global::Status::Mode == global::MODE::GAME_MODE)
        {
                for (int i=0; i<p_renderStatic.size(); ++i)
                {
                        p_renderStatic[i]->update(deltaTime);
                }
        }

}
