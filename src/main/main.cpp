#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <iostream>
#include <memory>

#include "global/global.h"
#include "global/globalDisplay.h"
#include "global/globalCamera.h"
#include "entity/entity.h"

#include "global/globalStatus.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "lighting/lighting.h"
#include "render/framebuffers.h"
#include "render/cubemap.h"





void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void processMouse(GLFWwindow *window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);


int main()
{
        glfwInit();     
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        //initialization before glad is initialized
        global::preInit();

        int WIDTH = global::Display::getWidth();
        int HEIGHT = global::Display::getheight();
        GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "curloz engine", NULL, NULL);
        if(!window)
        {
                std::cout<<"Could not initialize window"<<std::endl;
                return 1;
        }
        glfwMakeContextCurrent(window);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
                std::cout<<"Could not initialize glad"<<std::endl;
                glfwTerminate();
                return 1;
        }
        //initialization after glad is initialized
        global::postInit();


        screenFramebuffer::Init();
        glViewport(0, 0, WIDTH, HEIGHT);

        renderFramebuffer::Init(WIDTH, HEIGHT);
        Lighting::init();
        Entity::init();
        CubeMap::init();


        if (global::Status::Profile == global::PROFILE::DEBUG_MODE)
        {
                global::Status::Mode = global::MODE::EDIT_MODE;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO &io = ImGui::GetIO();
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                ImGui::StyleColorsDark();
                if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
                {
                        std::cerr << "Could not initialize GLFW(opengl) for imgui" << std::endl;
                        return 1;
                }
                if (!ImGui_ImplOpenGL3_Init("#version 330"))
                {
                        std::cerr << "Could not initialize glsl for imgui" << std::endl;
                }
        }


        if (global::Status::Profile == global::PROFILE::RELEASE_MODE)
        {
                glfwSetCursorPosCallback(window, mouse_callback);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        glfwSetScrollCallback(window, scrollCallback);


        float deltaTime = 0;
        float timeTakenThisFrame = 0;
        float timeTakenLastFrame = 0;

        bool vsync = global::Display::VSync();
        int refreshRate = global::Display::getRefreshRate();


        //Game Loop
        while(!glfwWindowShouldClose(window))
        {
                glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer::getFramebuffer());
                // glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);

                glfwPollEvents();
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


                timeTakenThisFrame = glfwGetTime();
                deltaTime = timeTakenThisFrame - timeTakenLastFrame;
                timeTakenLastFrame = timeTakenThisFrame;
                if(vsync)
                {
                        if (deltaTime < (float)1/refreshRate)
                        {
                                deltaTime = (float)1/refreshRate;
                        }
                }


                processInput(window);
                global::update();
                global::CameraManager::camera->ProcessKeyboard(window, deltaTime);


                if (global::Status::Mode == global::MODE::EDIT_MODE)
                {
                        ImGui_ImplOpenGL3_NewFrame();
                        ImGui_ImplGlfw_NewFrame();
                        ImGui::NewFrame();

                }
                if (global::Status::Mode == global::MODE::GAME_MODE)
                {
                       processMouse(window); 
                }


                Lighting::update();
                Entity::update(deltaTime);
		            CubeMap::Draw();


                if (global::Status::Mode == global::MODE::EDIT_MODE)
                {
                        ImGui::Render();
                        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                }


                screenFramebuffer::Draw(renderFramebuffer::getScreenTexture());

                glfwSwapBuffers(window);
        }

        global::update();
        global::saveConfig();

        glfwTerminate();
        return 0;
}

void processInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
                glfwSetWindowShouldClose(window, true);
        }

        if(global::Status::Profile == global::PROFILE::DEBUG_MODE)
        {
                if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && global::CameraManager::camera->shouldMove)
                {
                        global::Status::Mode = global::MODE::EDIT_MODE;
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
                        global::CameraManager::camera->shouldMove = false;
                }
                if (glfwGetKey(window,GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window,GLFW_KEY_G) == GLFW_PRESS && !global::CameraManager::camera->shouldMove)
                {
                        global::Status::Mode = global::MODE::GAME_MODE;
                        global::CameraManager::camera->firstInput = true;
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        global::CameraManager::camera->shouldMove = true;
                }

        }

}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
        global::CameraManager::camera->ProcessMouseMovement(xPos, yPos);
}
void processMouse(GLFWwindow *window)
{
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        global::CameraManager::camera->ProcessMouseMovement(xPos, yPos);
}
void scrollCallback(GLFWwindow* window, double xPos, double yPos)
{
        global::CameraManager::camera->processScrollBack(yPos);
}
