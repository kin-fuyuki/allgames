#include "configmenu.h"

#include <enginend/scenes/node2d.h>
Font forefont;

enginend::group mainpage= enginend::group(
	{
		new enginend::nodes::labeledbutton("graphics",nullptr,{255,127,0,255},{0,0,0,0},100,100,0,0,std::function<void()>([]() {
			
		}),forefont,32),
		
	}
);
enginend::group graphics= enginend::group(
	{
		
	}
);
enginend::group sound= enginend::group(
	{
		
	}
);
enginend::group controls= enginend::group(
	{
		
	}
);
void initconfigmenu() {
	forefont=LoadFont(AT("res/fonts/dos.fnt"));
}