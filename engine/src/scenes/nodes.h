#pragma once
#include "../gr.h"
#include "../aud.h"
#include "../net.h"
#include<tiny/term.h>

namespace enginend {
	struct node{
	public:
	
		virtual void boot()=0;
		virtual void tick()=0;
		virtual void draw()=0;
		virtual void exit()=0;
	};
}