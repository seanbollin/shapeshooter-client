/*
 * This file is part of the Marmalade SDK Code Samples.
 *
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This source code is intended only as a supplement to the Marmalade SDK.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "IwGL.h"
#include "s3e.h"
#include <iostream>
#include "input.h"

using namespace std;

static int eglInit()
{
    if (!IwGLInit() )
    {
        s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, "eglInit failed");
        return 1;
    }
    return 0;
}

const char* vShaderStr =
    "attribute vec4 vPosition;  \n"
    "void main()                \n"
    "{                          \n"
    "gl_Position = vPosition;   \n"
    "}                          \n";

const char* fShaderStr =
    "precision mediump float;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;
    
    // Create the shader object
    shader = glCreateShader(type);
    
    if (shader == 0)
        return 0;
    
    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);
    
    // Compile the shader
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            
            GLsizei info_length = 0;
            glGetShaderInfoLog(shader, infoLen, &info_length, infoLog);
            printf("Error compiling shader:\n%s\n", infoLog);
            
            free(infoLog);
        }
        
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLfloat vVertices[] = {0.0f, 0.1f, 0.0f,
                       -0.1f, -0.1f, 0.0f,
                       0.1f, -0.1f, 0.0f};

GLfloat hLinesVertices[40];
GLfloat vLinesVertices[40];

void setupGrid() {
    GLfloat x1 = -1.0f;
    GLfloat y = 0.8f;
    GLfloat x2 = 1.0f;
    
    for (int i = 0; i < 40; i++) {
        hLinesVertices[i] = x1;
        hLinesVertices[++i] = y;
        hLinesVertices[++i] = x2;
        hLinesVertices[++i] = y;
        
        y -= 0.2f;
    }
    
    GLfloat x = -0.8f;
    GLfloat y1 = -1.0f;
    GLfloat y2 = 1.0f;
    
    for (int i = 0; i < 40; i++) {
        // -0.8, 1.0
        // -0.8, -1.0
        vLinesVertices[i] = x;
        vLinesVertices[++i] = y1;
        vLinesVertices[++i] = x;
        vLinesVertices[++i] = y2;
        
        x += 0.2f;
    }
}

void render()
{
    // Get dimensions from IwGL
    int w = IwGLGetInt(IW_GL_WIDTH);
    int h = IwGLGetInt(IW_GL_HEIGHT);
    
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, hLinesVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, 20);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vLinesVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINES, 0, 20);
    
    IwGLSwapBuffers();
}

void rotateTriangle() {
    int w = IwGLGetInt(IW_GL_WIDTH);
    int h = IwGLGetInt(IW_GL_HEIGHT);
    float diameter = 300.0;
    if (g_pInput->m_X > diameter || (h - g_pInput->m_Y) > 300) { return; }
    cout << "INFO ::: rotateTriangle() w: " << w << << " h: " << h << endl;
    cout << "PRESSED ::: rotateTriangle() x: " << g_pInput->m_X << " y: " << g_pInput->m_Y << endl;
    
    // y = mx + b
    // y = -((h - y) - diameter/2)
    // m = ?
    // x = x
    // b = 0
    
    float slope = -((h - g_pInput->m_Y) - diameter/2);
    cout << "SLOPE ::: rotateTriangle() slope: " << slope << endl;
    
    float radians = atanf(slope);
    cout << "RADIANS ::: rotateTriangle() radians: " << radians << endl;
    
    float degrees = (radians * 180) / 3.141592653589793;
    cout << "DEGREES ::: rotateTriangle() degrees: " << degrees << endl;


}

int main()
{
    setupGrid();
    
    g_pInput = new Input();
    
    if (eglInit())
        return 1;
    printf("Screen BPP: %d\n", s3eSurfaceGetInt(S3E_SURFACE_PIXEL_TYPE) & S3E_SURFACE_PIXEL_SIZE_MASK);
    printf("\n");
    printf( "Vendor     : %s\n", (char*)glGetString(GL_VENDOR));
    printf( "Renderer   : %s\n", (char*)glGetString(GL_RENDERER));
    printf( "Version    : %s\n", (char*)glGetString(GL_VERSION));
    printf( "Extensions : %s\n", (char*)glGetString(GL_EXTENSIONS));
    printf("\n");

    GLuint vertexShader;
    GLuint fragmentShader;
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
    
    // Create the program object
    GLint programObject;
    GLint linked;
    programObject = glCreateProgram();
    
    if (programObject == 0)
        return 0;
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    
    glBindAttribLocation(programObject, 0, "vPosition");
    
    glLinkProgram(programObject);
    
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        
        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            
            GLsizei log_length;
            glGetProgramInfoLog(programObject, infoLen, &log_length, infoLog);
            printf("Error linking program:\n%s\n", infoLog);
            
            free(infoLog);
        }
        
        glDeleteProgram(programObject);
        return 0;
    }
    
    glUseProgram(programObject);
    
    bool quit = false;
    int numFrames = 0;
    while (!quit) {
        g_pInput->Update();
        s3eKeyboardUpdate();
        s3eDeviceYield(0);
        if (s3eDeviceCheckQuitRequest())
            quit = 1;
        if (s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_PRESSED)
            quit = 1;
        
        if(!g_pInput->m_Touched && g_pInput->m_PrevTouched)
        {
            rotateTriangle();
            g_pInput->Reset();
        }
        
        
        render();
        numFrames++;
    }
    
    delete g_pInput;
    //Shutdown GL system
    IwGLTerminate();
    return 0;
}