///////////////////////////////////////////////////////////////////////////////
// ModelGL.h
// =========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2021-10-03
///////////////////////////////////////////////////////////////////////////////

#ifndef MODEL_GL_H
#define MODEL_GL_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>
#include <string>
#include <functional>
#include "glExtension.h"
#include "Matrices.h"
#include "Vectors.h"
#include "ObjModel.h"
#include "BoundingBox.h"
#include "FrameBuffer.h"
#include "geomUtils.h"

// C callback definition
extern "C"
{
    // 1st param is callee ptr, the 2nd is success/failed and the 3rd is the elapsed milliseconds
    typedef void (*CallbackResult)(void* callee, bool, double);
}

/*@@ Use C-style callback for compatibility
// C++ callback definition for loading/saving OBJ
// 1st param is success/failed and the second is the elapsed milliseconds
typedef std::function<void(bool, double)> CallbackResult;
*/



class ModelGL
{
public:
    ModelGL();
    ~ModelGL();

    void init();                            // initialize OpenGL states
    void quit();                            // clean up OpenGL objects
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
    void draw();

    void setMouseLeft(bool flag)            { mouseLeftDown = flag; };
    void setMouseRight(bool flag)           { mouseRightDown = flag; };
    void setMousePosition(int x, int y)     { mouseX = x; mouseY = y; };
    void setDrawMode(int mode);
    void setWindowSize(int width, int height);

    void rotateCamera(int x, int y);
    void zoomCamera(int dist);
    void zoomCameraDelta(float delta);      // for mousewheel
    void shiftCamera(int x, int y);
    void shiftCameraDelta(float dx, float dy);
    void startShiftCamera(float dx, float dy);
    void stopShiftCamera();
    void resetMatrices();

    void setModelX(float x)         { modelPosition.x = x; updateModelMatrix(); }
    void setModelY(float y)         { modelPosition.y = y; updateModelMatrix(); }
    void setModelZ(float z)         { modelPosition.z = z; updateModelMatrix(); }
    void setModelAngleX(float a)    { modelAngle.x = a; updateModelMatrix(); }
    void setModelAngleY(float a)    { modelAngle.y = a; updateModelMatrix(); }
    void setModelAngleZ(float a)    { modelAngle.z = a; updateModelMatrix(); }
    void setModelScaleX(float s)    { modelScale.x = s; updateModelMatrix(); }
    void setModelScaleY(float s)    { modelScale.y = s; updateModelMatrix(); }
    void setModelScaleZ(float s)    { modelScale.z = s; updateModelMatrix(); }
    float getModelX()               { return modelPosition.x; }
    float getModelY()               { return modelPosition.y; }
    float getModelZ()               { return modelPosition.z; }
    float getModelAngleX()          { return modelAngle.x; }
    float getModelAngleY()          { return modelAngle.y; }
    float getModelAngleZ()          { return modelAngle.z; }
    float getModelScaleX()          { return modelScale.x; }
    float getModelScaleY()          { return modelScale.y; }
    float getModelScaleZ()          { return modelScale.z; }

    void startLoadObj(const std::string fileName, CallbackResult callback, void* callee=0);
    void startSaveObj(const std::string& fileName, CallbackResult callback, void* callee=0);
    void startSaveImage(const std::string fileName, CallbackResult callback, void* callee=0);
    void setSelectedGroup(int index)        { selectedGroupId = index; }
    void setCaptureSize(int size);
    int getCaptureSize() const              { return captureWidth; }

    const std::string& getObjName() const   { return obj.getObjFileName(); }
    const std::string& getMtlName() const   { return obj.getMtlFileName(); }
    const std::string& getObjDir() const    { return obj.getObjDirectory(); }
    const std::string& getTextureName() { return obj.getMaterial(0).textureName; }
    const std::string& getCaptureName() const { return captureName; }
    unsigned int getTriangleCount() const   { return obj.getTriangleCount(); }
    unsigned int getIndexCount() const      { return obj.getIndexCount(); }
    unsigned int getVertexCount() const     { return obj.getVertexCount(); }
    unsigned int getNormalCount() const     { return obj.getNormalCount(); }
    unsigned int getTexCoordCount() const   { return obj.getTexCoordCount(); }
    unsigned int getGroupCount() const      { return obj.getGroupCount(); }
    unsigned int getMaterialCount() const   { return obj.getMaterialCount(); }
    std::string getGroupName(int idx)       { return obj.getGroupName(idx); }
    std::string getMaterialName(int i)      { return obj.getMaterialName(i); }
    const BoundingBox& getBoundingBox()     { return obj.getBoundingBox(); }
    const std::string& getObjError() const  { return obj.getErrorMessage(); }
    const std::string getFboStatus() const  { return fbo.getStatus(); }
    int getMaxTextureSize() const           { return maxTextureSize; }
    float getAnisotropy() const             { return anisotropy; }
    bool isFboSupported() const             { return fboSupported; }
    bool isShaderSupported() const          { return glslSupported; }
    bool isShaderReady() const              { return glslReady; }
    bool isVboSupported() const             { return vboSupported; }
    bool isAnisotropicSupported() const     { return anisotropicSupported; }

    void loadDefaultTexture(const std::string& texName);

    // toggle options
    void enableGrid()                       { gridEnabled = true; }
    void enableTexture()                    { textureEnabled = true; drawModeChanged = true; }
    void enableWireframe()                  { drawMode = 1; drawModeChanged = true; }
    void enableSpin()                       { spinEnabled = true; drawModeChanged = true; }
    void enableBoundingBox()                { boundingBoxEnabled = true; }
    void enableBlackBackground();
    void enableDefaultTexture()             { defaultTextureEnabled = true; drawModeChanged = true; }

    void disableGrid()                      { gridEnabled = false; }
    void disableTexture()                   { textureEnabled = false; drawModeChanged = true; }
    void disableWireframe()                 { drawMode = 0; drawModeChanged = true; }
    void disableSpin();
    void disableBoundingBox()               { boundingBoxEnabled = false; }
    void disableBlackBackground();
    void disableDefaultTexture()            { defaultTextureEnabled = false; drawModeChanged = true; }

    // lighting options
    float getAmbient() const                { return defaultAmbient[0]; }
    float getDiffuse() const                { return defaultDiffuse[0]; }
    float getSpecular() const               { return defaultSpecular[0]; }
    void setAmbient(float value);
    void setDiffuse(float value);
    void setSpecular(float value);
    void resetMaterial();

    float getLightPositionX() const         { return lightPosition[0]; }
    float getLightPositionY() const         { return lightPosition[1]; }
    float getLightPositionZ() const         { return lightPosition[2]; }
    void setLightPositionX(float value)     { lightPosition[0] = value; drawModeChanged = true; }
    void setLightPositionY(float value)     { lightPosition[1] = value; drawModeChanged = true; }
    void setLightPositionZ(float value)     { lightPosition[2] = value; drawModeChanged = true; }
    void resetLightPosition();

    // get surface area & volume
    float getVoume() const                  { return meshVolume;  }
    float getSurfaceArea() const            { return meshArea; }


protected:

private:
    // member functions
    void initLights();                              // add a white light ti scene
    void setViewport(int x, int y, int width, int height);
    void preFrame();
    void postFrame();
    void drawObj();
    void drawObjWithVbo();
    void drawSelectedGroup();
    void drawGrid(float size, float step);          // draw a grid on XZ plane
    void drawAxis(float size);
    void drawBoundingBox();
    void drawToFrameBuffer();
    void setFrustum(float l, float r, float b, float t, float n, float f);
    void setFrustum(float fovy, float ratio, float n, float f);
    void setOrthoFrustum(float l, float r, float b, float t, float n=-1, float f=1);
    void updateModelMatrix();
    void updateViewMatrix();
    void setGridSize(float radius);
    bool loadObj(const std::string fileName);
    bool saveObj(const std::string& fileName);
    void loadTextures();
    bool createShaderPrograms();
    bool createVertexBufferObjects();
    void logShaders();

    // members
    int windowWidth;
    int windowHeight;
    bool windowSizeChanged;
    bool drawModeChanged;
    int drawMode;
    bool mouseLeftDown;
    bool mouseRightDown;
    int mouseX;
    int mouseY;
    float nearPlane;
    float farPlane;
    Vector4 bgColor;
    int maxTextureSize;
    bool gridEnabled;
    bool textureEnabled;
    bool spinEnabled;
    bool boundingBoxEnabled;
    bool defaultLightingEnabled;
    bool captureEnabled;
    bool captureSizeChanged;
    bool shiftEnabled;
    bool defaultTextureEnabled;
    Vector3 shiftDelta;

    void* callee;
    CallbackResult callbackObjLoaded;
    CallbackResult callbackObjSaved;
    CallbackResult callbackCaptureSaved;

    float modelRadius;
    float gridSize;
    float gridStep;

    Vector3 cameraPosition;
    Vector3 cameraAngle;
    Vector3 cameraShift;
    Vector3 modelCenter;
    Vector3 modelPosition;
    Vector3 modelAngle;
    Vector3 modelScale;

    // 4x4 transform matrices
    Matrix4 matrixView;
    Matrix4 matrixModel;
    Matrix4 matrixModelView;
    Matrix4 matrixProjection;

    // obj
    ObjModel obj;
    bool objLoading;
    bool objSaving;
    int selectedGroupId;
    std::string objName;
    std::string objNameSave;

    // textures
    std::vector<unsigned int> textures;
    unsigned int defaultTexture;
    bool textureLoading;

    // vbo extensions
    bool vboSupported;
    GLuint vboVertex;               // vbo for OBJ vertices
    std::vector<GLuint> vboIndices; // vbo for OBJ indices

    // glsl extensions
    bool glslSupported;
    bool glslReady;
    GLuint progId1;                 // shader program with color
    GLuint progId2;                 // shader program with color + lighting
    GLuint progId3;                 // shader program with texture + lighting
    GLuint uniformMap0;             // uniform for texture object

    // default material
    float defaultAmbient[4];
    float defaultDiffuse[4];
    float defaultSpecular[4];
    float defaultShininess;

    // light
    float lightPosition[4];         // light in eye space

    // FBO
    bool fboSupported;
    int captureWidth;
    int captureHeight;
    FrameBuffer fbo;
    std::string captureName;

    // anisotropic extension
    bool anisotropicSupported;
    float anisotropy;

    // volume & surface area
    float meshVolume;               // volume for closed geometry
    float meshArea;                 // surface area
};
#endif
