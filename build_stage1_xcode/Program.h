//
//  Program.h
//  Stage1
//
//  Created by Sean Bollin on 1/8/14.
//
//

#ifndef __Stage1__Program__
#define __Stage1__Program__

#include <iostream>
#include "Shader.h"
#include <vector>

namespace tdogl {
    class Program {
    public:
        Program(const std::vector<Shader>& shaders);
        ~Program();
        
        GLuint object() const;
        
        GLint attrib(const GLchar* attribName) const;
        
        GLint uniform(const GLchar* uniformName) const;
        
    private:
        GLuint _object;
        
        Program(const Program&);
        const Program& operator=(const Program&);
    };
}

#endif /* defined(__Stage1__Program__) */
