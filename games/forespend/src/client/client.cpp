#include "client.h"

#include "scenes/mainmenu.h"

client::client() {
	
}
void client::boot() {
	this->framerate=60;
	this->tickrate=20;
//	SetConfigFlags();
	InitWindow(380,240,"forespend - 0.03h");
	initconfigmenu();
	this->currentscene=new mainmenu();
	this->currentscene->boot();
	target=LoadRenderTexture(380,240);
}
void client::draw() {
	BeginDrawing();
	ClearBackground(WHITE);
	this->currentscene->draw();
	EndDrawing();
}
void client::exit() {
	this->currentscene->exit();
	delete this->currentscene;
}
void client::tick() {
	this->currentscene->tick();
}



