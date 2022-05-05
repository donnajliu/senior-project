#include "line.h"

Line::Line(OpenGLContext* context, vector<glm::vec4> points) :
    Drawable(context), points(points)
{}

void Line::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> color;
    std::vector<GLuint> idx;

    pos = points;

    for (int i = 0; i < (int) points.size(); i++) {
        color.push_back(glm::vec4(5, 5, 5, 2.f));
    }

    for (int i = 0; i < (int) points.size() - 1; i++) {
        idx.push_back(i);
        idx.push_back(i+1);
    }
    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec4), color.data(), GL_STATIC_DRAW);
}

GLenum Line::drawMode() {
    return GL_LINES;
}

void Line::updatePoints(vector<glm::vec4> input) {
    this->points = input;
}
