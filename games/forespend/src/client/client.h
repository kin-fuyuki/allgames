#pragma once
#include <incmgr.h>

class client :public enginend::program{
public:
	
	RenderTexture2D target;
	const char* CONF() final{return "client.tdf";}
	client();
	void boot() override;
	void tick() override;
	void draw() override;
	void exit() override;
	
};