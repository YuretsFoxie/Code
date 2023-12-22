///////////////////////////////////////////////////////////////////////////////
// ViewTab1.cpp
// ============
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-31
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ViewTab1.h"
#include "resource.h"
#include "Log.h"
#include "wcharUtil.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewTab1::ViewTab1(ModelGL* model) : model(model), parentHandle(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewTab1::~ViewTab1()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::initControls(HWND handle)
{
    // remember the handle to parent window
    parentHandle = handle;

    // set all controls
    checkGrid.set(handle, IDC_CHECK_GRID);
    checkGrid.check();
    checkTexture.set(handle, IDC_CHECK_TEXTURE);
    checkTexture.check();
    checkWireframe.set(handle, IDC_CHECK_WIREFRAME);
    checkWireframe.uncheck();
    checkDefaultTexture.set(handle, IDC_CHECK_DEFAULT_TEXTURE);
    checkDefaultTexture.uncheck();
    checkBoundingBox.set(handle, IDC_CHECK_BOUNDINGBOX);
    checkBoundingBox.check();
    checkBlackBackground.set(handle, IDC_CHECK_BLACK);
    checkBlackBackground.check();

    // get material
    int ambientPos = (int)(model->getAmbient() * 100);
    int diffusePos = (int)(model->getDiffuse() * 100);
    int specularPos = (int)(model->getSpecular() * 100);

    buttonResetMaterial.set(handle, IDC_BUTTON_RESET_MATERIAL);
    sliderAmbient.set(handle, IDC_SLIDER_AMBIENT);
    sliderAmbient.setRange(0, 100);
    sliderAmbient.setPos(ambientPos);
    sliderDiffuse.set(handle, IDC_SLIDER_DIFFUSE);
    sliderDiffuse.setRange(0, 100);
    sliderDiffuse.setPos(diffusePos);
    sliderSpecular.set(handle, IDC_SLIDER_SPECULAR);
    sliderSpecular.setRange(0, 100);
    sliderSpecular.setPos(specularPos);

    textAmbient.set(handle, IDC_TEXT_AMBIENT);
    textAmbient.setText(toWchar(ambientPos * 0.01f));
    textDiffuse.set(handle, IDC_TEXT_DIFFUSE);
    textDiffuse.setText(toWchar(diffusePos * 0.01f));
    textSpecular.set(handle, IDC_TEXT_SPECULAR);
    textSpecular.setText(toWchar(specularPos * 0.01f));

    // get light position
    int xPos = (int)((model->getLightPositionX() + 1) * 50 + 0.5f);
    int yPos = (int)((model->getLightPositionY() + 1) * 50 + 0.5f);
    int zPos = (int)((model->getLightPositionZ() + 1) * 50 + 0.5f);

    buttonResetLight.set(handle, IDC_BUTTON_RESET_LIGHT);
    sliderLightX.set(handle, IDC_SLIDER_LIGHT_X);
    sliderLightX.setRange(0, 100);
    sliderLightX.setPos(xPos);
    sliderLightY.set(handle, IDC_SLIDER_LIGHT_Y);
    sliderLightY.setRange(0, 100);
    sliderLightY.setPos(yPos);
    sliderLightZ.set(handle, IDC_SLIDER_LIGHT_Z);
    sliderLightZ.setRange(0, 100);
    sliderLightZ.setPos(zPos);

    textLightX.set(handle, IDC_TEXT_LIGHT_X);
    textLightX.setText(toWchar(xPos * 0.02f - 1.0f));
    textLightY.set(handle, IDC_TEXT_LIGHT_Y);
    textLightY.setText(toWchar(yPos * 0.02f - 1.0f));
    textLightZ.set(handle, IDC_TEXT_LIGHT_Z);
    textLightZ.setText(toWchar(zPos * 0.02f - 1.0f));

    comboScreenshot.set(handle, IDC_COMBO_SCREENSHOT);
    comboScreenshot.addString(L"1024 x 1024");
    comboScreenshot.addString(L"2048 x 2048");
    comboScreenshot.addString(L"4096 x 4096");
    comboScreenshot.addString(L"8192 x 8192");
    comboScreenshot.setCurrentSelection(1);
    model->setCaptureSize(2048);

    buttonScreenshot.set(handle, IDC_BUTTON_SCREENSHOT);
    textScreenshotFile.set(handle, IDC_EDIT_SCREENSHOT);

    HINSTANCE instance = ::GetModuleHandle(0);

    buttonLeft.set(handle, IDC_BUTTON_LEFT);
    buttonLeft.setImage(::LoadBitmap(instance, MAKEINTRESOURCE(IDB_BITMAP_LEFT)));
    buttonRight.set(handle, IDC_BUTTON_RIGHT);
    buttonRight.setImage(::LoadBitmap(instance, MAKEINTRESOURCE(IDB_BITMAP_RIGHT)));
    buttonUp.set(handle, IDC_BUTTON_UP);
    buttonUp.setImage(::LoadBitmap(instance, MAKEINTRESOURCE(IDB_BITMAP_UP)));
    buttonDown.set(handle, IDC_BUTTON_DOWN);
    buttonDown.setImage(::LoadBitmap(instance, MAKEINTRESOURCE(IDB_BITMAP_DOWN)));
    buttonResetCamera.set(handle, IDC_BUTTON_RESET_CAMERA);
    buttonResetCamera.setImage(::LoadBitmap(instance, MAKEINTRESOURCE(IDB_BITMAP_RESET)));
}



///////////////////////////////////////////////////////////////////////////////
// reset material track bars
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::resetMaterialTrackBars()
{
    int ambientPos = (int)(model->getAmbient() * 100);
    int diffusePos = (int)(model->getDiffuse() * 100);
    int specularPos = (int)(model->getSpecular() * 100);

    sliderAmbient.setPos(ambientPos);
    sliderDiffuse.setPos(diffusePos);
    sliderSpecular.setPos(specularPos);

    textAmbient.setText(toWchar(ambientPos * 0.01f));
    textDiffuse.setText(toWchar(diffusePos * 0.01f));
    textSpecular.setText(toWchar(specularPos * 0.01f));
}



///////////////////////////////////////////////////////////////////////////////
// reset light position track bars
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::resetLightTrackBars()
{
    int xPos = (int)((model->getLightPositionX() + 1) * 50 + 0.5f);
    int yPos = (int)((model->getLightPositionY() + 1) * 50 + 0.5f);
    int zPos = (int)((model->getLightPositionZ() + 1) * 50 + 0.5f);

    sliderLightX.setPos(xPos);
    sliderLightY.setPos(yPos);
    sliderLightZ.setPos(zPos);

    textLightX.setText(toWchar(xPos * 0.02f - 1.0f));
    textLightY.setText(toWchar(yPos * 0.02f - 1.0f));
    textLightZ.setText(toWchar(zPos * 0.02f - 1.0f));
}



///////////////////////////////////////////////////////////////////////////////
// update text of screenshot file
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::setScreenshotFile(std::wstring filename)
{
    textScreenshotFile.setText(filename.c_str());
}



///////////////////////////////////////////////////////////////////////////////
// toggle on/off options
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::toggleGrid()
{
    if(checkGrid.isChecked())
        model->enableGrid();
    else
        model->disableGrid();
}
void ViewTab1::toggleTexture()
{
    if(checkTexture.isChecked())
    {
        model->enableTexture();
        checkDefaultTexture.enable();
    }
    else
    {
        model->disableTexture();
        checkDefaultTexture.disable();
    }
}
void ViewTab1::toggleWireframe()
{
    if(checkWireframe.isChecked())
        model->enableWireframe();
    else
        model->disableWireframe();
}
void ViewTab1::toggleDefaultTexture()
{
    if(checkDefaultTexture.isChecked())
        model->enableDefaultTexture();
    else
        model->disableDefaultTexture();
}
void ViewTab1::toggleBoundingBox()
{
    if(checkBoundingBox.isChecked())
        model->enableBoundingBox();
    else
        model->disableBoundingBox();
}
void ViewTab1::toggleBlackBackground()
{
    if(checkBlackBackground.isChecked())
        model->enableBlackBackground();
    else
        model->disableBlackBackground();
}



///////////////////////////////////////////////////////////////////////////////
// update trackbars
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::updateTrackbars(HWND handle, int position)
{
    float value = position * 0.01f;   // [0,100] -> [0,1]
    std::wstringstream wss;
    wss << std::setprecision(2);

    if(handle == sliderAmbient.getHandle())
    {
        sliderAmbient.setPos(position);
        wss << value << std::ends;
        textAmbient.setText(wss.str().c_str());
        model->setAmbient(value);
    }
    else if(handle == sliderDiffuse.getHandle())
    {
        sliderDiffuse.setPos(position);
        wss << value << std::ends;
        textDiffuse.setText(wss.str().c_str());
        model->setDiffuse(value);
    }
    else if(handle == sliderSpecular.getHandle())
    {
        sliderSpecular.setPos(position);
        wss << value << std::ends;
        textSpecular.setText(wss.str().c_str());
        model->setSpecular(value);
    }
    else if(handle == sliderLightX.getHandle())
    {
        sliderLightX.setPos(position);
        value = (value * 2.0f) - 1.0f; // [0,1] -> [-1,1]
        wss << value << std::ends;
        textLightX.setText(wss.str().c_str());
        model->setLightPositionX(value);
    }
    else if(handle == sliderLightY.getHandle())
    {
        sliderLightY.setPos(position);
        value = (value * 2.0f) - 1.0f; // [0,1] -> [-1,1]
        wss << value << std::ends;
        textLightY.setText(wss.str().c_str());
        model->setLightPositionY(value);
    }
    else if(handle == sliderLightZ.getHandle())
    {
        sliderLightZ.setPos(position);
        value = (value * 2.0f) - 1.0f; // [0,1] -> [-1,1]
        wss << value << std::ends;
        textLightZ.setText(wss.str().c_str());
        model->setLightPositionZ(value);
    }

    // unset floating format
    wss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

}



///////////////////////////////////////////////////////////////////////////////
// update combobox
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::changeScreenshotSize()
{
    int scale = (int)pow(2, comboScreenshot.getCurrentSelection());
    int size = scale * 1024;
    //Win::log("size: %d", size);
    model->setCaptureSize(size);
}



///////////////////////////////////////////////////////////////////////////////
// enable/disable controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab1::enableControls()
{
    buttonLeft.enable();
    buttonRight.enable();
    buttonUp.enable();
    buttonDown.enable();
    buttonResetCamera.enable();
    buttonScreenshot.enable();
    buttonResetMaterial.enable();
    buttonResetLight.enable();

    checkGrid.enable();
    checkWireframe.enable();
    checkBoundingBox.enable();
    checkBlackBackground.enable();
    if(model->getTexCoordCount() > 0)
    {
        checkTexture.enable();
        checkDefaultTexture.enable();
    }

    sliderAmbient.enable();
    sliderDiffuse.enable();
    sliderSpecular.enable();
    sliderLightX.enable();
    sliderLightY.enable();
    sliderLightZ.enable();

    comboScreenshot.enable();
    buttonScreenshot.enable();
}

void ViewTab1::disableControls()
{
    buttonLeft.disable();
    buttonRight.disable();
    buttonUp.disable();
    buttonDown.disable();
    buttonResetCamera.disable();
    buttonScreenshot.disable();
    buttonResetMaterial.disable();
    buttonResetLight.disable();

    checkGrid.disable();
    checkTexture.disable();
    checkWireframe.disable();
    checkDefaultTexture.disable();
    checkBoundingBox.disable();
    checkBlackBackground.disable();

    sliderAmbient.disable();
    sliderDiffuse.disable();
    sliderSpecular.disable();
    sliderLightX.disable();
    sliderLightY.disable();
    sliderLightZ.disable();

    comboScreenshot.disable();
    buttonScreenshot.disable();
}
