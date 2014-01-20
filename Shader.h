//
//  Shader.h
//  Stage1
//
//  Created by Sean Bollin on 1/8/14.
//
//

#ifndef __Stage1__Shader__
#define __Stage1__Shader__

#include <iostream>
#include <string>
#include "gl2.h"
#include "s3eFile.h"

namespace tdogl {
    class Shader {
    public:
        static Shader shaderFromFile(const std::string& filePath, GLenum shaderType);
        
        Shader(const std::string& shaderCode, GLenum shaderType);
        
        GLuint object() const;
        
        Shader(const Shader& other);
        Shader& operator =(const Shader& other);
        ~Shader();
    
    private:
        GLuint _object;
        unsigned* _refCount;
        
        void _retain();
        void _release();
        
    };
}

#endif /* defined(__Stage1__Shader__) */
