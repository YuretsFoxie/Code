///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// ===========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2022-05-12
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <cmath>
#include <sstream>
#include "ModelGL.h"
#include "BoundingBox.h"
#include "textureUtils.h"
#include "Timer.h"
#include "Png.h"

// constants
const float DEG2RAD = 3.141593f / 180;
const float FOV_Y = 60.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 1000.0f;
const int   CAPTURE_WIDTH = 1024;
const int   CAPTURE_HEIGHT = 1024;
const int   MSAA = 8;               // multi-sample anti-aliasing



// flat shading ===========================================
const char* vsSource1 = R"(
void main()
{
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
)";
const char* fsSource1 = R"(
void main()
{
    gl_FragColor = gl_Color;
}
)";


// phong shading ==========================================
const char* vsSource2 = R"(
varying vec3 esVertex, esNormal;
void main()
{
    esVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    esNormal = gl_NormalMatrix * gl_Normal;
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
)";
const char* fsSource2 = R"(
varying vec3 esVertex, esNormal;
void main()
{
    vec3 normal = normalize(esNormal);
    vec3 light;
    if(gl_LightSource[0].position.w == 0.0)
    {
        light = normalize(gl_LightSource[0].position.xyz);
    }
    else
    {
        light = normalize(gl_LightSource[0].position.xyz - esVertex);
    }
    vec3 reflectVec = reflect(-light, normal);
    vec3 viewVec = normalize(-esVertex);
    vec4 color =  gl_FrontMaterial.ambient * gl_FrontLightProduct[0].ambient;
    float dotNL = max(dot(normal, light), 0.0);
    color += gl_FrontMaterial.diffuse * gl_FrontLightProduct[0].diffuse * dotNL;
    float dotVR = max(dot(viewVec, reflectVec), 0.0);
    /*vec4 specular = (vec4(1.0) - color) * gl_FrontMaterial.specular * gl_FrontLightProduct[0].specular * pow(dotVR, gl_FrontMaterial.shininess);
    color += specular;*/
    color += gl_FrontMaterial.specular * gl_FrontLightProduct[0].specular * pow(dotVR, gl_FrontMaterial.shininess);
    gl_FragColor = color;
}
)";


// phong shading + texture
const char* vsSource3 = R"(
varying vec3 esVertex, esNormal;
void main()
{
    esVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    esNormal = gl_NormalMatrix * gl_Normal;
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
)";
const char* fsSource3 = R"(
uniform sampler2D map0;
varying vec3 esVertex, esNormal;
void main()
{
    vec3 normal = normalize(esNormal);
    vec3 light;
    if(gl_LightSource[0].position.w == 0.0)
    {
        light = normalize(gl_LightSource[0].position.xyz);
    }
    else
    {
        light = normalize(gl_LightSource[0].position.xyz - esVertex);
    }
    vec3 reflectVec = reflect(-light, normal);
    vec3 viewVec = normalize(-esVertex);
    vec4 color = gl_FrontMaterial.ambient * gl_FrontLightProduct[0].ambient;
    float dotNL = max(dot(normal, light), 0.0);
    color += gl_FrontMaterial.diffuse * gl_FrontLightProduct[0].diffuse * dotNL;
    color *= texture2D(map0, gl_TexCoord[0].st);
    float dotVR = max(dot(viewVec, reflectVec), 0.0);
    /*vec4 specular = (vec4(1.0) - color) * gl_FrontMaterial.specular * gl_FrontLightProduct[0].specular * pow(dotVR, gl_FrontMaterial.shininess);
    color += specular;*/
    color += gl_FrontMaterial.specular * gl_FrontLightProduct[0].specular * pow(dotVR, gl_FrontMaterial.shininess);
    gl_FragColor = color;
}
)";



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ModelGL::ModelGL() : windowWidth(0), windowHeight(0), mouseLeftDown(false),
                     mouseRightDown(false), drawModeChanged(false), drawMode(0),
                     windowSizeChanged(false), nearPlane(NEAR_PLANE),
                     farPlane(FAR_PLANE), gridEnabled(true), textureEnabled(true),
                     boundingBoxEnabled(true), defaultLightingEnabled(true),
                     spinEnabled(false), modelRadius(1.0f), objLoading(false), objSaving(false),
                     gridSize(10), gridStep(1), selectedGroupId(-1), textureLoading(false),
                     vboSupported(false), vboVertex(0), captureEnabled(false),
                     captureSizeChanged(false), shiftEnabled(false),
                     defaultTextureEnabled(false), glslSupported(false),
                     glslReady(false), progId1(0), progId2(0), progId3(0),
                     anisotropicSupported(false), anisotropy(1.0f),
                     callee(0), callbackObjLoaded(0), callbackObjSaved(0), callbackCaptureSaved(0),
                     meshVolume(0), meshArea(0)
{
    bgColor.set(0,0,0,0);

    modelPosition.set(0, 0, 0);
    modelAngle.set(0, 0, 0);
    modelScale.set(1, 1, 1);

    // model, view, modelview matrices are fixed in this app
    matrixView.identity();
    matrixModel.identity();
    matrixModelView.identity();
    matrixProjection.identity();

    // init default material
    defaultAmbient[0] = defaultAmbient[1] = defaultAmbient[2] = 0.3f; defaultAmbient[3] = 1.0f;
    defaultDiffuse[0] = defaultDiffuse[1] = defaultDiffuse[2] = 1.0f; defaultDiffuse[3] = 1.0f;
    defaultSpecular[0] = defaultSpecular[1] = defaultSpecular[2] = 0.5f; defaultSpecular[3] = 1.0f;
    defaultShininess = 128.0f;

    // default light position
    lightPosition[0] = 0;
    lightPosition[1] = 0;
    lightPosition[2] = 1;
    lightPosition[3] = 0;

    captureWidth = CAPTURE_WIDTH;
    captureHeight = CAPTURE_HEIGHT;
}



///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
ModelGL::~ModelGL()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL states and scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::init()
{
    glShadeModel(GL_SMOOTH);                        // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);          // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);   // background color
    glClearStencil(0);                              // clear stencil buffer
    glClearDepth(1.0f);                             // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();

    // check extensions
    glExtension& extension = glExtension::getInstance();
    glslSupported = extension.isSupported("GL_ARB_shader_objects");
    if(glslSupported)
        createShaderPrograms();

    vboSupported = extension.isSupported("GL_ARB_vertex_buffer_object");
    if(vboSupported)
        createVertexBufferObjects();

    fboSupported = extension.isSupported("GL_ARB_framebuffer_object");
    if(fboSupported)
        fboSupported = fbo.init(captureWidth, captureHeight, MSAA);

    anisotropicSupported = extension.isSupported("GL_ARB_texture_filter_anisotropic");
    if(anisotropicSupported)
    {
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }
}



///////////////////////////////////////////////////////////////////////////////
// clean up OpenGL objects
///////////////////////////////////////////////////////////////////////////////
void ModelGL::quit()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void ModelGL::initLights()
{
    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {1.0f, 1.0f, 1.0f, 1.0f};   // ambient light
    GLfloat lightKd[] = {1.0f, 1.0f, 1.0f, 1.0f};   // diffuse light
    GLfloat lightKs[] = {1.0f, 1.0f, 1.0f, 1.0f};   // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light in eye space
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_LIGHT0);                            // MUST enable each light source after configuration
}
void ModelGL::resetLightPosition()
{
    lightPosition[0] = lightPosition[1] = 0;
    lightPosition[2] = 1;
}



///////////////////////////////////////////////////////////////////////////////
// set default material
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setAmbient(float value)
{
    defaultAmbient[0] = defaultAmbient[1] = defaultAmbient[2] = value;
}
void ModelGL::setDiffuse(float value)
{
    defaultDiffuse[0] = defaultDiffuse[1] = defaultDiffuse[2] = value;
}
void ModelGL::setSpecular(float value)
{
    defaultSpecular[0] = defaultSpecular[1] = defaultSpecular[2] = value;
}
void ModelGL::resetMaterial()
{
    defaultAmbient[0] = defaultAmbient[1] = defaultAmbient[2] = 0.3f; defaultAmbient[3] = 1.0f;
    defaultDiffuse[0] = defaultDiffuse[1] = defaultDiffuse[2] = 1.0f; defaultDiffuse[3] = 1.0f;
    defaultSpecular[0] = defaultSpecular[1] = defaultSpecular[2] = 0.5f; defaultSpecular[3] = 1.0f;
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
    float forward[4];
    float up[4];
    float left[4];
    float position[4];
    float invLength;

    // determine forward vector (direction reversed because it is camera)
    forward[0] = posX - targetX;    // x
    forward[1] = posY - targetY;    // y
    forward[2] = posZ - targetZ;    // z
    forward[3] = 0.0f;              // w
    // normalize it without w-component
    invLength = 1.0f / sqrtf(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    forward[0] *= invLength;
    forward[1] *= invLength;
    forward[2] *= invLength;

    // assume up direction is straight up
    up[0] = 0.0f;   // x
    up[1] = 1.0f;   // y
    up[2] = 0.0f;   // z
    up[3] = 0.0f;   // w

    // compute left vector with cross product
    left[0] = up[1]*forward[2] - up[2]*forward[1];  // x
    left[1] = up[2]*forward[0] - up[0]*forward[2];  // y
    left[2] = up[0]*forward[1] - up[1]*forward[0];  // z
    left[3] = 1.0f;                                 // w

    // re-compute orthogonal up vector
    up[0] = forward[1]*left[2] - forward[2]*left[1];    // x
    up[1] = forward[2]*left[0] - forward[0]*left[2];    // y
    up[2] = forward[0]*left[1] - forward[1]*left[0];    // z
    up[3] = 0.0f;                                       // w

    // camera position
    position[0] = -posX;
    position[1] = -posY;
    position[2] = -posZ;
    position[3] = 1.0f;

    // copy axis vectors to matrix
    matrixView.identity();
    matrixView.setColumn(0, left);
    matrixView.setColumn(1, up);
    matrixView.setColumn(2, forward);
    matrixView.setColumn(3, position);
}



///////////////////////////////////////////////////////////////////////////////
// set rendering window size
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setWindowSize(int width, int height)
{
    // assign the width/height of viewport
    windowWidth = width;
    windowHeight = height;
    windowSizeChanged = true;
}



///////////////////////////////////////////////////////////////////////////////
// configure projection and viewport
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setViewport(int x, int y, int w, int h)
{
    // set viewport to be the entire window
    glViewport((GLsizei)x, (GLsizei)y, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    setFrustum(FOV_Y, (float)(w)/h, nearPlane, farPlane); // FOV, AspectRatio, NearClip, FarClip
}



///////////////////////////////////////////////////////////////////////////////
// draw 2D/3D scene
///////////////////////////////////////////////////////////////////////////////
void ModelGL::draw()
{
    preFrame();

    // clear buffer
    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // pass projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrixProjection.get());
    glMatrixMode(GL_MODELVIEW);

    // position the light in eye space
    //glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //glPopMatrix();


    if(glslReady)
        glUseProgram(progId1);

    // draw grid and axis
    if(gridEnabled)
    {
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);

        // copy  the current ModelView matrix to OpenGL after transpose
        glLoadMatrixf(matrixView.get());
        drawGrid(gridSize, gridStep);
        drawAxis(gridSize);

        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
    }

    // draw OBJ mesh
    glLoadMatrixf(matrixModelView.get());
    if(!objLoading)
    {
        if(vboSupported)
            drawObjWithVbo();
        else
            drawObj();

        // draw selected group with wireframe
        if(selectedGroupId >= 0 && selectedGroupId < (int)obj.getGroupCount())
            drawSelectedGroup();

        // draw bbox
        if(boundingBoxEnabled)
            drawBoundingBox();
    }

    postFrame();
}



///////////////////////////////////////////////////////////////////////////////
// pre-frame
///////////////////////////////////////////////////////////////////////////////
void ModelGL::preFrame()
{
    if(windowSizeChanged)
    {
        setViewport(0, 0, windowWidth, windowHeight);
        windowSizeChanged = false;
    }

    if(drawModeChanged)
    {
        if(drawMode == 0)           // fill mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(drawMode == 1)      // wireframe mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(drawMode == 2)      // point mode
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            //glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }

        drawModeChanged = false;
    }

    // load obj within OpenGL rendering thread
    if(objLoading)
    {
        loadObj(objName);
        objLoading = false;
    }

    if(objSaving)
    {
        saveObj(objNameSave);
        objSaving = false;
    }

    // load texture within OpenGL rendering thread
    if(textureLoading)
    {
        loadTextures();
        textureLoading = false;
    }

    // fbo size changed
    if(captureSizeChanged)
    {
        fbo.init(captureWidth, captureHeight, MSAA);
        captureSizeChanged = false;
    }

    // increment angle
    if(spinEnabled)
    {
        modelAngle.y += 0.2f;
        updateModelMatrix();
    }

    // increment camera shift delta
    if(shiftEnabled)
    {
        cameraShift += shiftDelta;
        updateViewMatrix();
    }
}


///////////////////////////////////////////////////////////////////////////////
// post-frame
///////////////////////////////////////////////////////////////////////////////
void ModelGL::postFrame()
{
    if(captureEnabled)
    {
        Timer t;
        t.start();

        // draw to FBO
        drawToFrameBuffer();

        // flip vertically
        int width = fbo.getWidth();
        int height = fbo.getHeight();
        unsigned char* data = new unsigned char[width * height * 4];
        int rowBytes = width * 4;
        const unsigned char* src = fbo.getColorBuffer();
        unsigned char* dst = data + (rowBytes * (height - 1));
        for(int row = 0; row < height; ++row, src += rowBytes, dst -= rowBytes)
            memcpy(dst, src, rowBytes);

        // save to image
        Image::Png png;
        bool result = png.save(captureName.c_str(), width, height, 4, data);

        setViewport(0, 0, windowWidth, windowHeight);
        delete [] data;
        captureEnabled = false;

        // send result to callback
        if(callbackCaptureSaved)
            callbackCaptureSaved(callee, result, t.getElapsedTimeInMilliSec());
    }
}



///////////////////////////////////////////////////////////////////////////////
// rotate the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::rotateCamera(int x, int y)
{
    cameraAngle.y -= (x - mouseX) * 0.5f;
    cameraAngle.x -= (y - mouseY) * 0.5f;
    mouseX = x;
    mouseY = y;

    // constrain x angle -89 < x < 89
    if(cameraAngle.x < -89.0f)
        cameraAngle.x = -89.0f;
    else if(cameraAngle.x > 89.0f)
        cameraAngle.x = 89.0f;

    updateViewMatrix();
}



///////////////////////////////////////////////////////////////////////////////
// zoom the camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::zoomCamera(int y)
{
    cameraPosition.z -= (y - mouseY) * (modelRadius * 0.05f);
    mouseY = y;

    // constrain min and max
    if(cameraPosition.z < modelRadius * 0.3f)
        cameraPosition.z = modelRadius * 0.3f;
    else if(cameraPosition.z > modelRadius * 20)
        cameraPosition.z = modelRadius * 20;

    updateViewMatrix();
}
void ModelGL::zoomCameraDelta(float delta)
{
    cameraPosition.z -= delta * (modelRadius * 0.05f);

    // constrain min and max
    if(cameraPosition.z < modelRadius * 0.3f)
        cameraPosition.z = modelRadius * 0.3f;
    else if(cameraPosition.z > modelRadius * 20)
        cameraPosition.z = modelRadius * 20;

    updateViewMatrix();
}



///////////////////////////////////////////////////////////////////////////////
// shift camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::shiftCamera(int x, int y)
{
    cameraShift.x += (x - mouseX) * modelRadius * 0.005f;
    cameraShift.y -= (y - mouseY) * modelRadius * 0.005f;

    mouseX = x;
    mouseY = y;

    updateViewMatrix();
}
void ModelGL::shiftCameraDelta(float dx, float dy)
{
    cameraShift.x += dx * modelRadius * 0.01f;
    cameraShift.y += dy * modelRadius * 0.01f;

    updateViewMatrix();
}
void ModelGL::startShiftCamera(float dx, float dy)
{
    shiftDelta.x = dx * modelRadius * 0.01f;
    shiftDelta.y = dy * modelRadius * 0.01f;
    shiftEnabled = true;
}
void ModelGL::stopShiftCamera()
{
    shiftEnabled = false;
}



///////////////////////////////////////////////////////////////////////////////
// change drawing mode
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setDrawMode(int mode)
{
    if(drawMode != mode)
    {
        drawModeChanged = true;
        drawMode = mode;
    }
}



///////////////////////////////////////////////////////////////////////////////
// set a perspective frustum with 6 params similar to glFrustum()
// (left, right, bottom, top, near, far)
// Note: this is for row-major notation. OpenGL needs transpose it
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setFrustum(float l, float r, float b, float t, float n, float f)
{
    matrixProjection.identity();
    matrixProjection[0]  =  2 * n / (r - l);
    matrixProjection[5]  =  2 * n / (t - b);
    matrixProjection[8]  =  (r + l) / (r - l);
    matrixProjection[9]  =  (t + b) / (t - b);
    matrixProjection[10] = -(f + n) / (f - n);
    matrixProjection[11] = -1;
    matrixProjection[14] = -(2 * f * n) / (f - n);
    matrixProjection[15] =  0;
}



///////////////////////////////////////////////////////////////////////////////
// set a symmetric perspective frustum with 4 params similar to gluPerspective
// (vertical field of view, aspect ratio, near, far)
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setFrustum(float fovY, float aspectRatio, float front, float back)
{
    float tangent = tanf(fovY/2 * DEG2RAD);   // tangent of half fovY
    float height = front * tangent;           // half height of near plane
    float width = height * aspectRatio;       // half width of near plane

    // params: left, right, bottom, top, near, far
    setFrustum(-width, width, -height, height, front, back);
}



///////////////////////////////////////////////////////////////////////////////
// set a orthographic frustum with 6 params similar to glOrtho()
// (left, right, bottom, top, near, far)
// Note: this is for row-major notation. OpenGL needs transpose it
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setOrthoFrustum(float l, float r, float b, float t, float n, float f)
{
    matrixProjection.identity();
    matrixProjection[0]  =  2 / (r - l);
    matrixProjection[5]  =  2 / (t - b);
    matrixProjection[10] = -2 / (f - n);
    matrixProjection[12]  =  -(r + l) / (r - l);
    matrixProjection[13]  =  -(t + b) / (t - b);
    matrixProjection[14] = -(f + n) / (f - n);
}




///////////////////////////////////////////////////////////////////////////////
// load obj model
///////////////////////////////////////////////////////////////////////////////
void ModelGL::startLoadObj(const std::string fileName, CallbackResult callback, void* callbackObj)
{
    objLoading = true;
    selectedGroupId = -1;   // unselect group
    callbackObjLoaded = callback;
    callee = callbackObj;
    objName = fileName;
}

bool ModelGL::loadObj(const std::string fileName)
{
    Timer t;
    t.start();
    bool result = obj.read(fileName.c_str());
    if(result)
    {
        resetMatrices();

        // remember radius
        BoundingBox bbox = obj.getBoundingBox();
        modelRadius = bbox.getRadius();

        // compute grid size
        setGridSize(modelRadius);

        // set ready to load texture
        textures.assign(obj.getGroupCount(), defaultTexture);
        textureLoading = true;

        // create VBOs for OBJ model
        createVertexBufferObjects();
    }
    t.stop();

    // compute volume and surface area
    meshVolume = (float)computeVolume(obj.getVertices(), obj.getIndices(), obj.getIndexCount());
    meshArea = (float)computeArea(obj.getVertices(), obj.getIndices(), obj.getIndexCount());

    // return to callback
    if(callbackObjLoaded)
        callbackObjLoaded(callee, result, t.getElapsedTimeInMilliSec());

    return result;
}



///////////////////////////////////////////////////////////////////////////////
// save OBJ
///////////////////////////////////////////////////////////////////////////////
void ModelGL::startSaveObj(const std::string& fileName, CallbackResult callback, void* callbackObj)
{
    objSaving = true;
    callbackObjSaved = callback;
    callee = callbackObj;
    objNameSave = fileName;
}

bool ModelGL::saveObj(const std::string& fileName)
{
    Timer t;
    t.start();

    // transform model, the order is: scale -> rotate(z->y->x) -> translate
    Matrix4 matrix;
    matrix.scale(modelScale.x, modelScale.y, modelScale.z);
    matrix.rotateZ(modelAngle.z);
    matrix.rotateY(modelAngle.y);
    matrix.rotateX(modelAngle.x);
    matrix.translate(modelPosition);

    bool result = obj.save(fileName.c_str(), textureEnabled, matrix.get());
    t.stop();

    // return to callback
    if(callbackObjSaved)
        callbackObjSaved(callee, result, t.getElapsedTimeInMilliSec());

    return result;
}



///////////////////////////////////////////////////////////////////////////////
// draw obj model
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawObj()
{
    // skip if obj contains nothing
    if(obj.getGroupCount() <= 0)
        return;

    // enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // before draw, specify vertex arrays
    const float* vertices = obj.getInterleavedVertices();
    int stride = obj.getInterleavedStride();
    glVertexPointer(3, GL_FLOAT, stride, vertices);
    glNormalPointer(GL_FLOAT, stride, vertices + 3);

    if(obj.getTexCoordCount() == obj.getVertexCount())
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, stride, vertices + 6);
        glEnable(GL_TEXTURE_2D);
        if(glslReady)
        {
            glUseProgram(progId3);
        }
    }
    else
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        if(glslReady)
            glUseProgram(progId2);
    }

    int count = (int)obj.getGroupCount();
    for(int i = 0; i < count; ++i)
    {
        if(textureEnabled)
        {
            if(defaultTextureEnabled)
                glBindTexture(GL_TEXTURE_2D, defaultTexture);
            else
                glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            if(glslReady)
                glUseProgram(progId2);
        }

        // material
        if(defaultLightingEnabled)
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
            glMaterialf(GL_FRONT, GL_SHININESS, defaultShininess);
        }
        else
        {
            ObjMaterial material = obj.getMaterial(i);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
            glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
        }

        glDrawElements(GL_TRIANGLES, (GLsizei)obj.getIndexCount(i), GL_UNSIGNED_INT, obj.getIndices(i));
    }

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);

    // reset shader to no-texturing & no-lighting
    if(glslReady)
        glUseProgram(progId1);
}



///////////////////////////////////////////////////////////////////////////////
// draw OBJ model with VBOs
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawObjWithVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);

    // before draw, specify vertex and index arrays with their offsets and stride
    int stride = obj.getInterleavedStride();
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, stride, (void*)(sizeof(float)*3));
    glVertexPointer(3, GL_FLOAT, stride, 0);

    if(obj.getTexCoordCount() == obj.getVertexCount())  // texture coords defined
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, stride, (void*)(sizeof(float)*6));
        glEnable(GL_TEXTURE_2D);
        if(glslReady)
        {
            glUseProgram(progId3);
        }
    }
    else                // no tex coords defined
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        if(glslReady)
            glUseProgram(progId2);
    }

    for(int i = 0; i < (int)vboIndices.size(); ++i)
    {
        // texture
        if(textureEnabled)
        {
            if(defaultTextureEnabled)
                glBindTexture(GL_TEXTURE_2D, defaultTexture);
            else
                glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            if(glslReady)
                glUseProgram(progId2);
        }

        // material
        if(defaultLightingEnabled)
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
            glMaterialf(GL_FRONT, GL_SHININESS, defaultShininess);
        }
        else
        {
            ObjMaterial material = obj.getMaterial(i);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
            glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices[i]);
        glDrawElements(GL_TRIANGLES, obj.getIndexCount(i), GL_UNSIGNED_INT, 0);
    }

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // reset shader to no-texturing & no-lighting
    if(glslReady)
        glUseProgram(progId1);
}



///////////////////////////////////////////////////////////////////////////////
// draw selected group only with wireframe
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawSelectedGroup()
{
    const float color[] = {0.6f, 0.8f, 0.9f, 1.0f};

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0f, -1.0f);

    if(drawMode == 0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // enable vertex arrays
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);

    // before draw, specify vertex arrays
    const float* vertices = obj.getInterleavedVertices();
    int stride = obj.getInterleavedStride();
    glVertexPointer(3, GL_FLOAT, stride, vertices);
    glNormalPointer(GL_FLOAT, stride, vertices + 3);

    glColor4fv(color);
    glDrawElements(GL_TRIANGLES, (GLsizei)obj.getIndexCount(selectedGroupId), GL_UNSIGNED_INT, obj.getIndices(selectedGroupId));

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_NORMAL_ARRAY);

    // restore the previous polygon mode
    if(drawMode == 0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glDisable(GL_POLYGON_OFFSET_LINE);
}



///////////////////////////////////////////////////////////////////////////////
// draw a grid on the xz plane
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawGrid(float size, float step)
{
    // disable lighting
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    glColor3f(0.8f, 0.8f, 0.8f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(0.5f, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(0,0,0.5f);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();

    // enable lighting back
    glEnable(GL_LIGHTING);
}



///////////////////////////////////////////////////////////////////////////////
// draw the local axis of an object
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
    glDisable(GL_LIGHTING);

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glEnable(GL_LIGHTING);
    glDepthFunc(GL_LEQUAL);
}



///////////////////////////////////////////////////////////////////////////////
// draw the bounding box
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawBoundingBox()
{
    float colorLine[4] = {0.6f, 0.8f, 1.0f, 0.8f};
    float colorPoint[4] = {1.0f, 0.5f, 0.2f, 1.0f};
    const BoundingBox& bbox = obj.getBoundingBox();

    // draw lines
    glDisable(GL_LIGHTING);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4fv(colorLine);
    glBegin(GL_LINE_LOOP);
    glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
    glEnd();

    // draw points
    glPointSize(3);
    glColor4fv(colorPoint);
    glBegin(GL_POINTS);
    glVertex3f(bbox.minX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.minX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.minZ);
    glVertex3f(bbox.minX, bbox.maxY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.minY, bbox.maxZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.minZ);
    glVertex3f(bbox.maxX, bbox.maxY, bbox.maxZ);
    glEnd();
    glPointSize(1);

    glEnable(GL_LIGHTING);

}



///////////////////////////////////////////////////////////////////////////////
// reset camera
///////////////////////////////////////////////////////////////////////////////
void ModelGL::resetMatrices()
{
    BoundingBox bbox = obj.getBoundingBox();

    // set model center and rotation angle
    modelCenter.x = bbox.getCenterX();
    modelCenter.y = bbox.getCenterY();
    modelCenter.z = bbox.getCenterZ();
    modelAngle.set(0, 0, 0);
    modelPosition.set(0, 0, 0);
    modelScale.set(1, 1, 1);

    // set camera position and rotation angle
    cameraAngle.set(0,0,0);
    cameraShift.set(0,0,0);
    cameraPosition.x = cameraPosition.y = 0;
    cameraPosition.z = bbox.getRadius() * 2.5f;

    // update modelview matrices
    updateModelMatrix();
    updateViewMatrix();

    // update projection matrix
    nearPlane = bbox.getRadius() * 0.1f;
    farPlane = bbox.getRadius() * 50.0f;
    windowSizeChanged = true;
    setFrustum(FOV_Y, (float)windowWidth/windowHeight, nearPlane, farPlane);
}



///////////////////////////////////////////////////////////////////////////////
// update matrix
///////////////////////////////////////////////////////////////////////////////
void ModelGL::updateViewMatrix()
{
    // transform the camera (viewing matrix) from world space to eye space
    // Notice all values are negated, because we move the whole scene with the
    // inverse of camera transform matrix
    matrixView.identity();
    matrixView.translate(-modelCenter);
    matrixView.rotateZ(-cameraAngle.z);     // roll
    matrixView.rotateY(-cameraAngle.y);     // heading
    matrixView.rotateX(-cameraAngle.x);     // pitch
    matrixView.translate(-cameraPosition);

    Matrix4 matrixShift;
    matrixShift.translate(cameraShift);
    matrixView = matrixShift * matrixView;

    matrixModelView = matrixView * matrixModel;
}

void ModelGL::updateModelMatrix()
{
    // transform objects from object space to world space
    // it rotates at the center of the model
    matrixModel.identity();
    //matrixModel.translate(-modelCenter);
    matrixModel.scale(modelScale.x, modelScale.y, modelScale.z);
    matrixModel.rotateZ(modelAngle.z);
    matrixModel.rotateY(modelAngle.y);
    matrixModel.rotateX(modelAngle.x);
    matrixModel.translate(modelPosition);
    //matrixModel.translate(modelCenter);

    matrixModelView = matrixView * matrixModel;
}



///////////////////////////////////////////////////////////////////////////////
// compute grid size and step based on model radius
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setGridSize(float radius)
{
    if(radius < 5.0f)
    {
        gridSize = 5;
        gridStep = 1;
    }
    else if(radius < 10.0f)
    {
        gridSize = 10;
        gridStep = 1;
    }
    else if(radius < 50.0f)
    {
        gridSize = 50;
        gridStep = 10;
    }
    else if(radius < 100.0f)
    {
        gridSize = 100;
        gridStep = 10;
    }
    else if(radius < 500.0f)
    {
        gridSize = 500;
        gridStep = 100;
    }
    else if(radius < 1000.0f)
    {
        gridSize = 1000;
        gridStep = 100;
    }
    else if(radius < 5000.0f)
    {
        gridSize = 5000;
        gridStep = 1000;
    }
    else
    {
        gridSize = (int)radius + 5000.0f;
        gridStep = 1000;
    }
}



///////////////////////////////////////////////////////////////////////////////
// load textures
// the size of container is allocated in loadObj() and assigned with default
// texture initially. Re-assign texture ID after load it here.
///////////////////////////////////////////////////////////////////////////////
void ModelGL::loadTextures()
{
    std::string dir = obj.getObjDirectory();
    int groupCount = (int)obj.getGroupCount();
    for(int i = 0; i < groupCount; ++i)
    {
        ObjMaterial material = obj.getMaterial(i);
        std::string textureName = material.textureName;
        GLuint texture = 0;
        if(textureName.size() > 0)
            texture = loadTexture(dir + textureName);
        else
            texture = defaultTexture;

        textures[i] = texture;
    }
}



///////////////////////////////////////////////////////////////////////////////
// load default texture, must provide with fullpath
///////////////////////////////////////////////////////////////////////////////
void ModelGL::loadDefaultTexture(const std::string& texName)
{
    defaultTexture = loadTexture(texName.c_str(), true);
}



///////////////////////////////////////////////////////////////////////////////
// disable spining and reset model's rotation angle
///////////////////////////////////////////////////////////////////////////////
void ModelGL::disableSpin()
{
    modelAngle.set(0,0,0);
    updateModelMatrix();
    drawModeChanged = true;
    spinEnabled = false;
}



///////////////////////////////////////////////////////////////////////////////
// create VBOs
///////////////////////////////////////////////////////////////////////////////
bool ModelGL::createVertexBufferObjects()
{
    if(obj.getVertexCount() <= 0)
        return false;

    if(!vboVertex)
        glGenBuffers(1, &vboVertex);

    // setup vertex VBO
    const float* interleavedVertices = obj.getInterleavedVertices();
    unsigned int dataSize = obj.getInterleavedVertexSize();
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
    glBufferData(GL_ARRAY_BUFFER, dataSize, interleavedVertices, GL_STATIC_DRAW);

    // create VBO array for indices
    if(vboIndices.size() > 0)
    {
        // delete prev buffer
        glDeleteBuffers((GLsizei)vboIndices.size(), &vboIndices[0]);
        vboIndices.clear();
    }

    int count = obj.getGroupCount();
    vboIndices.resize(count);
    glGenBuffers(count, &vboIndices[0]);

    // setup vbos for indices
    for(int i = 0; i < count; ++i)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndexCount(i)*sizeof(int), (void*)obj.getIndices(i), GL_STATIC_DRAW);
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////
// create glsl programs
///////////////////////////////////////////////////////////////////////////////
bool ModelGL::createShaderPrograms()
{
    // create 1st shader and program
    GLuint vsId1 = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsId1 = glCreateShader(GL_FRAGMENT_SHADER);
    progId1 = glCreateProgram();

    // load shader sources: flat shader
    glShaderSource(vsId1, 1, &vsSource1, NULL);
    glShaderSource(fsId1, 1, &fsSource1, NULL);

    // compile shader sources
    glCompileShader(vsId1);
    glCompileShader(fsId1);

    // attach shaders to the program
    glAttachShader(progId1, vsId1);
    glAttachShader(progId1, fsId1);

    // link program
    glLinkProgram(progId1);

    // create 2nd shader and program
    GLuint vsId2 = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsId2 = glCreateShader(GL_FRAGMENT_SHADER);
    progId2 = glCreateProgram();

    // load shader sources:
    glShaderSource(vsId2, 1, &vsSource2, NULL);
    glShaderSource(fsId2, 1, &fsSource2, NULL);

    // compile shader sources
    glCompileShader(vsId2);
    glCompileShader(fsId2);

    // attach shaders to the program
    glAttachShader(progId2, vsId2);
    glAttachShader(progId2, fsId2);

    // link program
    glLinkProgram(progId2);

    // 3rd shader and program
    GLuint vsId3 = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsId3 = glCreateShader(GL_FRAGMENT_SHADER);
    progId3 = glCreateProgram();

    // load shader sources:
    glShaderSource(vsId3, 1, &vsSource3, NULL);
    glShaderSource(fsId3, 1, &fsSource3, NULL);

    // compile shader sources
    glCompileShader(vsId3);
    glCompileShader(fsId3);

    // attach shaders to the program
    glAttachShader(progId3, vsId3);
    glAttachShader(progId3, fsId3);

    // link program
    glLinkProgram(progId3);

    glUseProgram(progId3);
    uniformMap0 = glGetUniformLocation(progId3, "map0");

    // check status
    int linkStatus1, linkStatus2, linkStatus3;
    glGetProgramiv(progId1, GL_LINK_STATUS, &linkStatus1);
    glGetProgramiv(progId2, GL_LINK_STATUS, &linkStatus2);
    glGetProgramiv(progId3, GL_LINK_STATUS, &linkStatus3);
    if(linkStatus1 == GL_TRUE && linkStatus2 == GL_TRUE && linkStatus3 == GL_TRUE)
    {
        glslReady = true;
    }
    else
    {
        glslReady = false;
    }

    return glslReady;
}



///////////////////////////////////////////////////////////////////////////////
// save screen captured images
///////////////////////////////////////////////////////////////////////////////
void ModelGL::startSaveImage(const std::string fileName, CallbackResult callback, void* callbackObj)
{
    callbackCaptureSaved = callback;
    callee = callbackObj;
    captureName = fileName;
    captureEnabled = true;
}



///////////////////////////////////////////////////////////////////////////////
// draw to FBO
///////////////////////////////////////////////////////////////////////////////
void ModelGL::drawToFrameBuffer()
{
    fbo.bind();

    setViewport(0, 0, fbo.getWidth(), fbo.getHeight());
    //glViewport(0, 0, (GLsizei)imageWidth, (GLsizei)imageHeight);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // pass projection matrix to OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrixProjection.get());
    glMatrixMode(GL_MODELVIEW);

    // draw grid and axis
    if(gridEnabled)
    {
        //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        //glEnable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);

        // use flat shading
        glLoadMatrixf(matrixView.get());
        drawGrid(gridSize, gridStep);
        drawAxis(gridSize);

        //glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
    }

    // draw OBJ mesh
    glLoadMatrixf(matrixModelView.get());
    if(!objLoading)
    {
        if(vboSupported)
            drawObjWithVbo();
        else
            drawObj();

        // draw selected group with wireframe
        if(selectedGroupId >= 0 && selectedGroupId < (int)obj.getGroupCount())
            drawSelectedGroup();

        // draw bbox
        if(boundingBoxEnabled)
            drawBoundingBox();
    }

    glFinish();

    // copy colorbuffer to array
    fbo.copyColorBuffer();

    fbo.unbind();
}



///////////////////////////////////////////////////////////////////////////////
// toggle black background
///////////////////////////////////////////////////////////////////////////////
void ModelGL::enableBlackBackground()
{
    bgColor.set(0, 0, 0, 0);
}
void ModelGL::disableBlackBackground()
{
    bgColor.set(1, 1, 1, 0);
}



///////////////////////////////////////////////////////////////////////////////
// update capture fbo size
///////////////////////////////////////////////////////////////////////////////
void ModelGL::setCaptureSize(int size)
{
    captureWidth = captureHeight = size;
    captureSizeChanged = true;
}
