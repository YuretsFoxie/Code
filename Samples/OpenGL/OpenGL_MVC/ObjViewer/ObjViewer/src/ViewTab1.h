///////////////////////////////////////////////////////////////////////////////
// ViewTab1.h
// ==========
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-01-27
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_TAB1_H
#define VIEW_TAB1_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"

namespace Win
{
    class ViewTab1
    {
    public:
        ViewTab1(ModelGL* model);
        ~ViewTab1();

        void initControls(HWND handle);         // init all controls
        void updateTrackbars(HWND handle, int position);
        void changeScreenshotSize();
        void toggleGrid();
        void toggleTexture();
        void toggleWireframe();
        void toggleDefaultTexture();
        void toggleBoundingBox();
        void toggleBlackBackground();
        void enableControls();
        void disableControls();
        void resetMaterialTrackBars();
        void resetLightTrackBars();
        void setScreenshotFile(std::wstring filename);

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;

        // controls
        Win::CheckBox checkGrid;
        Win::CheckBox checkTexture;
        Win::CheckBox checkWireframe;
        Win::CheckBox checkDefaultTexture;
        Win::CheckBox checkBoundingBox;
        Win::CheckBox checkBlackBackground;

        Win::Trackbar sliderAmbient;
        Win::Trackbar sliderDiffuse;
        Win::Trackbar sliderSpecular;
        Win::Trackbar sliderLightX;
        Win::Trackbar sliderLightY;
        Win::Trackbar sliderLightZ;
        Win::TextBox textAmbient;
        Win::TextBox textDiffuse;
        Win::TextBox textSpecular;
        Win::TextBox textLightX;
        Win::TextBox textLightY;
        Win::TextBox textLightZ;
        Win::Button buttonResetMaterial;
        Win::Button buttonResetLight;

        Win::ComboBox comboScreenshot;
        Win::Button  buttonScreenshot;
        Win::EditBox textScreenshotFile;

        Win::Button buttonLeft;
        Win::Button buttonRight;
        Win::Button buttonUp;
        Win::Button buttonDown;
        Win::Button buttonResetCamera;
    };
}

#endif
