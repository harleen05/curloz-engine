#include "renderer/renderPlatform.h"


Shader renderPlatform::platformShader;
GLuint renderPlatform::VAO;
GLuint renderPlatform::platformDiffuse;
GLuint renderPlatform::platformSpecular;


void renderPlatform::init()
{
    platformShader = Shader("assets/shaders/platform.vert","assets/shaders/platform.frag");

    float verticesPlatform[] = 
    {
        -1.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,
        -1.0f,0.0f, 1.0f, 0.0f,1.0f,0.0f, 0.0f,100.0f,
         1.0f,0.0f, 1.0f, 0.0f,1.0f,0.0f, 100.0f,100.0f,
         1.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f, 100.0f,0.0f
    };
    unsigned int indicesPlatform [] = 
    {
        0,1,2,
        2,3,0
    };

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verticesPlatform),verticesPlatform,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicesPlatform),indicesPlatform,GL_STATIC_DRAW);

    
    platformShader.use();
    platformDiffuse = loadTextureFromFile("assets/textures/sexyAhhGround/GroundDirtWeedsPatchy004_COL_1K.jpg",false);
    platformShader.setInt("material.diffuse0",0);
    platformSpecular = loadTextureFromFile("assets/textures/sexyAhhGround/GroundDirtWeedsPatchy004_COL_1K.jpg",false);
    platformShader.setInt("material.specular0",1);

    glBindVertexArray(0);
}

void renderPlatform::Draw()
{
    platformShader.use();
    platformShader.setVec3("lighting.position",glm::vec3(0.0f,25.0f,0.0f));
    platformShader.setVec3("lighting.ambient",glm::vec3(1.0f));
    platformShader.setVec3("lighting.diffuse",glm::vec3(1.0f));
    platformShader.setVec3("lighting.specular",glm::vec3(1.0f));
    platformShader.setFloat("lighting.constant",1);
    platformShader.setFloat("lighting.linear",0.009f);
    platformShader.setFloat("lighting.quadratic",0.0032f);
    platformShader.setVec3("viewPos",generalInfo.camera.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, platformDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, platformSpecular);
    platformShader.setInt("material.diffuse0",0);
    platformShader.setInt("material.specular0",1);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(100.0f));
    platformShader.setMat4("model", model);
    platformShader.setMat4("transform", generalInfo.projectionMatrix * generalInfo.viewMatrix * model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

