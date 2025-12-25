#pragma once
#include <incmgr.h>
#include <enginend/scenes/node2drelative.h>
#include <enginend/scenes/node2d.h>

class mainmenu :public virtual enginend::scene{
private:
	Texture2D bg= LoadTexture(AT("res/images/tilesheet.png"));
public:
	void boot() override {
		
		this->nodes=std::list<enginend::nodes::node*>{
			new enginend::nodes::relative::animated(AT("res/images/sky.gif"),0,0,1,1,2),
		};
		enginend::scene::boot();
	}
	void tick() override {
		for (enginend::nodes::node* n : this->nodes) {
			n->tick();
			
		}
	}
	void draw() override {
		for (enginend::nodes::node* n : this->nodes) {
			n->draw();
		}
	}
	void exit() override {
		enginend::scene::exit();
	}
};
