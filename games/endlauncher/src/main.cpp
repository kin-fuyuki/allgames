#include <incmgr.h>
#include <raylib.h>
#include <enginend/scenes/node2d.h>
#include "netio.h"
using namespace enginend;
struct background: public virtual textured {
	background(Texture2D* texture,float x,float y,float w, float h) {
		this->pos=Vector2{x,y};this->size=Vector2{w,h};
		this->texture=texture;
		this->justclicked=false;
		
	}
	void boot()override{
		img=LoadImageFromTexture(*this->texture);
	}
	void tick()override {
		
		Vector2 mp = Vector2(GetMousePositionDesktop());
		Vector2 wp = GetWindowPosition();
		Vector2 rp=Vector2{mp.x-wp.x,mp.y-wp.y};
			if (IsMouseButtonDown(0)) {
		if (rp.x >= 0 && rp.y >= 0 && rp.x < 600 && rp.y < 300) {
			Color pix = GetImageColor(img, (int)rp.x, (int)(300 - rp.y));
				if (pix.a != 0) {
					
					if (clicked==false) {
						clicked=true;
						relmouse = rp;
					}
				}
			}
			}else {
				clicked=false;
			}
		if (clicked) {
			
			Vector2 nwp{
				mp.x-relmouse.x, mp.y-relmouse.y
			};
			//todo: implement proper multi-monitor lmaooo
			int it =GetMonitorCount();
			RenderTexture rt;
			for (int i=0;i<it;i++) {
				Vector2 mtp=GetMonitorPosition(i);
				int mtw=GetMonitorWidth(i);
				int mth=GetMonitorHeight(i);
				if (mp.x>=mtp.x && mp.y>=mtp.y && mp.x<mtp.x+mtw && mp.y<mtp.y+mth) {
					SetWindowMonitor(i);
					break;
				}
			}
			int cm=GetCurrentMonitor();
			Vector2 mop=GetMonitorPosition(cm);
			int newwpx=nwp.x-mop.x;
			int newwpy=nwp.y-mop.y;
			int maxwpx=GetMonitorWidth(cm)-GetScreenWidth();
			int maxwpy=GetMonitorHeight(cm)-GetScreenHeight();
			newwpx=newwpx>maxwpx?maxwpx:newwpx;
			newwpy=newwpy>maxwpy?maxwpy:newwpy;
			SetWindowPosition(newwpx,newwpy);
		}
	}
	void draw()override{this->textured::draw();}
	void exit()override{}
private:
	Image img;bool justclicked;bool clicked=false;
	Vector2 relmouse;
};


class test:public program {
public:
	Texture2D bg;
	Texture2D buttonfore;
	Texture2D buttonlock;
	Texture2D playbtn[2];
	Texture2D menubtn[3];
	button* buttons[8];
	Texture2D buttonslabel[8];
	Font gamename;
	bool vsync = true;
	scene s;
	RenderTexture2D target;
	Image img;
	bool captured=true;
	const char* CONF() final{return "test.tdf";}
	test(){};
	void boot() override {
		tickrate=10;
		framerate=10;
		SetConfigFlags(FLAG_VSYNC_HINT|FLAG_WINDOW_UNDECORATED|FLAG_WINDOW_TRANSPARENT);
		tiny::error("is transparent lol");
		InitWindow(600,300,"test");target = LoadRenderTexture(600, 300);
		img = GenImageColor(600, 300, BLANK);
		SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
		this->tickrate=GetMonitorRefreshRate(GetCurrentMonitor());
		bg=LoadTexture("res/launcher.png");
		buttonfore=LoadTexture("res/forebuttonon.png");
		buttonlock=LoadTexture("res/lockbutton.png");
		gamename=LoadFont("res/showcase.ttf");
		menubtn[0]=LoadTexture("res/btn.png");
		menubtn[1]=LoadTexture("res/btnhover.png");
		menubtn[2]=LoadTexture("res/btnpress.png");
		SetTraceLogLevel(LOG_ERROR);
		//468
		//58
		buttons[0]= new button(&menubtn[0], {255,255,255,255},468,58,96,16,nullptr);
		buttonslabel[0]=LoadTexture("res/options.png");
		buttons[1]= new button(&menubtn[0], {255,255,255,255},468,58+(18*1),96,16,nullptr);
		buttonslabel[1]=LoadTexture("res/website.png");
		buttons[2]= new button(&menubtn[0], {255,255,255,255},468,58+(18*2),96,16,nullptr);
		buttonslabel[2]=LoadTexture("res/forums.png");
		buttons[3]= new button(&menubtn[0], {255,255,255,255},468,58+(18*3),96,16,nullptr);
		buttons[4]= new button(&menubtn[0], {255,255,255,255},468,58+(18*4),96,16,nullptr);
		buttonslabel[4]=LoadTexture("res/update.png");
		buttons[5]= new button(&menubtn[0], {255,255,255,255},468,58+(18*5),96,16,nullptr);
		buttonslabel[5]=LoadTexture("res/verify.png");
		buttons[6]= new button(&menubtn[0], {255,255,255,255},468,58+(18*6)+15,96,16,nullptr);
		buttonslabel[6]=LoadTexture("res/versions.png");
		buttons[7]= new button(&menubtn[0], {255,255,255,255},468,58+(18*7)+17,96,16,nullptr);
		buttonslabel[7]=LoadTexture("res/exit.png");
		playbtn[0]=LoadTexture("res/playoff.png");
		playbtn[1]=LoadTexture("res/playon.png");
		s.nodes=std::list<node*>{
			new background(&bg,0,0,600,300),
			new textured(&buttonfore,3,36,62,62),
			new textured(&buttonlock,3,36+((62+4)*1),62,62),
			new textured(&buttonlock,3,36+((62+4)*2),62,62),
			new textured(&buttonlock,3,36+((62+4)*3),62,62),
			new text(nullptr,Color{0,255,0,255},{0,0,0,0},232,19,1,1,gamename,16,"FORESPEND"),
			buttons[0],new textured(&buttonslabel[0],468,58,96,16),
			buttons[1],new textured(&buttonslabel[1],468,58+(18*1),96,16),
			buttons[2],new textured(&buttonslabel[2],468,58+(18*2),96,16),
			buttons[3],//new textured(&buttonslabel[3],468,58+(18*3),96,16),
			buttons[4],new textured(&buttonslabel[4],468,58+(18*4),96,16),
			buttons[5],new textured(&buttonslabel[5],468,58+(18*5),96,16),
			buttons[6],new textured(&buttonslabel[6],468,58+(18*6)+15,96,16),
			buttons[7],new textured(&buttonslabel[7],468,58+(18*7)+17,96,16),
			new button(&playbtn[0], {255,255,255,255},406,(18*11)+17+9,153,59,nullptr),
			
		};
		s.boot();
		
	}
	void tick() override {
		if (shouldtick()) {
			s.tick();
		}
		
	}
	void draw() override {
		BeginTextureMode(target);
		s.draw();
		EndTextureMode();
		
		BeginDrawing();
		ClearBackground(BLANK);
		DrawTexturePro(target.texture, {0, 0, 600, -300}, {0, 0, 600, 300}, {0, 0}, 0, WHITE);
		EndDrawing();
		
		Vector2 mp = Vector2(GetMousePositionDesktop());
		Vector2 wp = GetWindowPosition();
		Vector2 rp=Vector2{mp.x-wp.x,mp.y-wp.y};
		tiny::error("%f %f",rp.x,rp.y);
		
		if (rp.x >= 0 && rp.y >= 0 && rp.x < 600 && rp.y < 300) {
			UnloadImage(img);
			img = LoadImageFromTexture(target.texture);
			Color pix = GetImageColor(img, (int)rp.x, (int)(300 - rp.y));
			if (pix.a == 0) {
				SetWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
				captured = false;
			} else {
				ClearWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
				captured = true;
			}
		}
	}
	void exit() override {
		s.exit();
	}
};

tiny::ErrorLevel tiny::level{6};
int main(int argc, char *argv[]) {
	netio nete{};
	nete.download();
	tiny::startup((char*)"enginend test",(char*)"1.0");
	test e;
	e.boot();
	while (!WindowShouldClose()) {
		e.tick();
		e.draw();
	}
	e.exit();
	return 0;
}