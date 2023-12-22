///////////////////////////////////////////////////////////////////////////////
// ViewTab2.h
// ==========
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-10-26
///////////////////////////////////////////////////////////////////////////////

#ifndef VIEW_TAB2_H
#define VIEW_TAB2_H

#include <windows.h>
#include "Controls.h"
#include "ModelGL.h"

namespace Win
{
    class ViewTab2
    {
    public:
        ViewTab2(ModelGL* model);
        ~ViewTab2();

        void initControls(HWND handle);         // init all controls
        void setDescription();
        void clearDescription();
        void resizeListBox(int width, int height);
        void selectGroup();
        void enableControls();
        void disableControls();

    protected:

    private:
        ModelGL* model;
        HWND parentHandle;

        // controls
        Win::EditBox textObjFile;
        Win::EditBox textMtlFile;
        Win::EditBox textTexFile;
        Win::EditBox textDimX;
        Win::EditBox textDimY;
        Win::EditBox textDimZ;
        Win::TextBox labelDimX;
        Win::TextBox labelDimY;
        Win::TextBox labelDimZ;
        Win::TextBox textArea;
        Win::TextBox labelCenter;
        Win::EditBox textTriangleCount;
        Win::EditBox textIndexCount;
        Win::EditBox textVertexCount;
        Win::EditBox textTexCoordCount;
        Win::ListBox listDesc;
    };
}

#endif
