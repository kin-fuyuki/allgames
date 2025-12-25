#pragma once
#include <incmgr.h>
#include <enginend/scenes/node2drelative.h>

class mainmenu :public virtual enginend::scene{
public:
	void boot() override {
		this->nodes=std::list<enginend::nodes::node*>{
			new enginend::nodes::relative::animated(AT("res/images/sky.gif"),0,0,10,10,5)
		};
		enginend::scene::boot();
	}
	void tick() override {
		enginend::scene::tick();
	}
	void draw() override {
		enginend::scene::draw();
	}
	void exit() override {
		enginend::scene::exit();
	}
};
