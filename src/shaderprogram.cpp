#include "shaderprogram.h"
#include <QFile>


ShaderProgram::ShaderProgram(OpenGLContext *context)
    : m_vertShader(), m_fragShader(), m_prog(),
      locationPos(-1), locationNor(-1), locationCol(-1),
      unifViewProj(-1), unifModel(-1), unifModelInvTr(-1),
      unifLightPos(-1), unifLightCol(-1), unifEyePos(-1),
      context(context)
{}

void ShaderProgram::create(const char *vertfile, const char *fragfile)
{
    // Allocate space on our GPU for a vertex shader and a fragment shader and a shader program to manage the two
    m_vertShader = context->glCreateShader(GL_VERTEX_SHADER);
    m_fragShader = context->glCreateShader(GL_FRAGMENT_SHADER);
    m_prog = context->glCreateProgram();
    // Get the body of text stored in our two .glsl files
    QString qVertSource = qTextFileRead(vertfile);
    QString qFragSource = qTextFileRead(fragfile);

    char* vertSource = new char[qVertSource.size()+1];
    strcpy(vertSource, qVertSource.toStdString().c_str());
    char* fragSource = new char[qFragSource.size()+1];
    strcpy(fragSource, qFragSource.toStdString().c_str());


    // Send the shader text to OpenGL and store it in the shaders specified by the handles vertShader and fragShader
    context->glShaderSource(m_vertShader, 1, &vertSource, 0);
    context->glShaderSource(m_fragShader, 1, &fragSource, 0);
    // Tell OpenGL to compile the shader text stored above
    context->glCompileShader(m_vertShader);
    context->glCompileShader(m_fragShader);
    // Check if everything compiled OK
    GLint compiled;
    context->glGetShaderiv(m_vertShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(m_vertShader);
    }
    context->glGetShaderiv(m_fragShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printShaderInfoLog(m_fragShader);
    }

    // Tell prog that it manages these particular vertex and fragment shaders
    context->glAttachShader(m_prog, m_vertShader);
    context->glAttachShader(m_prog, m_fragShader);
    context->glLinkProgram(m_prog);

    // Check for linking success
    GLint linked;
    context->glGetProgramiv(m_prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        printLinkInfoLog(m_prog);
    }

    // Get the handles to the variables stored in our shaders
    // See shaderprogram.h for more information about these variables

    locationPos    = context->glGetAttribLocation (m_prog, "vs_Position");
    locationNor    = context->glGetAttribLocation (m_prog, "vs_Normal");
    locationCol    = context->glGetAttribLocation (m_prog, "vs_Color");
    unifViewProj   = context->glGetUniformLocation(m_prog, "u_ViewProj");
    unifModel      = context->glGetUniformLocation(m_prog, "u_Model");
    unifModelInvTr = context->glGetUniformLocation(m_prog, "u_ModelInvTr");

    unifLightPos   = context->glGetUniformLocation(m_prog, "u_lightPos");
    unifLightCol   = context->glGetUniformLocation(m_prog, "u_lightCol");
    unifEyePos     = context->glGetUniformLocation(m_prog, "u_eyePos");

}

void ShaderProgram::useMe()
{
    context->glUseProgram(m_prog);
}

void ShaderProgram::setModelMatrix(const glm::mat4 &model)
{
    useMe();

    if (unifModel != -1)
    {
        // Pass a 3x3 matrix into a uniform variable in our shader
                        // Handle to the matrix variable on the GPU
        context->glUniformMatrix4fv(unifModel,
                        // How many matrices to pass
                           1,
                        // Transpose the matrix? OpenGL uses column-major, so no.
                           GL_FALSE,
                        // Pointer to the first element of the matrix
                           &model[0][0]);
    }
}

void ShaderProgram::setViewProjMatrix(const glm::mat4 &vp)
{
    // Tell OpenGL to use this shader program for subsequent function calls
    useMe();

    if (unifViewProj != -1)
    {
    // Pass a 3x3 matrix into a uniform variable in our shader
                    // Handle to the matrix variable on the GPU
    context->glUniformMatrix4fv(unifViewProj,
                    // How many matrices to pass
                       1,
                    // Transpose the matrix? OpenGL uses column-major, so no.
                       GL_FALSE,
                    // Pointer to the first element of the matrix
                       &vp[0][0]);
    }
}


void ShaderProgram::setLightData(const glm::vec3 &lp, const glm::vec3 &lc, const glm::vec3 &ep) {
    useMe();
    if(unifLightPos != -1) {
        context->glUniform4f(unifLightPos, lp.x, lp.y, lp.z, 1.f);
    }
    if(unifLightCol != -1) {
        context->glUniform4f(unifLightCol, lc.x, lc.y, lc.z, 1.f);
    }
    if(unifEyePos != -1) {
        context->glUniform4f(unifEyePos, ep.x, ep.y, ep.z, 1.f);
    }
}

//This function, as its name implies, uses the passed in GL widget
void ShaderProgram::draw(OpenGLContext &f, Geometry &g, glm::mat4 model, std::array<GLuint, 4> &vbos)
{   
    useMe();

    f.glUniformMatrix4fv(unifModel, 1, GL_FALSE, &model[0][0]);
    const glm::mat4 modelInvTranspose = glm::inverse(glm::transpose(model));
    f.glUniformMatrix4fv(unifModelInvTr, 1, GL_FALSE, &modelInvTranspose[0][0]);

    // Each of the following blocks checks that:
    //   * This shader has this attribute, and
    //   * This Drawable has a vertex buffer for this attribute.
    // If so, it binds the appropriate buffers to each attribute.

    if (locationPos != -1)
    {
        f.glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        context->glEnableVertexAttribArray(locationPos);
        context->glVertexAttribPointer(locationPos, 3, GL_FLOAT, false, 0, NULL);
    }

    if (locationNor != -1)
    {
        f.glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        context->glEnableVertexAttribArray(locationNor);
        context->glVertexAttribPointer(locationNor, 3, GL_FLOAT, false, 0, NULL);
    }

    if (locationCol != -1)
    {
        f.glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        context->glEnableVertexAttribArray(locationCol);
        context->glVertexAttribPointer(locationCol, 3, GL_FLOAT, false, 0, NULL);
    }

    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    f.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]); // idx buffer
    f.glDrawElements(GL_TRIANGLES, g.getIndexCount(), GL_UNSIGNED_INT, 0);

    if (locationPos != -1) context->glDisableVertexAttribArray(locationPos);
    if (locationNor != -1) context->glDisableVertexAttribArray(locationNor);
    if (locationCol != -1) context->glDisableVertexAttribArray(locationCol);

    f.printGLErrorLog();
}

//This function, as its name implies, uses the passed in GL widget
void ShaderProgram::drawLine(Drawable &d)
{
        useMe();

    // Each of the following blocks checks that:
    //   * This shader has this attribute, and
    //   * This Drawable has a vertex buffer for this attribute.
    // If so, it binds the appropriate buffers to each attribute.

        // Remember, by calling bindPos(), we call
        // glBindBuffer on the Drawable's VBO for vertex position,
        // meaning that glVertexAttribPointer associates vs_Pos
        // (referred to by attrPos) with that VBO
    if (locationPos != -1 && d.bindPos()) {
        context->glEnableVertexAttribArray(locationPos);
        context->glVertexAttribPointer(locationPos, 4, GL_FLOAT, false, 0, nullptr);
    }

    if (locationNor != -1 && d.bindNor()) {
        context->glEnableVertexAttribArray(locationNor);
        context->glVertexAttribPointer(locationNor, 4, GL_FLOAT, false, 0, nullptr);
    }

    if (locationCol != -1 && d.bindCol()) {
        context->glEnableVertexAttribArray(locationCol);
        context->glVertexAttribPointer(locationCol, 4, GL_FLOAT, false, 0, nullptr);
    }


    // Bind the index buffer and then draw shapes from it.
    // This invokes the shader program, which accesses the vertex buffers.
    d.bindIdx();
    context->glDrawElements(d.drawMode(), d.elemCount(), GL_UNSIGNED_INT, 0);

    if (locationPos != -1) context->glDisableVertexAttribArray(locationPos);
    if (locationNor != -1) context->glDisableVertexAttribArray(locationNor);
    if (locationCol != -1) context->glDisableVertexAttribArray(locationCol);

    context->printGLErrorLog();
}

char* ShaderProgram::textFileRead(const char* fileName) {
    char* text;

    if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");

        if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol, fixed by Cory
            }
            fclose(file);
        }
    }
    return text;
}



QString ShaderProgram::qTextFileRead(const char *fileName)
{
    QString text;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        text = in.readAll();
        text.append('\0');
    }
    return text;
}

void ShaderProgram::printShaderInfoLog(int shader)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    context->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0)
    {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        context->glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
        qDebug() << "ShaderInfoLog:" << Qt::endl << infoLog << Qt::endl;
        delete [] infoLog;
    }

    // should additionally check for OpenGL errors here
}

void ShaderProgram::printLinkInfoLog(int prog)
{
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    context->glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        context->glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        qDebug() << "LinkInfoLog:" << Qt::endl << infoLog << Qt::endl;
        delete [] infoLog;
    }
}
