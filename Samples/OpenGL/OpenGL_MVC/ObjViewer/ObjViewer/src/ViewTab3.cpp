///////////////////////////////////////////////////////////////////////////////
// ViewTab3.cpp
// ============
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-02-05
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ViewTab3.h"
#include "resource.h"
#include "Log.h"
#include "wcharUtil.h"
using namespace Win;

const int SPIN_POS_LOW  =-100000;
const int SPIN_POS_HIGH = 100000;
const int SPIN_ROT_LOW  =-3600;
const int SPIN_ROT_HIGH = 3600;
const int SPIN_SCALE_LOW  = 1;
const int SPIN_SCALE_HIGH = 1000;



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewTab3::ViewTab3(ModelGL* model) : model(model), parentHandle(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewTab3::~ViewTab3()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::initControls(HWND handle)
{
    // remember the handle to parent window
    parentHandle = handle;

    // set all controls
    textPositionX.set(handle, IDC_EDIT_TX);
    textPositionY.set(handle, IDC_EDIT_TY);
    textPositionZ.set(handle, IDC_EDIT_TZ);
    textRotationX.set(handle, IDC_EDIT_RX);
    textRotationY.set(handle, IDC_EDIT_RY);
    textRotationZ.set(handle, IDC_EDIT_RZ);
    textScaleX.set(handle, IDC_EDIT_SX);
    textScaleY.set(handle, IDC_EDIT_SY);
    textScaleZ.set(handle, IDC_EDIT_SZ);

    spinPositionX.set(handle, IDC_SPIN_TX);
    spinPositionX.setBuddy(textPositionX.getHandle());
    spinPositionX.setRange(SPIN_POS_LOW, SPIN_POS_HIGH);
    spinPositionX.setPos(0);
    spinPositionY.set(handle, IDC_SPIN_TY);
    spinPositionY.setBuddy(textPositionY.getHandle());
    spinPositionY.setRange(SPIN_POS_LOW, SPIN_POS_HIGH);
    spinPositionY.setPos(0);
    spinPositionZ.set(handle, IDC_SPIN_TZ);
    spinPositionZ.setBuddy(textPositionZ.getHandle());
    spinPositionZ.setRange(SPIN_POS_LOW, SPIN_POS_HIGH);
    spinPositionZ.setPos(0);
    spinRotationX.set(handle, IDC_SPIN_RX);
    spinRotationX.setBuddy(textRotationX.getHandle());
    spinRotationX.setRange(SPIN_ROT_LOW, SPIN_ROT_HIGH);
    spinRotationX.setPos(0);
    spinRotationY.set(handle, IDC_SPIN_RY);
    spinRotationY.setBuddy(textRotationY.getHandle());
    spinRotationY.setRange(SPIN_ROT_LOW, SPIN_ROT_HIGH);
    spinRotationY.setPos(0);
    spinRotationZ.set(handle, IDC_SPIN_RZ);
    spinRotationZ.setBuddy(textRotationZ.getHandle());
    spinRotationZ.setRange(SPIN_ROT_LOW, SPIN_ROT_HIGH);
    spinRotationZ.setPos(0);
    spinScaleX.set(handle, IDC_SPIN_SX);
    spinScaleX.setBuddy(textScaleX.getHandle());
    spinScaleX.setRange(SPIN_SCALE_LOW, SPIN_SCALE_HIGH);
    spinScaleX.setPos(100);
    spinScaleY.set(handle, IDC_SPIN_SY);
    spinScaleY.setBuddy(textScaleY.getHandle());
    spinScaleY.setRange(SPIN_SCALE_LOW, SPIN_SCALE_HIGH);
    spinScaleY.setPos(100);
    spinScaleZ.set(handle, IDC_SPIN_SZ);
    spinScaleZ.setBuddy(textScaleZ.getHandle());
    spinScaleZ.setRange(SPIN_SCALE_LOW, SPIN_SCALE_HIGH);
    spinScaleZ.setPos(100);

    buttonResetPosition.set(handle, IDC_BUTTON_RESET_T);
    buttonResetRotation.set(handle, IDC_BUTTON_RESET_R);
    buttonResetScale.set(handle, IDC_BUTTON_RESET_S);

    buttonSaveObj.set(handle, IDC_BUTTON_SAVE_OBJ);
    textSaveObj.set(handle, IDC_EDIT_SAVE_OBJ);
}



///////////////////////////////////////////////////////////////////////////////
// change updown position
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::changeUpDownPosition(HWND handle, int position)
{
    if(handle == spinPositionX.getHandle())
    {
        if(position >= SPIN_POS_LOW && position <= SPIN_POS_HIGH)
        {
            textPositionX.setText(toWchar(position * 0.01f));
            model->setModelX(position * 0.01f);
        }
    }
    else if(handle == spinPositionY.getHandle())
    {
        if(position >= SPIN_POS_LOW && position <= SPIN_POS_HIGH)
        {
            textPositionY.setText(toWchar(position * 0.01f));
            model->setModelY(position * 0.01f);
        }
    }
    else if(handle == spinPositionZ.getHandle())
    {
        if(position >= SPIN_POS_LOW && position <= SPIN_POS_HIGH)
        {
            textPositionZ.setText(toWchar(position * 0.01f));
            model->setModelZ(position * 0.01f);
        }
    }
    else if(handle == spinRotationX.getHandle())
    {
        if(position >= SPIN_ROT_LOW && position <= SPIN_ROT_HIGH)
        {
            textRotationX.setText(toWchar(position * 0.1f));
            model->setModelAngleX(position * 0.1f);
        }
    }
    else if(handle == spinRotationY.getHandle())
    {
        if(position >= SPIN_ROT_LOW && position <= SPIN_ROT_HIGH)
        {
            textRotationY.setText(toWchar(position * 0.1f));
            model->setModelAngleY(position * 0.1f);
        }
    }
    else if(handle == spinRotationZ.getHandle())
    {
        if(position >= SPIN_ROT_LOW && position <= SPIN_ROT_HIGH)
        {
            textRotationZ.setText(toWchar(position * 0.1f));
            model->setModelAngleZ(position * 0.1f);
        }
    }
    else if(handle == spinScaleX.getHandle())
    {
        if(position < SPIN_SCALE_LOW)
            position = SPIN_SCALE_LOW;
        else if(position > SPIN_SCALE_HIGH)
            position = SPIN_SCALE_HIGH;

        float value = position * 0.01f;
        textScaleX.setText(toWchar(value));
        model->setModelScaleX(value);
    }
    else if(handle == spinScaleY.getHandle())
    {
        if(position < SPIN_SCALE_LOW)
            position = SPIN_SCALE_LOW;
        else if(position > SPIN_SCALE_HIGH)
            position = SPIN_SCALE_HIGH;

        float value = position * 0.01f;
        textScaleY.setText(toWchar(value));
        model->setModelScaleY(value);
    }
    else if(handle == spinScaleZ.getHandle())
    {
        if(position < SPIN_SCALE_LOW)
            position = SPIN_SCALE_LOW;
        else if(position > SPIN_SCALE_HIGH)
            position = SPIN_SCALE_HIGH;

        float value = position * 0.01f;
        textScaleZ.setText(toWchar(value));
        model->setModelScaleZ(value);
    }
}



///////////////////////////////////////////////////////////////////////////////
// clear text fields
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::resetTransforms()
{
    resetPosition();
    resetRotation();
    resetScale();
}
void ViewTab3::resetPosition()
{
    textPositionX.setText(L"0");
    textPositionY.setText(L"0");
    textPositionZ.setText(L"0");
    spinPositionX.setPos(0);
    spinPositionY.setPos(0);
    spinPositionZ.setPos(0);
    model->setModelX(0);
    model->setModelY(0);
    model->setModelZ(0);
}
void ViewTab3::resetRotation()
{
    textRotationX.setText(L"0");
    textRotationY.setText(L"0");
    textRotationZ.setText(L"0");
    spinRotationX.setPos(0);
    spinRotationY.setPos(0);
    spinRotationZ.setPos(0);
    model->setModelAngleX(0);
    model->setModelAngleY(0);
    model->setModelAngleZ(0);
}
void ViewTab3::resetScale()
{
    textScaleX.setText(L"1");
    textScaleY.setText(L"1");
    textScaleZ.setText(L"1");
    spinScaleX.setPos(100);
    spinScaleY.setPos(100);
    spinScaleZ.setPos(100);
    model->setModelScaleX(1);
    model->setModelScaleY(1);
    model->setModelScaleZ(1);
}






///////////////////////////////////////////////////////////////////////////////
// enable/disable controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::enableControls()
{
    buttonResetPosition.enable();
    buttonResetRotation.enable();
    buttonResetScale.enable();
    buttonSaveObj.enable();

    spinPositionX.enable();
    spinPositionY.enable();
    spinPositionZ.enable();
    spinRotationX.enable();
    spinRotationY.enable();
    spinRotationZ.enable();
    spinScaleX.enable();
    spinScaleY.enable();
    spinScaleZ.enable();
}

void ViewTab3::disableControls()
{
    buttonResetPosition.disable();
    buttonResetRotation.disable();
    buttonResetScale.disable();
    buttonSaveObj.disable();

    spinPositionX.disable();
    spinPositionY.disable();
    spinPositionZ.disable();
    spinRotationX.disable();
    spinRotationY.disable();
    spinRotationZ.disable();
    spinScaleX.disable();
    spinScaleY.disable();
    spinScaleZ.disable();
}



///////////////////////////////////////////////////////////////////////////////
// update model's position
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::changePositionX()
{
    wchar_t buffer[64] = L"\0";
    textPositionX.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelX(value);
    spinPositionX.setPos((int)(value * 100));
}

void ViewTab3::changePositionY()
{
    wchar_t buffer[64] = L"\0";
    textPositionY.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelY(value);
    spinPositionY.setPos((int)(value * 100));
}

void ViewTab3::changePositionZ()
{
    wchar_t buffer[64] = L"\0";
    textPositionZ.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelZ(value);
    spinPositionZ.setPos((int)(value * 100));
}

void ViewTab3::changeAngleX()
{
    wchar_t buffer[64] = L"\0";
    textRotationX.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelAngleX(value);
    spinRotationX.setPos((int)(value * 10));
}

void ViewTab3::changeAngleY()
{
    wchar_t buffer[64] = L"\0";
    textRotationY.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelAngleY(value);
    spinRotationY.setPos((int)(value * 10));
}
void ViewTab3::changeAngleZ()
{
    wchar_t buffer[64] = L"\0";
    textRotationZ.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelAngleZ(value);
    spinRotationZ.setPos((int)(value * 10));
}

void ViewTab3::changeScaleX()
{
    wchar_t buffer[64] = L"\0";
    textScaleX.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelScaleX(value);
    spinScaleX.setPos((int)(value * 100));
}

void ViewTab3::changeScaleY()
{
    wchar_t buffer[64] = L"\0";
    textScaleY.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelScaleY(value);
    spinScaleY.setPos((int)(value * 100));
}

void ViewTab3::changeScaleZ()
{
    wchar_t buffer[64] = L"\0";
    textScaleZ.getText(buffer, 64);
    float value = (float)atof(toChar(buffer));
    model->setModelScaleZ(value);
    spinScaleZ.setPos((int)(value * 100));
}



///////////////////////////////////////////////////////////////////////////////
// update text of obj file
///////////////////////////////////////////////////////////////////////////////
void ViewTab3::setObjFile(std::wstring filename)
{
    textSaveObj.setText(filename.c_str());
}



