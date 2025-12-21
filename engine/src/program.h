#pragma once
#define CFG this->CONF()
inline const char* COMMONCONFIG();
class program {
public:
	program():client(false){}
	program(bool isclient):client(isclient){}
	virtual const char* CONF()=0;
	const bool client;
	virtual void boot()=0;
	virtual void tick()=0;
	virtual void draw()=0;
	virtual void exit()=0;
};