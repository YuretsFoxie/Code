///////////////////////////////////////////////////////////////////////////////
// ViewTab2.cpp
// ============
// View component of dialog window
//
//  AUTHORL Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-01-09
// UPDATED: 2020-10-26
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ViewTab2.h"
#include "resource.h"
#include "Log.h"
#include "wcharUtil.h"
using namespace Win;



///////////////////////////////////////////////////////////////////////////////
// default ctor
///////////////////////////////////////////////////////////////////////////////
ViewTab2::ViewTab2(ModelGL* model) : model(model), parentHandle(0)
{
}


///////////////////////////////////////////////////////////////////////////////
// default dtor
///////////////////////////////////////////////////////////////////////////////
ViewTab2::~ViewTab2()
{
}



///////////////////////////////////////////////////////////////////////////////
// initialize all controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::initControls(HWND handle)
{
    // remember the handle to parent window
    parentHandle = handle;

    // set all controls
    textObjFile.set(handle, IDC_OBJ);
    textMtlFile.set(handle, IDC_MTL);
    textTexFile.set(handle, IDC_TEX);
    textDimX.set(handle, IDC_DIMX);
    textDimY.set(handle, IDC_DIMY);
    textDimZ.set(handle, IDC_DIMZ);
    labelDimX.set(handle, IDC_LABEL_DIMX);
    labelDimY.set(handle, IDC_LABEL_DIMY);
    labelDimZ.set(handle, IDC_LABEL_DIMZ);
    textArea.set(handle, IDC_AREA);
    labelCenter.set(handle, IDC_LABEL_CENTER);
    textTriangleCount.set(handle, IDC_TRI_COUNT);
    textIndexCount.set(handle, IDC_INDEX_COUNT);
    textVertexCount.set(handle, IDC_VERTEX_COUNT);
    textTexCoordCount.set(handle, IDC_TEX_COUNT);
    listDesc.set(handle, IDC_LIST_DESC);
}



///////////////////////////////////////////////////////////////////////////////
// set the description of OBJ file
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::setDescription()
{
    textObjFile.setText(toWchar(model->getObjName().c_str()));
    textMtlFile.setText(toWchar(model->getMtlName().c_str()));
    textTexFile.setText(toWchar(model->getTextureName().c_str()));

    textTriangleCount.setText(toWchar((long)model->getTriangleCount()));
    textIndexCount.setText(toWchar((long)model->getIndexCount()));
    textVertexCount.setText(toWchar((long)model->getVertexCount()));
    textTexCoordCount.setText(toWchar((long)model->getTexCoordCount()));

    // fill bounding dimension
    std::wstringstream wss;
    wss << std::setprecision(3);
    const BoundingBox& bbox = model->getBoundingBox();
    textDimX.setText(toWchar(bbox.maxX - bbox.minX));
    textDimY.setText(toWchar(bbox.maxY - bbox.minY));
    textDimZ.setText(toWchar(bbox.maxZ - bbox.minZ));

    textArea.setText(toWchar(model->getSurfaceArea()));

    wss << L"(" << bbox.minX << L" ~ " << bbox.maxX << L")";
    labelDimX.setText(wss.str().c_str());
    wss.str(L"");
    wss << L"(" << bbox.minY << L" ~ " << bbox.maxY << L")";
    labelDimY.setText(wss.str().c_str());
    wss.str(L"");
    wss << L"(" << bbox.minZ << L" ~ " << bbox.maxZ << L")";
    labelDimZ.setText(wss.str().c_str());
    wss.str(L"");
    wss << L"Bounding Box Center: (" << bbox.getCenterX() << L", " << bbox.getCenterY() << L", " << bbox.getCenterZ() << L")";
    labelCenter.setText(wss.str().c_str());

    // fill listbox
    int groupCount = model->getGroupCount();
    int materialCount = model->getMaterialCount();
    std::wstring indent = L"    ";
    listDesc.resetContent();

    wss.str(L"");
    wss << L"Group Count: " << groupCount;
    listDesc.addString(wss.str().c_str());

    // limit the numbers to avoid slowing down
    const int MAX_COUNT = 1000;
    if(groupCount > MAX_COUNT)
    {
        groupCount = MAX_COUNT;
        wss.str(L"");
        wss << L"NOTE: Showing only " << MAX_COUNT << " groups...";
        listDesc.addString(wss.str().c_str());
    }

    for(int i = 0; i < groupCount; ++i)
    {
        wss.str(L"");
        wss << indent << i+1 << L": " << toWchar(model->getGroupName(i).c_str());
        listDesc.addString(wss.str().c_str());
    }
    wss.str(L"");
    listDesc.addString(L""); // blank line

    wss.str(L"");
    wss << L"Material Count: " << materialCount;
    listDesc.addString(wss.str().c_str());

    // limit the numbers to avoid slowing down
    if(materialCount > MAX_COUNT)
    {
        materialCount = MAX_COUNT;
        wss.str(L"");
        wss << L"NOTE: Showing only " << MAX_COUNT << " materials...";
        listDesc.addString(wss.str().c_str());
    }

    for(int i = 0; i < materialCount; ++i)
    {
        wss.str(L"");
        wss << indent << i+1 << L": " << toWchar(model->getMaterialName(i).c_str());
        listDesc.addString(wss.str().c_str());
    }

    // unset floating format
    wss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

    // debug
    Win::log(" OBJ File Name: %s", model->getObjName().c_str());
    Win::log(" MTL File Name: %s", model->getMtlName().c_str());
    Win::log(" TEX File Name: %s", model->getTextureName().c_str());
    Win::log("Triangle Count: %d", model->getTriangleCount());
    Win::log("   Index Count: %d", model->getIndexCount());
    Win::log("  Vertex Count: %d", model->getVertexCount());
    Win::log("  Normal Count: %d", model->getNormalCount());
    Win::log("TexCoord Count: %d", model->getTexCoordCount());
    Win::log("   Group Count: %d", model->getGroupCount());
    Win::log("Material Count: %d", model->getMaterialCount());
}



///////////////////////////////////////////////////////////////////////////////
// clear description field
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::clearDescription()
{
    textObjFile.setText(L"");
    textMtlFile.setText(L"");
    textTexFile.setText(L"");

    textDimX.setText(L"0");
    textDimY.setText(L"0");
    textDimZ.setText(L"0");
    labelDimX.setText(L"(0 ~ 0)");
    labelDimY.setText(L"(0 ~ 0)");
    labelDimZ.setText(L"(0 ~ 0)");
    labelCenter.setText(L"Bounding Box Center: (0, 0, 0)");

    textTriangleCount.setText(L"0");
    textIndexCount.setText(L"0");
    textVertexCount.setText(L"0");
    textTexCoordCount.setText(L"0");
    listDesc.resetContent();
}



///////////////////////////////////////////////////////////////////////////////
// handle listbox event
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::selectGroup()
{
    int index = (int)::SendMessage(listDesc.getHandle(), LB_GETCURSEL, 0, 0) - 1;
    model->setSelectedGroup(index);
}



///////////////////////////////////////////////////////////////////////////////
// resize listbox
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::resizeListBox(int parentWidth, int parentHeight)
{
    RECT rect;

    // get parent's position (x,y) in window coords
    ::GetWindowRect(parentHandle, &rect);
    int parentX = rect.left;
    int parentY = rect.top;
    int parentRight = rect.right;
    int parentBottom = rect.bottom;

    // compute new listbox's position, width and height
    ::GetWindowRect(listDesc.getHandle(), &rect);
    int x = rect.left - parentX;
    int y = rect.top - parentY;
    int spacing = parentRight - rect.right;
    int width = rect.right - rect.left;
    int height = (parentBottom - spacing) - rect.top;
    if(height < 0)
        height = 0;

    // resize width & height, but keep the x and y (do not move)
    ::SetWindowPos(listDesc.getHandle(), 0, x, y, width, height, SWP_NOMOVE | SWP_NOZORDER);
}



///////////////////////////////////////////////////////////////////////////////
// enable/disable controls
///////////////////////////////////////////////////////////////////////////////
void ViewTab2::enableControls()
{
    listDesc.enable();
}

void ViewTab2::disableControls()
{
    listDesc.disable();
}
