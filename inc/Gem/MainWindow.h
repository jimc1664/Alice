#ifndef GEM_MAINWINDOW_H
#define GEM_MAINWINDOW_H

#include "Basic.h"
#include "OS/Window.h"
#include "Dis/MainTarget.h"

namespace Gem {

class MainWindow : public OS::Window, public Dis::MainTarget {
public:
	MainWindow(const char *name, const vec2u16 &size, const u32 &flags);
};

}

#endif GEM_MAINWINDOW_H