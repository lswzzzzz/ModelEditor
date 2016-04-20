#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "CCQApplication.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : public cocos2d::CCQApplication
{
public:
    //AppDelegate();
	AppDelegate(int argc, char *argv[]) : cocos2d::CCQApplication(argc, argv){
		m_ParentHwnd = NULL;
		m_ParentRect.left = 0;
		m_ParentRect.bottom = 640;
		m_ParentRect.right = 960;
		m_ParentRect.top = 0;
	};
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

	void setParent(HWND hwnd, RECT rect);

private:
	HWND m_ParentHwnd;
	RECT m_ParentRect;
};

#endif // _APP_DELEGATE_H_

