//
//  Shader.cpp
//  Stage1
//
//  Created by Sean Bollin on 1/8/14.
//
//

#include "Shader.h"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include "s3eMemory.h"

using namespace tdogl;

Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
_object(0),
_refCount(NULL)
{
    //create the shader object
    _object = glCreateShader(shaderType);
    if(_object == 0)
        throw std::runtime_error("glCreateShader failed");
    
    //set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(_object, 1, (const GLchar**)&code, NULL);
    
    //compile
    glCompileShader(_object);
    
    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(_object); _object = 0;
        throw std::runtime_error(msg);
    }
    
    _refCount = new unsigned;
    *_refCount = 1;
}

Shader::Shader(const Shader& other) :
_object(other._object),
_refCount(other._refCount)
{
    _retain();
}

Shader::~Shader() {
    //_refCount will be NULL if constructor failed and threw an exception
    if(_refCount) _release();
}

GLuint Shader::object() const {
    return _object;
}

Shader& Shader::operator = (const Shader& other) {
    _release();
    _object = other._object;
    _refCount = other._refCount;
    _retain();
    return *this;
}

Shader Shader::shaderFromFile(const std::string& filePath, GLenum shaderType) {
    s3eFile* file = s3eFileOpen(filePath.c_str(), "r");
    int32 fileSize = s3eFileGetSize(file);
    char dataToBeRead[1024];
    
    s3eFileRead(&dataToBeRead, fileSize, 1, file);
    
    std::string fileString(dataToBeRead);
    std::cout << fileString + "\n";
    std::string shaderString(
        "#version 150\n"
        "in vec3 vert;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(vert, 1);\n"
        "}\n"
    );
    
    Shader shader(fileString, shaderType);
    return shader;
}

void Shader::_retain() {
    assert(_refCount);
    *_refCount += 1;
}

void Shader::_release() {
    assert(_refCount && *_refCount > 0);
    *_refCount -= 1;
    if(*_refCount == 0){
        glDeleteShader(_object); _object = 0;
        delete _refCount; _refCount = NULL;
    }
}