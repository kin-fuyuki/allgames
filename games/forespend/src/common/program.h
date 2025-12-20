#pragma once

class program {
public:
	virtual static const char* CONF;
	static const char* COMMONCONFIG="common.tdf";
	const bool client;
	virtual void boot();
	virtual void tick();
	virtual void draw();
	virtual void exit();
};