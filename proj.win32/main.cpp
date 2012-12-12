#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "LevelLoader.h"
USING_NS_CC;

extern unsigned int currentLevel;
extern bool overidedByCommandLine;

AppDelegate app;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	int tmp  = atoi(lpCmdLine);
	if (tmp != 0)
	{
		currentLevel = tmp;
		overidedByCommandLine = true;
	}

    //	create the application instance
    //	
	
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();		
    eglView->setFrameSize(780, 560);
	
	HWND handle = eglView->getHWnd();
	SetWindowText(handle, "Gem Matrix");
	
    int ret = CCApplication::sharedApplication()->run();
    return ret;
}
