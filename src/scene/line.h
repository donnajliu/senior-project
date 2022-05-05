#ifndef LINE_H
#define LINE_H

#include "Geometry.h"
#include <drawable.h>

class Line : public Drawable {

public:
    Line(OpenGLContext* context, vector<glm::vec4> points);
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;

    GLenum drawMode() override;

    void updatePoints(vector<glm::vec4> input);

    vector<glm::vec4> points;
};

#endif // LINE_H
