#include "s3ePointer.h"

class Input
{    
public:
    int m_X, m_Y;
    bool m_PrevTouched;
    bool m_Touched;
public:
    Input();
    void Update();
    void Reset();
    static void TouchButtonCB(s3ePointerEvent* event);
    static void TouchMotionCB(s3ePointerMotionEvent* event);
    static void MultiTouchButtonCB(s3ePointerTouchEvent* event);
    static void MultiTouchMotionCB(s3ePointerTouchMotionEvent* event);
};

extern Input* g_pInput;