#include "stdafx.h"

#include "Gem/MainWindow.h"

MainWindow::MainWindow(  const char *name, const vec2u16 &size, const u32 &flags ) : Window( name,size,flags ), MainTarget( size, *this )  {

}