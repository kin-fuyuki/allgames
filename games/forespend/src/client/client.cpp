#include "client.h"

#include "scenes/mainmenu.h"

client::client() {
	
}
void client::boot() {
	this->framerate=60;
	this->tickrate=20;
	InitWindow(380,240,"forespend - 0.03h");
//	SetConfigFlags();
	this->currentscene=new mainmenu();
	this->currentscene->boot();
	target=LoadRenderTexture(380,240);
}
void client::draw() {
	BeginTextureMode(target);
	this->currentscene->draw();
	EndTextureMode();
	BeginDrawing();
	ClearBackground(WHITE);
	DrawTexturePro(target.texture, {0, 0, 380,240}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
	EndDrawing();
}
void client::exit() {
	this->currentscene->exit();
	delete this->currentscene;
}
void client::tick() {
	this->currentscene->tick();
}



