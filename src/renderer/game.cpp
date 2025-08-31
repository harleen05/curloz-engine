//Basic windowing stuff
#include <glad/glad.h>
#include <GLFW/glfw3.h> //always glfw after glad

//load general stuff
#include "global/general.h"
info generalInfo;

/* Custom Files */

//rendering
#include "renderer/lighting.h"
#include "renderer/renderPlatform.h"


//glm
#include <glm/glm.hpp>


//textures
/*stb write implementation(texture), one more implementation is present in stb_image.cpp*/
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "texture/stb_image.h"


//entity
#include "entity/entity.h"


//loaders
#define TINYGLTF_IMPLEMENTATION


//physics
#include "physics/particle.h"
#include "physics/forceGen.h"
#include "physics/springs.h"


//audio
#define DR_WAV_IMPLEMENTATION
#include "audio/audioMaster.h"
#include "audio/source.h"
#include "audio/dr_wav.h"


//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



//Standard libraries
#include <iostream>
#include <string>


void processInput(GLFWwindow* window);
void processMouse(GLFWwindow *window, Camera &camera);
void ImGuiEvent();


int main()
{
    //initialize glfw
    glfwInit();

    //initialize glfw for opengl
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);

    //create our window
    GLFWwindow* window = glfwCreateWindow(generalInfo.width,generalInfo.height,"lighting window",NULL,NULL);
    if(window == NULL)
    {
        std::cout<<"GLFW window could not be created";
    }

    glfwMakeContextCurrent(window);

    //initalize glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Could not initialize glad\n";
        glfwTerminate();
        return -1;
    }

    //set window and viewpost attributes
    glfwSetWindowSize(window,generalInfo.width, generalInfo.height);
    glViewport(0,0,generalInfo.width, generalInfo.height);
    glEnable(GL_DEPTH_TEST);

    renderCube::bookInAdvance();


    //Working in ImGUI stuff
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;  
    ImGui::StyleColorsDark();
    if(!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::cout<<"Could not initialize ImplGlfw_InitForOpenGL"<<std::endl;
        return -1;
    }
    if(!ImGui_ImplOpenGL3_Init("#version 330"))
    {
        std::cout<<"Could not initialize ImplOpenGL3_Init"<<std::endl;
        return -1;
    }
   

    renderPlatform::init();




    DirectionalLight dirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.01f), glm::vec3(0.1f), glm::vec3(0.01f));
    glm::vec3 ptLightPosition [3] =
    {
        glm::vec3( 0.0f, 5.0f, 5.0f),
        glm::vec3( 7.0f, 5.0f,-7.0f),
        glm::vec3(-7.0f, 5.0f,-7.0f)
    };
    PointLight ptLight1 = PointLight(ptLightPosition[0],glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(1.0f,1.0f,1.0f));
    // PointLight ptLight2 = PointLight(ptLightPosition[1],glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f,0.8f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    // PointLight ptLight3 = PointLight(ptLightPosition[2],glm::vec3(0.1f,0.0f,0.0f), glm::vec3(0.8f,0.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f));
    Entity::PointLights.push_back(ptLight1);
    // Entity::PointLights.push_back(ptLight2);
    // Entity::PointLights.push_back(ptLight3);
    Entity::loadEntities();
    Entity::camera = &generalInfo.camera;
    Entity::viewMatrix = generalInfo.viewMatrix;
    Entity::projectionMatrix = generalInfo.projectionMatrix;
    Entity::directionLight = dirLight;

    // Entity statue("assets/models/statue-chiriqui-3d-model/scene.gltf");
    Particle bungeeParticle(physics::Vector3(0.0f,20.0f,0.0f));
    particleBungee bungee(&bungeeParticle, 2.0f, 10.0f);
    // particleAnchoredSpring spring(physics::Vector3(0.0f,0.0f,0.0f), 10.0f, 1.0f);
    physics::ParticleForceRegistry registry;
    // registry.add(&statue.particle, &bungee);
    // Entity mic("assets/models/microphone/microphone_gxl_066_bafhcteks.glb");


    
    float deltaTime = 0.0f;
    float timeTakenThisFrame = 0.0f;
    float timeTakenLastFrame = 0.0f;

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);

    glfwSetWindowUserPointer(window, &generalInfo.camera);
    // glm::vec3 lightPos = glm::vec3(0.0f,1.0f,-5.0f);
    // float lightPosArr[3] = {lightPos.x, lightPos.y, lightPos.z};
    // glm::vec3 modelPos = glm::vec3(0.0f,0.0f,0.0f);
    // float modelPosArr[3] = {modelPos.x, modelPos.y, modelPos.z};

    
    AudioMaster::init();
    AudioMaster::setListenerData();
    ALuint buffer = AudioMaster::loadSound("assets/audio/bounce.wav");
    Source source;
    source.setPosition(glm::vec3(0.0f,0.0f,0.0f));
    source.setPitch(1.0f);
    source.setVolume(2.0f);
    // source.setLooping(true);
    source.setVelocity(glm::vec3(0.0f));
    std::cout<<"setted our source 1"<<std::endl;

    // source.play(buffer);

     
    while(!glfwWindowShouldClose(window))
    {
 
        AudioMaster::update();
        if(glfwGetKey(window,GLFW_KEY_P) == GLFW_PRESS)
        {
            if(!source.isPlaying())
            {
                source.play(buffer);
            }
        }


        // std::cout<<"DEBUG camera z:"<<generalInfo.camera.Position.z<<std::endl;


        glfwPollEvents();
        timeTakenThisFrame = glfwGetTime();
        deltaTime = timeTakenThisFrame - timeTakenLastFrame;
        timeTakenLastFrame = timeTakenThisFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f, 1.0f);

        processInput(window);
        generalInfo.camera.ProcessKeyboard(window,deltaTime);
        if(generalInfo.camera.shouldMove) processMouse(window, generalInfo.camera);
        generalInfo.update();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderPlatform::Draw();
        


   
        Entity::drawImGuiMenu();
        registry.updateForce(deltaTime);
        Entity::registry.updateForce(deltaTime);
        for(auto &it : Entity::entities)
        {
            it->update(deltaTime);
        }

        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        // std::cout<<"FPS: "<<1/deltaTime<<std::endl;
    }

    Entity::saveConfig();
    source.deleteSource();
    AudioMaster::cleanUp();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,true);
    }
    if(glfwGetKey(window,GLFW_KEY_TAB) == GLFW_PRESS && camera->shouldMove)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        camera->shouldMove = false;
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_G) == GLFW_PRESS && !camera->shouldMove)
    {
        camera->firstInput = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera->shouldMove = true;
    }

}

void processMouse(GLFWwindow *window, Camera &camera)
{
    double MOUSE_POS_X, MOUSE_POS_Y;  
    glfwGetCursorPos(window, &MOUSE_POS_X, &MOUSE_POS_Y);
    camera.ProcessMouseMovement(MOUSE_POS_X, MOUSE_POS_Y);
}
