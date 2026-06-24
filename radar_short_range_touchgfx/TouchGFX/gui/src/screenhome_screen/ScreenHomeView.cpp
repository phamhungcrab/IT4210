#include <gui/screenhome_screen/ScreenHomeView.hpp>

extern "C"
{
#include "radar_app.h"
}

ScreenHomeView::ScreenHomeView()
{

}

void ScreenHomeView::setupScreen()
{
    ScreenHomeViewBase::setupScreen();

    /*
     * Vào Home thì dừng radar:
     * - servo về 90 độ
     * - tắt buzzer
     * - tắt LED alert
     */
    RadarApp_Stop();
}

void ScreenHomeView::tearDownScreen()
{
    ScreenHomeViewBase::tearDownScreen();
}
