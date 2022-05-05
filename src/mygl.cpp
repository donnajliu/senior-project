#include "mygl.h"

#include "la.h"
#include "smartpointerhelp.h"
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include "FileReader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      prog_flat(this),
      fileReader(":/knots_txt_files/knot.txt"), rope(nullptr), m_camera(640, 480, glm::vec3(0, 0, 12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)), lightPosition(), lightColor(), eyePos(),
      currentNode(nullptr), nodeList(), numSpheres(0), index(0), tempColor(1.f, 1.f, 1.f),
      old_time(), rotation(0.f),
      vboPos(0), vboCol(0), vboNor(0), vboIdx(0), line(this, vector<glm::vec4>())
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vboPos);
    glDeleteBuffers(1, &vboCol);
    glDeleteBuffers(1, &vboNor);
    glDeleteBuffers(1, &vboIdx);
}


void MyGL::initKnotObjs() {
    if(!fileReader.read()) {
        cout << "File reader could not read the file" << endl;
    }
    rope = std::move(fileReader.rope);

//    cam.setE(fileReader.EYEP);
//    cam.setU(fileReader.UVEC);
//    cam.setC(fileReader.VDIR);

    m_camera.setE(fileReader.EYEP);
    m_camera.setU(fileReader.UVEC);
    m_camera.setC(fileReader.VDIR);

    lightPosition = glm::vec3(fileReader.LPOS[0], fileReader.LPOS[1], fileReader.LPOS[2]);
    lightColor    = glm::vec3(fileReader.LCOL[0], fileReader.LCOL[1], fileReader.LCOL[2]);
    eyePos        = glm::vec3(fileReader.EYEP[0], fileReader.EYEP[1], fileReader.EYEP[2]);

    currentNode = rope->root;
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(10);
    glEnable(GL_LINE_WIDTH);
    glLineWidth(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.1, 0.1, 0.1, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vboPos);
    glGenBuffers(1, &vboCol);
    glGenBuffers(1, &vboNor);
    glGenBuffers(1, &vboIdx);

    initKnotObjs();

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    // Upload the view matrix to our shader (i.e. onto the graphics card)
    glm::mat4 projection = glm::perspective(fileReader.FOVY, fileReader.RESOwidth / (float) fileReader.RESOheight, 0.1f, 100.0f);
    glm::mat4 camera = glm::lookAt(fileReader.EYEP, fileReader.VDIR, fileReader.UVEC);
    projection = projection * camera;
    prog_flat.setViewProjMatrix(projection);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    clock_t newTime = clock();
    rotation += 2.5f * (static_cast<float>(newTime - old_time) / static_cast<float>(CLOCKS_PER_SEC));
    old_time = newTime;

    // Create a matrix to pass to the model matrix uniform variable in the
    // vertex shader, which is used to transform the vertices in our draw call.
    // The default provided value is an identity matrix; you'll change this.
    glm::mat4 modelmat = glm::mat4();

    prog_flat.setLightData(lightPosition, lightColor, eyePos);

    // Upload all our little spheres
    numSpheres = 0;
    traverse(currentNode, glm::mat4(1.0f));
    //cout << "We have " << numSpheres << " spheres" << endl;

    // Move the rendering we just made onto the screen

    // Check for any GL errors that have happened recently
    printGLErrorLog();
#if 0
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_showGrid)
    {
        prog_flat.setModelMatrix(glm::mat3());
        prog_flat.draw(*this, m_geomGrid);
    }

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////

    // Shapes will be drawn on top of one another, so the last object
    // drawn will appear in front of everything else

    prog_flat.setModelMatrix(glm::mat3());
    m_geomSquare.setColor(glm::vec3(0,1,0));
    prog_flat.draw(*this, m_geomSquare);

    m_geomSquare.setColor(glm::vec3(1,0,0));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(-1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(-30.f)));
    prog_flat.draw(*this, m_geomSquare);

    m_geomSquare.setColor(glm::vec3(0,0,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_geomSquare);

    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    // Here is a good spot to call your scene graph traversal function.
    // Any time you want to draw an instance of geometry, call
    // prog_flat.draw(*this, yourNonPointerGeometry);
#endif
    // donna: give the rope thickness
    vector<glm::vec4> interpPointsThickness;

    int midX = rope->buffX / 2;
    int midY = rope->buffY / 2;
    int midZ = rope->buffZ / 2;

    int minDistance = rope->buffX + rope->buffY + rope->buffZ;
    glm::vec4 minPoint;

    // get the difference from the origin
    for (int i = 0; i < rope->interpPoints.size(); i++) {
        // get the distance from origin
        int distance = std::sqrt(pow((midX - rope->interpPoints[i][0]), 2) + pow((midY - rope->interpPoints[i][1]), 2) + pow((midZ - rope->interpPoints[i][2]), 2));

        if (distance < minDistance) {
            minDistance = distance;
            minPoint = rope->interpPoints[i];
        }
    }

    int minX = minPoint[0] - midX;
    int minY = minPoint[1] - midY;
    int minZ = minPoint[2] - midZ;

    for (int i = -10; i < 10; i++) {
        for (int j = 0; j < rope->interpPoints.size(); j++) {
            //std::cout << rope->inteårpPoints[j][0] << " " << rope->interpPoints[j][1] << " " << rope->interpPoints[j][2] << std::endl;
            interpPointsThickness.push_back(rope->interpPoints[j] + glm::vec4(i * 0.001) + glm::vec4(2.0f * minX, minY, 2.0f * minZ, 0));// - glm::vec4(30, 15, 0, 0));
        }
    }

    line.updatePoints(interpPointsThickness);
    //line.updatePoints(rope->interpPoints);
    line.create();
    prog_flat.drawLine(line);
}


void MyGL::traverse(Node* N, glm::mat4 T) {

    N->transform();
    T = T * N->transformation;

    N->totalTransformation = T;

    if (N->hasGeometry) {
        uploadGeometry(N->geometry);
//        drawGeometry(N->geometry, T);
        std::array<GLuint, 4> vbos{vboPos, vboNor, vboCol, vboIdx};
        prog_flat.draw(*this, *(N->geometry), T, vbos);
        numSpheres++;
    }

    Node* thisNode;
    for (unsigned int i = 0; i < N->children.size(); i++) {
        thisNode = N->children[i];
        traverse(thisNode, T);

    }
}

void MyGL::uploadGeometry(Geometry* geometry) {

    int VERTICES = geometry->getVertexCount();
    int INDICES = geometry->getIndexCount();

    // Sizes of the various array elements below.
    static const GLsizei SIZE_POS = sizeof(glm::vec3);
    static const GLsizei SIZE_NOR = sizeof(glm::vec3);
    static const GLsizei SIZE_COL = sizeof(glm::vec3);
    static const GLsizei SIZE_IND = sizeof(GLuint);

    vector<glm::vec3> positions = geometry->getVertices();
    vector<glm::vec3> normals = geometry->getNormals();
    vector<unsigned int> indices = geometry->getIndices();
    vector<glm::vec3> colors = geometry->getColors();

    // ================UPLOADING CODE (GENERALLY, ONCE PER CHANGE IN DATA)==============
    // Now we put the data into the Vertex Buffer Object for the graphics system to use
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    // Use STATIC_DRAW since the square's vertices don't need to change while the program runs.
    // Take a look at STREAM_DRAW and DYNAMIC_DRAW to see when they should be used.
    // Always make sure you are telling OpenGL the right size to make the buffer. Here we need 16 floats.
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, &positions[0], GL_STATIC_DRAW);

    // Bind+upload the color data
    glBindBuffer(GL_ARRAY_BUFFER, vboCol);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, &colors[0], GL_STATIC_DRAW);

    // Bind+upload the normals
    glBindBuffer(GL_ARRAY_BUFFER, vboNor);
    glBufferData(GL_ARRAY_BUFFER, VERTICES * SIZE_POS, &normals[0], GL_STATIC_DRAW);

    // Bind+upload the indices to the GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIdx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * SIZE_IND, &indices[0], GL_STATIC_DRAW);

    // Once data is loaded onto the GPU, we are done with the float arrays.
    // For your scene graph implementation, you shouldn't create and delete the vertex information
    // every frame. You would probably want to store and reuse them.

}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    switch (e->key()) {
        case 'q':
//          cleanup();
            exit(0);
            break;
        case Qt::Key_P:
            rope->printVoxels();
            break;
        case Qt::Key_B: {
            vector<glm::vec3> newControlPoints = rope->bSplineCurver();
            rope->getCurve(newControlPoints);
            rope->writeOut(newControlPoints);
            break;
        }
//        case 'n':
//            index++;
//            currentNode->color = tempColor;
//            currentNode->refreshColors();

//            currentNode = nodeList[index % nodeList.size()];
//            tempColor = currentNode->color;
//            currentNode->color = currentNode->color * 1.5f;
//            currentNode->refreshColors();
//            break;
        case 'N':
            index = 0;
            currentNode->color = tempColor;
            currentNode->refreshColors();

            currentNode = nodeList[0];
            tempColor = currentNode->color;
            currentNode->color = currentNode->color * 1.5f;
            currentNode->refreshColors();
            break;
        case Qt::Key_A:
            // donna: set all of the translations
            currentNode->setXTrans(currentNode->transVec.x - 0.5);
            break;
        case Qt::Key_D:
            currentNode->setXTrans(currentNode->transVec.x + 0.5);
            break;
        case Qt::Key_Q:
            currentNode->setYTrans(currentNode->transVec.y - 0.5);
            break;
        case Qt::Key_E:
            currentNode->setYTrans(currentNode->transVec.y + 0.5);
            break;
        case Qt::Key_S:
            currentNode->setZTrans(currentNode->transVec.z - 0.5);
            break;
        case Qt::Key_W:
            currentNode->setZTrans(currentNode->transVec.z + 0.5);
            break;
        // Scale
        case '+':
            // donna: zoom in
            currentNode->setXScale(currentNode->scaleVec.x + 0.1);
            currentNode->setYScale(currentNode->scaleVec.y + 0.1);
            currentNode->setZScale(currentNode->scaleVec.z + 0.1);
            break;
        case '-':
            // donna: zoom out
            currentNode->setXScale(currentNode->scaleVec.x - 0.1);
            currentNode->setYScale(currentNode->scaleVec.y - 0.1);
            currentNode->setZScale(currentNode->scaleVec.z - 0.1);
            break;
        case Qt::Key_I:
            currentNode->setXRotation(currentNode->rotVec.x + (10.0f * PI / 180.0f));
            break;
        case Qt::Key_M:
            currentNode->setXRotation(currentNode->rotVec.x - (10.0f * PI / 180.0f));
            break;
        case Qt::Key_L:
            currentNode->setYRotation(currentNode->rotVec.y + (10.0f * PI / 180.0f));
            break;
        case Qt::Key_J:
            currentNode->setYRotation(currentNode->rotVec.y - (10.0f * PI / 180.0f));
            break;
        case Qt::Key_K:
            currentNode->setZRotation(currentNode->rotVec.z + (10.0f * PI / 180.0f));
            break;
        case Qt::Key_O:
            currentNode->setZRotation(currentNode->rotVec.z - (10.0f * PI / 180.0f));
            break;
        case Qt::Key_1:
            eyePos;
            break;
        case ' ':
            rope->simulate();
            break;
//        case '1' :
//            rope->thinXY();
//            break;
//        case '2' :
//            rope->thinXZ();
//            break;
//        case '3' :
//            rope->thinYZ();
//            break;
//        case '4' :
//        	rope->pushXY();
//        	break;
//        case '5' :
//        	rope->pushXZ();
//        	break;
//        case '6' :
//        	rope->pushYZ();
//        	break;
    }
}
