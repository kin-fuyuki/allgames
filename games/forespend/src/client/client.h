#pragma once
#include <incmgr.h>

class client :public program{
public:
	
	const char* CONF() final{return "client.tdf";}
	client();
	void boot() override;
	void tick() override;
	void draw() override;
	void exit() override;
	
};