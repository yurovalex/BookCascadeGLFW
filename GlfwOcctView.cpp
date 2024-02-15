#include "GlfwOcctView.h"

#include  "modelingOCCT.h"

#include <AIS_Shape.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>

namespace
{
  //! Convert GLFW mouse button into Aspect_VKeyMouse.
  static Aspect_VKeyMouse mouseButtonFromGlfw (int theButton)
  {
    switch (theButton)
    {
      case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
      case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
      case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
    }
    return Aspect_VKeyMouse_NONE;
  }

  //! Convert GLFW key modifiers into Aspect_VKeyFlags.
  static Aspect_VKeyFlags keyFlagsFromGlfw (int theFlags)
  {
    Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
    if ((theFlags & GLFW_MOD_SHIFT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_SHIFT;
    }
    if ((theFlags & GLFW_MOD_CONTROL) != 0)
    {
      aFlags |= Aspect_VKeyFlags_CTRL;
    }
    if ((theFlags & GLFW_MOD_ALT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_ALT;
    }
    if ((theFlags & GLFW_MOD_SUPER) != 0)
    {
      aFlags |= Aspect_VKeyFlags_META;
    }
    return aFlags;
  }
}

// ================================================================
// Function : GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::GlfwOcctView()
{
    myOcct = std::make_unique<ModelingOCCT>(this);
}

// ================================================================
// Function : ~GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::~GlfwOcctView()
{
}

// ================================================================
// Function : toView
// Purpose  :
// ================================================================
GlfwOcctView* GlfwOcctView::toView (GLFWwindow* theWin)
{
  return static_cast<GlfwOcctView*>(glfwGetWindowUserPointer (theWin));
}

// ================================================================
// Function : errorCallback
// Purpose  :
// ================================================================
void GlfwOcctView::errorCallback (int theError, const char* theDescription)
{
  Message::DefaultMessenger()->Send (TCollection_AsciiString ("Error") + theError + ": " + theDescription, Message_Fail);
}

// ================================================================
// Function : run
// Purpose  :
// ================================================================
void GlfwOcctView::run()
{
  initWindow (1366, 768, "glfw occt");
  initViewer();
  if (myView.IsNull())
  {
    return;
  }

  myOcct->Lession2();


  myView->MustBeResized();
  myOcctWindow->Map();
  mainloop();
  cleanup();
}

// ================================================================
// Function : initWindow
// Purpose  :
// ================================================================
void GlfwOcctView::initWindow (int theWidth, int theHeight, const char* theTitle)
{
  glfwSetErrorCallback (GlfwOcctView::errorCallback);
  glfwInit();
  const bool toAskCoreProfile = true;
  if (toAskCoreProfile)
  {
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);

#if defined (__APPLE__)
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  myOcctWindow = new GlfwOcctWindow (theWidth, theHeight, theTitle);
  glfwSetWindowUserPointer       (myOcctWindow->getGlfwWindow(), this);
  // window callback
  glfwSetWindowSizeCallback      (myOcctWindow->getGlfwWindow(), GlfwOcctView::onResizeCallback);
  glfwSetFramebufferSizeCallback (myOcctWindow->getGlfwWindow(), GlfwOcctView::onFBResizeCallback);
  // mouse callback
  glfwSetScrollCallback          (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseScrollCallback);
  glfwSetMouseButtonCallback     (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseButtonCallback);
  glfwSetCursorPosCallback       (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseMoveCallback);
}

// ================================================================
// Function : initViewer
// Purpose  :
// ================================================================
void GlfwOcctView::initViewer()
{
  if (myOcctWindow.IsNull()
   || myOcctWindow->getGlfwWindow() == nullptr)
  {
    return;
  }

  Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver (myOcctWindow->GetDisplay(), false);
  Handle(V3d_Viewer) aViewer = new V3d_Viewer (aGraphicDriver);
  aViewer->SetDefaultLights();
  aViewer->SetLightOn();
  aViewer->SetDefaultTypeOfView (V3d_PERSPECTIVE);
  aViewer->ActivateGrid (Aspect_GT_Rectangular, Aspect_GDM_Lines);
  myView = aViewer->CreateView();
  myView->SetImmediateUpdate (false);
  myView->SetWindow (myOcctWindow, myOcctWindow->NativeGlContext());
  myView->ChangeRenderingParams().ToShowStats = true;
  myContext = new AIS_InteractiveContext (aViewer);
}

Handle(AIS_InteractiveContext) GlfwOcctView::GetContext() const
{
  return myContext;
}


// ================================================================
// Function : mainloop
// Purpose  :
// ================================================================
void GlfwOcctView::mainloop()
{
  while (!glfwWindowShouldClose (myOcctWindow->getGlfwWindow()))
  {
    //glfwPollEvents();// for continuous rendering (immediate return if there are no new events)
    glfwWaitEvents();
    if (!myView.IsNull())
    {
      FlushViewEvents (myContext, myView, true);
    }
  }
}

// ================================================================
// Function : cleanup
// Purpose  :
// ================================================================
void GlfwOcctView::cleanup()
{
  if (!myView.IsNull())
  {
    myView->Remove();
  }
  if (!myOcctWindow.IsNull())
  {
    myOcctWindow->Close();
  }
  glfwTerminate();
}

// ================================================================
// Function : onResize
// Purpose  :
// ================================================================
void GlfwOcctView::onResize (int theWidth, int theHeight)
{
  if (theWidth  != 0
   && theHeight != 0
   && !myView.IsNull())
  {
    myView->Window()->DoResize();
    myView->MustBeResized();
    myView->Invalidate();
    myView->Redraw();
  }
}

// ================================================================
// Function : onMouseScroll
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseScroll (double theOffsetX, double theOffsetY)
{
  if (!myView.IsNull())
  {
    UpdateZoom (Aspect_ScrollDelta (myOcctWindow->CursorPosition(), int(theOffsetY * 8.0)));
  }
}

// ================================================================
// Function : onMouseButton
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseButton (int theButton, int theAction, int theMods)
{
  if (myView.IsNull()) { return; }

  const Graphic3d_Vec2i aPos = myOcctWindow->CursorPosition();
  if (theAction == GLFW_PRESS)
  {
    PressMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
  else
  {
    ReleaseMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
}

// ================================================================
// Function : onMouseMove
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseMove (int thePosX, int thePosY)
{
  const Graphic3d_Vec2i aNewPos (thePosX, thePosY);
  if (!myView.IsNull())
  {
    UpdateMousePosition (aNewPos, PressedMouseButtons(), LastMouseFlags(), false);
  }
}
