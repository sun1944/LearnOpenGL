#include "Utils.h"


using namespace std;

namespace Utils
{
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

        string vertShaderStr = readShaderSource("main/glsl/vertShader.glsl");
        string fragShaderStr = readShaderSource("main/glsl/fragShader.glsl");
    
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
}
