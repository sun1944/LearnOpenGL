#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

float x = 0.0f;
float inc = 0.01f;

void printShaderLog(GLuint shader)
{
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&len);
    if(len > 0)
    {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader,len,&chWrittn,log);
        cout<<"Shader Info Log: "<<log<<endl;
        free(log);
    }
}

void printProgramLog(int prog)
{
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog,GL_INFO_LOG_LENGTH,&len);
    if(len > 0)
    {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog,len,&chWrittn,log);
        cout<<"Program Info Log: "<<log<<endl;
        free(log);
    }
}

bool CheckOpenGLError()
{
    bool foundError = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR)
    {
        cout<<"glError: "<<glErr<<endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}



string readShaderSource(const char *filePath)
{
    string content;
    ifstream fileStream(filePath, ios::in);
    if (!fileStream.is_open()) {
        cout << "Could not read file " << filePath << ". File does not exist." << endl;
        return "";
    }
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}


GLuint createShaderProgram()
{

    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    
    
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint  fShader = glCreateShader(GL_FRAGMENT_SHADER);

    string vertShaderStr = readShaderSource("c/glsl/vertShader.glsl");
    string fragShaderStr = readShaderSource("c/glsl/fragShader.glsl");
    
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader,1,&vertShaderSrc,NULL);
    glShaderSource(fShader,1,&fragShaderSrc,NULL);
    
    glCompileShader(vShader);
    CheckOpenGLError();
    glGetShaderiv(vShader,GL_COMPILE_STATUS,&vertCompiled);
    if(vertCompiled != 1)
    {
        cout<<"vertex compilation failed" << endl;
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    CheckOpenGLError();
    glGetShaderiv(fShader,GL_COMPILE_STATUS,&fragCompiled);
    if(fragCompiled != 1)
    {
        cout<<"fragment compilation failed" << endl;
        printShaderLog(fShader);
    }
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram,vShader);
    glAttachShader(vfProgram,fShader);
    //补货链接着色器时的错误
    glLinkProgram(vfProgram);
    CheckOpenGLError();
    glGetProgramiv(vfProgram,GL_LINK_STATUS,&linked);
    if(linked != 1)
    {
        cout<<"linking failed" << endl;
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

void init(GLFWwindow* window)
{
    renderingProgram = createShaderProgram();
    glGenVertexArrays(numVAOs,vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window,double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(renderingProgram);

    x+=inc;
    if(x>1.0f) inc = -0.01f;
    if(x<-1.0f) inc = 0.01f;
    GLuint offsetLoc = glGetUniformLocation(renderingProgram,"offset");
    glProgramUniform1f(renderingProgram,offsetLoc,x);

    
    glDrawArrays(GL_TRIANGLES,0,3);
}

int main(void)
{
    if(!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow* window=glfwCreateWindow(600,600,"Chapter2 - program1",NULL,NULL);
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

