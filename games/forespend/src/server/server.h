#pragma once

#include <incmgr.h>

class server : public enginend::program{
public:
	server();
	const char* CONF() final{return "client.tdf";}
	
	void boot() override;
	void tick() override;
	void draw() override;
	void exit() override;
};