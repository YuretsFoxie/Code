///////////////////////////////////////////////////////////////////////////////
// ViewTab3.h
// ==========
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-02-05
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_TAB3_H
#define VIEW_TAB3_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"

namespace Win
{
    class ViewTab3
    {
    public:
        ViewTab3(ModelGL* model);
        ~ViewTab3();

        void initControls(HWND handle);         // init all controls
        void changeUpDownPosition(HWND handle, int position);
        void resetTransforms();
        void enableControls();
        void disableControls();
        void setObjFile(std::wstring filename);

        void resetPosition();
        void resetRotation();
        void resetScale();

        void changePositionX();
        void changePositionY();
        void changePositionZ();
        void changeAngleX();
        void changeAngleY();
        void changeAngleZ();
        void changeScaleX();
        void changeScaleY();
        void changeScaleZ();

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;

        // controls
        Win::EditBox textPositionX;
        Win::EditBox textPositionY;
        Win::EditBox textPositionZ;
        Win::EditBox textRotationX;
        Win::EditBox textRotationY;
        Win::EditBox textRotationZ;
        Win::EditBox textScaleX;
        Win::EditBox textScaleY;
        Win::EditBox textScaleZ;
        Win::UpDownBox spinPositionX;
        Win::UpDownBox spinPositionY;
        Win::UpDownBox spinPositionZ;
        Win::UpDownBox spinRotationX;
        Win::UpDownBox spinRotationY;
        Win::UpDownBox spinRotationZ;
        Win::UpDownBox spinScaleX;
        Win::UpDownBox spinScaleY;
        Win::UpDownBox spinScaleZ;
        Win::Button buttonResetPosition;
        Win::Button buttonResetRotation;
        Win::Button buttonResetScale;
        Win::Button buttonSaveObj;
        Win::EditBox textSaveObj;
    };
}

#endif
