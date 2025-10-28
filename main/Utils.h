#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

namespace Utils
{
    void printShaderLog(GLuint shader);

    void printProgramLog(int prog);

    bool CheckOpenGLError();

    string readShaderSource(const char *filePath);

    GLuint createShaderProgram(string vertFilePath,string fragFilePath);
}
