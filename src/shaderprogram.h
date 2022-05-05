#pragma once

#include "openglcontext.h"
#include "la.h"
#include <glm/glm.hpp>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLShaderProgram>
#include "drawable.h"
#include "scene/Geometry.h"


class ShaderProgram
{
public:
    GLuint m_vertShader; // A handle for the vertex shader stored in this shader program
    GLuint m_fragShader; // A handle for the fragment shader stored in this shader program
    GLuint m_prog;       // A handle for the linked shader program stored in this class

    int locationPos;
    int locationNor;
    int locationCol;
    int unifViewProj;
    int unifModel;
    int unifModelInvTr;

    int unifLightPos;
    int unifLightCol;
    int unifEyePos;

public:
    ShaderProgram(OpenGLContext* context);
    // Sets up the requisite GL data and shaders from the given .glsl files
    void create(const char *vertfile, const char *fragfile);
    // Tells our OpenGL context to use this shader to draw things
    void useMe();
    // Pass the given model matrix to this shader on the GPU
    void setModelMatrix(const glm::mat4 &model);
    // Pass the given Projection * View matrix to this shader on the GPU
    void setViewProjMatrix(const glm::mat4 &vp);
    void setLightData(const glm::vec3 &lp, const glm::vec3 &lc, const glm::vec3 &ep);
    // Draw the given object to our screen using this ShaderProgram's shaders
    void draw(OpenGLContext &f, Geometry &g, glm::mat4 model, std::array<GLuint, 4> &vbos);
    void drawLine(Drawable &d);
    // Utility function used in create()
    char* textFileRead(const char*);
    // Utility function used in create()
    QString qTextFileRead(const char*);
    // Utility function that prints any shader compilation errors to the console
    void printShaderInfoLog(int shader);
    // Utility function that prints any shader linking errors to the console
    void printLinkInfoLog(int prog);

private:
    OpenGLContext* context;   // Since Qt's OpenGL support is done through classes like QOpenGLFunctions_3_2_Core,
                            // we need to pass our OpenGL context to the Drawable in order to call GL functions
                            // from within this class.
};

