#include <fstream>
#include <iostream>
#include "lighting/lighting.h"
#include "json/json.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include "global/globalStatus.h"
#include "imgui/imgui.h"

using json = nlohmann::json;

lightingDirectional Lighting::global;
std::vector<std::unique_ptr<LightingPoint>> Lighting::m_point_lights;
nlohmann::json Lighting::config;
std::string Lighting::configFile;

void Lighting::init()
{
        configFile = "assets/config/lights.json";
        std::ifstream file (configFile);        
        if (!file.is_open())
        {
                std::cerr << "Could not open lights config file" << std::endl;
                return ;
        }
        json data = json::parse(file);
        file.close();

        if (!data.contains("lights"))
        {
                std::cout << "light config file is empty" << std::endl;
                data["lights"] = json::array();
                std::ofstream oFile(configFile);
                oFile << data.dump(4);
                oFile.close();
                return;
        }

        for (auto &lights : data["lights"])
        {
                std::string type = lights["type"];

                if (type == "directional")
                {
                        glm::vec3 ambient = glm::vec3(1.0f);
                        glm::vec3 diffuse = glm::vec3(1.0f);
                        glm::vec3 specular = glm::vec3(1.0f);
                        glm::vec3 direction = glm::vec3(1.0f);
                        
                        if (lights.contains("ambient"))
                        {
                                ambient.x = lights["ambient"][0];
                                ambient.y = lights["ambient"][1];
                                ambient.z = lights["ambient"][2];
                        }
                        if (lights.contains("diffuse"))
                        {
                                diffuse.x = lights["diffuse"][0];
                                diffuse.y = lights["diffuse"][1];
                                diffuse.z = lights["diffuse"][2];
                        }
                        if (lights.contains("specular"))
                        {
                                specular.x = lights["specular"][0];
                                specular.y = lights["specular"][1];
                                specular.z = lights["specular"][2];
                        }

                        if (lights.contains("direction"))
                        {
                               direction.x = lights["direction"][0]; 
                               direction.y = lights["direction"][1]; 
                               direction.z = lights["direction"][2]; 
                        }

                        std::cout << "Found direction of global to be: " << direction[0]<<","<<direction[1]<<","<<direction[2] << std::endl;

                        //initialize the main static global light
                        glm::vec3 lightComponent[3] = {ambient, diffuse, specular};
                        global = lightingDirectional(&lightComponent[0], direction);
                        continue;
                }

                else if(type == "point")
                {
                        std::string name = "";
                        glm::vec3 ambient = glm::vec3(1.0f);
                        glm::vec3 diffuse = glm::vec3(1.0f);
                        glm::vec3 specular = glm::vec3(1.0f);
                        glm::vec3 position = glm::vec3(1.0f);
                        
                        if(!lights.contains("name"))
                        {
                                std::cerr << "point light does not have a name" << std::endl;
                                return;
                        }
                        name = lights["name"];

                        if (!lights.contains("ambient"))
                        {
                                std::cerr << "point light does not have ambient" << std::endl;
                                return;
                        }
                        ambient.x = lights["ambient"][0];
                        ambient.y = lights["ambient"][1];
                        ambient.z = lights["ambient"][2];

                        if (!lights.contains("diffuse"))
                        {
                                std::cerr << "point light does not have diffuse" << std::endl;
                                return;

                        }
                        diffuse.x = lights["diffuse"][0];
                        diffuse.y = lights["diffuse"][1];
                        diffuse.z = lights["diffuse"][2];

                        if (!lights.contains("specular"))
                        {
                                std::cerr << "point light does not have specular" << std::endl;
                                return;
                        }
                        specular.x = lights["specular"][0];
                        specular.y = lights["specular"][1];
                        specular.z = lights["specular"][2];


                        if (!lights.contains("position"))
                        {
                                std::cerr << "point light does not have position(wth!!?)" << std::endl;
                                return;
                        }
                        position.x = lights["position"][0]; 
                        position.y = lights["position"][1]; 
                        position.z = lights["position"][2]; 


                        //initialize the point light
                        glm::vec3 lightComponent[3] = {ambient, diffuse, specular};
                        m_point_lights.emplace_back(std::make_unique<LightingPoint>(name, lightComponent, position));
                        continue;

                }

        }

}

void Lighting::saveConfig()
{
        std::ifstream file(configFile);
        if(!file.is_open())
        {
                std::cerr << "Could not load lights config file " << std::endl;
                return ;
        }
        json data = json::parse(file);
        file.close();

        if(!data.contains("lights"))
        {
                std::cout << "Lights config file is empty" << std::endl;
                data["lights"] = json::array();
                std::ofstream oFile(configFile);
        }

        std::unordered_map<std::string, json*> lightsHashMap;
        for (auto &lights : data["lights"])
        {
                lightsHashMap[lights["name"]] = &lights;
        }

        // first checking for global light
        auto globalLight = lightsHashMap.find("global");
        if(globalLight != lightsHashMap.end())
        {
                json &light = *(globalLight->second);
                 
                light["direction"][0] = global.getDirection().x;
                light["direction"][1] = global.getDirection().y;
                light["direction"][2] = global.getDirection().z;

                light["ambient"][0] = global.getColor()[0].x;
                light["ambient"][1] = global.getColor()[0].y;
                light["ambient"][2] = global.getColor()[0].z;

                light["diffuse"][0] = global.getColor()[1].x;
                light["diffuse"][1] = global.getColor()[1].y;
                light["diffuse"][2] = global.getColor()[1].z;

                light["specular"][0] = global.getColor()[2].x;
                light["specular"][1] = global.getColor()[2].y;
                light["specular"][2] = global.getColor()[2].z;

        }

        //Now checking for point lights
        std::cout << "remove me, sizef of point light vector is: " << m_point_lights.size() << std::endl;
        for (int i=0; i<m_point_lights.size(); ++i)
        {
                auto pointLight = lightsHashMap.find(m_point_lights[i]->getName());

                //existing lights
                if (pointLight != lightsHashMap.end())
                {
                        json &light = *(pointLight->second);

                        std::cout << "remove me, position of point light is ->" << light["position"][0]  << std::endl;

                        light["position"][0] = m_point_lights[i]->getPosition().x;
                        light["position"][1] = m_point_lights[i]->getPosition().y;
                        light["position"][2] = m_point_lights[i]->getPosition().z;

                        light["ambient"][0] = m_point_lights[i]->getColor()[0].x;
                        light["ambient"][1] = m_point_lights[i]->getColor()[0].y;
                        light["ambient"][2] = m_point_lights[i]->getColor()[0].z;

                        light["diffuse"][0] = m_point_lights[i]->getColor()[1].x;
                        light["diffuse"][1] = m_point_lights[i]->getColor()[1].y;
                        light["diffuse"][2] = m_point_lights[i]->getColor()[1].z;

                        light["specular"][0] = m_point_lights[i]->getColor()[2].x;
                        light["specular"][1] = m_point_lights[i]->getColor()[2].y;
                        light["specular"][2] = m_point_lights[i]->getColor()[2].z;

                }

        }


        
        std::ofstream fileWrite(configFile);

        fileWrite << data.dump(4);
        fileWrite.close();

}

void Lighting::update()
{
        if (global::Status::Mode == global::MODE::EDIT_MODE) 
        {
                ImGui::Begin("Lights");        
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
                if (ImGui::Button("Save"))
                {
                        saveConfig();
                        std::cout << "Saved config" << std::endl;
                }
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
                ImGui::Separator();
        
                if (ImGui::CollapsingHeader("Global light"))
                {                
                        ImGui::Separator();
                        ImGui::Separator();

                        global.Edit();
                }

                if (ImGui::CollapsingHeader("Point lights"))
                {
                        ImGui::Separator();
                        ImGui::Separator();
                        for(int i=0; i<m_point_lights.size(); ++i)
                        {
                                ImGui::PushID(i);

                                m_point_lights[i]->Edit();

                                ImGui::PopID();
                        }
                }
                ImGui::End();

        }
}



glm::vec3 Lighting::getGlobalLight_Direction()
{
        return Lighting::global.getDirection();
}
glm::vec3 *Lighting::getGlobalLight_Color()
{
        return Lighting::global.getColor();
}

int Lighting::getNumPointLights()
{
        return m_point_lights.size();
}

