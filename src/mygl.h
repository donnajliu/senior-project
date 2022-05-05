#pragma once

#include "openglcontext.h"
#include "utils.h"
#include "shaderprogram.h"
#include "FileReader.h"
#include "Camera.h"
#include <scene/line.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public OpenGLContext
{
    //Q_OBJECT
private:
    // The camera used to view the scene.
    Camera m_camera;

    // The collection of renderable models available to the user. This vector is only
    // modified once, in createMeshes().
    std::vector<std::shared_ptr<Mesh>> m_models;
    // A pointer to the model that is currently being rendered. Used by paintGL to determine
    // which model to render.
    Mesh* mp_modelCurrent;

    // A handle for our vertex array object.
    // This will store the VBOs created in our geometry classes.
    GLuint m_vao;

    // A collection of handles to the five frame buffers we've given
    // ourselves to perform render passes. The 0th frame buffer is always
    // written to by the render pass that uses the currently bound surface shader.
    GLuint m_frameBuffer;
    // A collection of handles to the textures used by the frame buffers.
    // m_frameBuffers[i] writes to m_renderedTextures[i].
    GLuint m_renderedTexture;
    // A collection of handles to the depth buffers used by our frame buffers.
    // m_frameBuffers[i] writes to m_depthRenderBuffers[i].
    GLuint m_depthRenderBuffer;

    // A variable used to keep track of the time elapsed.
    // It increments by 1 at the end of each call of paintGL().
    // In paintGL, it is passed to the currently bound surface and post-process shaders,
    // if they have a uniform variable for time.
    int m_time;
    // A variable used to track the mouse's previous position when
    // clicking and dragging on the GL viewport. Used to move the camera
    // in the scene.
    glm::vec2 m_mousePosPrev;

    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    // Knot stuff
    Camera cam;
    glm::vec3 lightPosition, lightColor, eyePos;

    vector<Node*> nodeList;
    int numSpheres, index;
    glm::vec3 tempColor;
    clock_t old_time;
    float rotation;


    unsigned int vboPos;
    unsigned int vboCol;
    unsigned int vboNor;
    unsigned int vboIdx;


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


    void initKnotObjs();
    void traverse(Node* N, glm::mat4 T);
    void uploadGeometry(Geometry* geometry);

    FileReader fileReader;
    uPtr<Rope> rope;
    Line line;

    Node* currentNode;

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);

//protected:
    //void keyPressEvent(QKeyEvent *e);

};

