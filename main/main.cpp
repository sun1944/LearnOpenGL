#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float camerX, camerY, camerZ;
float cubeLocX, cubeLocY, cubeLocZ;

//存储着色器程序ID
GLuint renderingProgram;
//顶点数组对象数组
GLuint vao[numVAOs];
//顶点缓冲对象数组
GLuint vbo[numVBOs];
//
GLuint mvLoc,projLoc;
int width,height;
float aspect;
//透视矩阵，视图矩阵，模型矩阵，MV矩阵
glm::mat4 pMat, vMat,mMat,mvMat;
void setupVertices(void)
{
    float vertexPositions[108] = {
        -1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,-1.0f,
        1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,
        1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,
        -1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,
        -1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,
    };
    glGenVertexArrays(1,vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs,vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertexPositions),vertexPositions,GL_STATIC_DRAW);
    
}



void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("main/glsl/vertShader.glsl","main/glsl/fragShader.glsl");
    camerX = 0.0f; camerY = 0.0f; camerZ = 8.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    setupVertices();
}

void display(GLFWwindow* window,double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    //获取MV矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgram,"mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram,"proj_matrix");

    //构建透视矩阵
    glfwGetFramebufferSize(window,&width,&height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f,aspect,0.1f,1000.0f);

    //构建视图矩阵，模型矩阵和MV矩阵
    vMat = glm::translate(glm::mat4(1.0f),glm::vec3(-camerX,-camerY,-camerZ));
    glm::mat4 tMat;  // 平移矩阵
    glm::mat4 rMat;  // 旋转矩阵
    tMat = glm::translate(glm::mat4(1.0f),glm::vec3(sin(0.35f*currentTime)*2.0f,cos(0.52f*currentTime)*2.0f,sin(0.7f*currentTime)*2.0f));
    rMat = glm::rotate(glm::mat4(1.0f),1.75f*(float)currentTime,glm::vec3(0.0f,1.0f,0.0f));
    rMat = glm::rotate(rMat,1.75f*(float)currentTime,glm::vec3(1.0f,0.0f,0.0f));
    rMat = glm::rotate(rMat,1.75f*(float)currentTime,glm::vec3(0.0f,0.0f,1.0f));
    mMat = rMat * tMat;
    
    //mMat = glm::translate(glm::mat4(1.0f),glm::vec3(cubeLocX,cubeLocY,cubeLocZ));
    mvMat = vMat * mMat;
    //将透视矩阵和模型矩阵复制给相应的统一变量
    glUniformMatrix4fv(mvLoc,1,GL_FALSE,glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(pMat));
    //将VBO关联给顶点着色器中的相应顶点属性
    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    //调整OpenGL设置，绘制模型
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES,0,36);
    
}

int main(void)
{
    if(!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window=glfwCreateWindow(600,600,"我的OpenGL项目",NULL,NULL);
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK)
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    init(window);
    while(!glfwWindowShouldClose(window))
    {
        display(window,glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

