#include <incmgr.h>
#include <raylib.h>
#include <enginend/scenes/node2d.h>
#include "netio.h"
#include <thread>
#include <atomic>
#include <chrono>

using namespace enginend;
using namespace enginend::nodes;
netio nete{};
/*

 */
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
		Vector2 mp=Vector2(GetMousePositionDesktop());
		Vector2 wp=GetWindowPosition();
		Vector2 rp=Vector2{mp.x-wp.x,mp.y-wp.y};
		if (IsMouseButtonDown(0)) {
			if (rp.x>=0&&rp.y>=0&&rp.x<600&&rp.y<300) {
				Color pix=GetImageColor(img, (int)rp.x, (int)(300-rp.y));
				if (pix.a!=0) {
					if (clicked==false) {
						clicked=true;
						relmouse=rp;
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
			int cm=GetCurrentMonitor();
			Vector2 mop=GetMonitorPosition(cm);
			int newwpx=nwp.x-mop.x;
			int newwpy=nwp.y-mop.y;
			int maxwpx=GetMonitorWidth(cm)-GetScreenWidth();
			int maxwpy=GetMonitorHeight(cm)-GetScreenHeight();
			newwpx=newwpx>maxwpx?maxwpx:newwpx<0?0:newwpx;
			newwpy=newwpy>maxwpy?maxwpy:newwpy<0?0:newwpy;
			SetWindowPosition(newwpx,newwpy);
		}
	}
	void draw()override{this->textured::draw();}
	void exit()override{}
private:
	Image img;bool justclicked;bool clicked=false;
	Vector2 relmouse;
};

struct logi: public virtual textfield {
	logi(Texture2D* texture,Color textcol,Color color,float x,float y,float w,float h,Font f,float fs,std::string txt) {
		this->pos=Vector2{x,y};this->size=Vector2{w,h};this->content=txt;
		this->texture=texture;this->txc=textcol;this->c=color;
		this->font=f;
		this->fs=fs;
	}
	void draw() override {
		Vector2 p=pos;
		float spacing=fs/2.0f;
		std::stringstream ss(content);
		std::string line;
		int lines=0;

		while (std::getline(ss, line)) {
			if (lines%2==0) {
				Vector2 charPos=p;
				for (char& c : line) {
					char str[2]={c, '\0'};
					Color fc=Color{255,0,255,255};
					if (c=='/'||c==':') {
						fc=Color{255,255,0,255};
					}

					DrawTextEx(font, str, charPos, (float)fs, spacing, fc);
					charPos.x+=MeasureTextEx(font, str, (float)fs, spacing).x;
				}
			} else {
				DrawTextEx(font, line.c_str(), p, (float)fs, spacing, Color{0,255,255,255});
			}

			p.y+=(fs+2);
			lines++;
		}
	}
};
void playbuttonfunc();
void quit(){exit(0);}
#include <boost/process.hpp>
class launcher:public program {
public:
	Texture2D bg;
	Texture2D buttonfore;
	Texture2D buttonlock;
	Texture2D playbtn[2];
	Texture2D menubtn[3];
	button* buttons[8];
	button* playbutton;
	Texture2D buttonslabel[8];
	Font gamename,changelog,information;
	bool vsync=true;
	RenderTexture2D target;
	text version;
	std::string selectedversion="";
	Image img;
	bool captured=true;
	const char* CONF() final{return "test.tdf";}
	launcher(){};
	void boot() override {
		tickrate=15;
		framerate=15;
		SetConfigFlags(FLAG_WINDOW_UNDECORATED|FLAG_WINDOW_TRANSPARENT|FLAG_WINDOW_TOPMOST);
		tiny::error("is transparent lol");
		InitWindow(600,300,"test");target=LoadRenderTexture(600, 300);
		img=GenImageColor(600, 300, rl::BLANK);
		SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
		this->tickrate=GetMonitorRefreshRate(GetCurrentMonitor());
		bg=LoadTexture("res/launcher.png");
		buttonfore=LoadTexture("res/forebuttonon.png");
		buttonlock=LoadTexture("res/lockbutton.png");
		gamename=LoadFont("res/showcase.ttf");
		changelog=LoadFont("res/log.ttf");
		information=LoadFont("res/info.ttf");
		menubtn[0]=LoadTexture("res/btn.png");
		menubtn[1]=LoadTexture("res/btnhover.png");
		menubtn[2]=LoadTexture("res/btnpress.png");
		selectedversion=nete.currentversion;
		version= text (nullptr,Color{255,255,255,255},Color{0,0,0,0},96,16*17,1,1,information,20,"version: "+selectedversion);
		SetTraceLogLevel(LOG_ERROR);
		buttons[0]=new button(&menubtn[0], {255,255,255,255},468,58,96,16,nullptr);
		buttonslabel[0]=LoadTexture("res/options.png");
		buttons[1]=new button(&menubtn[0], {255,255,255,255},468,58+(18*1),96,16,std::function<void()>([]() {
			OpenURL("http://kosumi.ddns.net:60000/");
		}));
		buttonslabel[1]=LoadTexture("res/website.png");
		buttons[2]=new button(&menubtn[0], {255,255,255,255},468,58+(18*2),96,16,std::function<void()>([]() {
			OpenURL("http://kosumi.ddns.net:60001/");
		}));
		buttonslabel[2]=LoadTexture("res/forums.png");
		buttons[3]=new button(&menubtn[0], {255,255,255,255},468,58+(18*3),96,16,std::function<void()>([]() {
			OpenURL("https://github.com/kin-fuyuki/allgames/issues");
		}));
		buttonslabel[3]=LoadTexture("res/help.png");
		buttons[4]=new button(&menubtn[0], {255,255,255,255},468,58+(18*4),96,16,std::function<void()>([]() {
			nete.download();
		}));
		buttonslabel[4]=LoadTexture("res/update.png");
		buttons[5]=new button(&menubtn[0], {255,255,255,255},468,58+(18*5),96,16,nullptr);
		buttonslabel[5]=LoadTexture("res/verify.png");
		buttons[6]=new button(&menubtn[0], {255,255,255,255},468,58+(18*6)+15,96,16,nullptr);
		buttonslabel[6]=LoadTexture("res/versions.png");
		buttons[7]=new button(&menubtn[0], {255,255,255,255},468,58+(18*7)+17,96,16,std::function<void()>([]() {
			quit();
		}));
		buttonslabel[7]=LoadTexture("res/exit.png");
		playbtn[0]=LoadTexture("res/playoff.png");
		playbtn[1]=LoadTexture("res/playon.png");
		playbutton= new button(&playbtn[0], {255,255,255,255},406,(18*11)+17+9,153,59,std::function<void()>(playbuttonfunc));
		currentscene->nodes=std::list<node*>{
			new background(&bg,0,0,600,300),
			new textured(&buttonfore,3,36,62,62),
			new textured(&buttonlock,3,36+((62+4)*1),62,62),
			new textured(&buttonlock,3,36+((62+4)*2),62,62),
			new textured(&buttonlock,3,36+((62+4)*3),62,62),
			new text(nullptr,Color{0,255,0,255},{0,0,0,0},232,19,1,1,gamename,16,"FORESPEND"),
			buttons[0],new textured(&buttonslabel[0],468,58,96,16),
			buttons[1],new textured(&buttonslabel[1],468,58+(18*1),96,16),
			buttons[2],new textured(&buttonslabel[2],468,58+(18*2),96,16),
			buttons[3],new textured(&buttonslabel[3],468,58+(18*3),96,16),
			buttons[4],new textured(&buttonslabel[4],468,58+(18*4),96,16),
			buttons[5],new textured(&buttonslabel[5],468,58+(18*5),96,16),
			buttons[6],new textured(&buttonslabel[6],468,58+(18*6)+15,96,16),
			buttons[7],new textured(&buttonslabel[7],468,58+(18*7)+17,96,16),
			playbutton,
			new logi(nullptr,Color{255,0,255,255},Color{0,0,0,0},90,58,466,159,changelog,8,nete.changelog),
			new text(nullptr,Color{255,255,255,255},Color{0,0,0,0},96+16,(16*14)-3,1,1,information,20,"downloaded        verified"),
			&version,
		};
		currentscene->boot();
	}
	int buttondelay=0;
	void tick() override {
		if (nete.currentversion!=selectedversion) {
			selectedversion=nete.currentversion;
			this->version.content="version: "+selectedversion;
			tiny::echo("selecting version: %s",selectedversion.c_str());
		}
		{
			for (button* b : buttons) {
				if (b->hover) {
					if (b->pressed) {
						b->texture=&menubtn[2];
						buttondelay=5;
					}else {
						if (b->texture->id==menubtn[2].id) {
							if(buttondelay<0) {
								b->texture=&menubtn[1];
							}
						}else {
							b->texture=&menubtn[1];
						}
					}
				}else {
					if (b->texture->id==menubtn[2].id) {
						if(buttondelay<0) {
							b->texture=&menubtn[0];
						}
					}else {
						b->texture=&menubtn[0];
					}
				}
			}
		}
		{
			if (playbutton->pressed) {
				playbutton->texture=&playbtn[1];
				buttondelay=5;
			}else {
			if(buttondelay<0) {
				playbutton->texture=&playbtn[0];
			}
			}
		}
		currentscene->tick();
		buttondelay--;
	}
	bool changedmenu=true;
	bool itemchanged=true;
	void draw() override {
		BeginTextureMode(target);
		currentscene->draw();
		EndTextureMode();
		BeginDrawing();
		ClearBackground(rl::BLANK);
		DrawTexturePro(target.texture, {0, 0, 600, -300}, {0, 0, 600, 300}, {0, 0}, 0, rl::WHITE);
		EndDrawing();
		Vector2 mp=Vector2(GetMousePositionDesktop());
		Vector2 wp=GetWindowPosition();
		Vector2 rp=Vector2{mp.x-wp.x,mp.y-wp.y};
		if (rp.x>=0&&rp.y>=0&&rp.x<600&&rp.y<300) {
			if (changedmenu) {
				UnloadImage(img);
				img=LoadImageFromTexture(target.texture);
				changedmenu=false;
			}
			Color pix=GetImageColor(img, (int)rp.x, (int)(300-rp.y));
			if (pix.a==0) {
				SetWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
				captured=false;
			} else {
				ClearWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
				captured=true;
			}
		}
	}
	void exit() override {
		currentscene->exit();
	}
};

launcher* launch;
tiny::ErrorLevel tiny::level{8};

int main(int argc, char *argv[]) {
	tiny::startup((char*)"enginend test",(char*)"1.0");
		tiny::echo ("checking if cfg.tdf exists, if not create");
		std::ofstream tdffile("cfg.tdf");
		tdffile.close();
	
	tiny::echo ("read cfg.tdf");
	tiny::TDF_FILE config;config.filepath="cfg.tdf";config.read();
	std::string version;
	tiny::echo ("getting version");
	try {version=config.getstring({"version"});}
	catch (tiny::TDF_ERR e) {version="NULL";}
	if (version!="NULL") {
		nete.currentversion=version;
		config.setstring({"version"},nete.currentversion);
	}
	launcher e;
	launch=&e;
	tiny::echo ("starting net");
	nete.start();
	tiny::echo ("downloading github info");
	nete.github();
	tiny::echo ("starting launcher");
	e.boot();
	std::atomic<bool> running{true};

	std::thread tickthread([&e, &running]() {
		double tickrate=1.0/e.tickrate;
		auto lasttick=std::chrono::high_resolution_clock::now();
		
		while (running) {
			auto now=std::chrono::high_resolution_clock::now();
			double elapsed=std::chrono::duration_cast<std::chrono::duration<double>>(now-lasttick).count();

			if (elapsed>=tickrate) {
				e.tick();
				lasttick=now;
			} else {
				std::this_thread::sleep_for(std::chrono::duration<double>(tickrate-elapsed));
			}
		}
	});

	double framerate=1.0/e.framerate;
	auto lastframe=std::chrono::high_resolution_clock::now();

	while (!WindowShouldClose()) {
		auto now=std::chrono::high_resolution_clock::now();
		double elapsed=std::chrono::duration_cast<std::chrono::duration<double>>(now-lastframe).count();

		if (elapsed>=framerate) {
			e.draw();
			lastframe=now;
		} else {
			WaitTime(framerate-elapsed);
		}
	}

	running=false;
	tickthread.join();

	e.exit();
	config.setstring({"version"},nete.currentversion);
	config.save();
	config.close();
	return 0;
}
void playbuttonfunc() {
	if (nete.currentversion!="NULL") {
		FILE* gameexe = fopen(("forespend/versions/"+nete.currentversion+"/bin/game").c_str(), "r");
		if (gameexe) {
			fclose(gameexe);
			
			std::thread thread([]() {
				boost::process::system("bin/game",boost::process::start_dir=("forespend/versions/"+nete.currentversion));
			});
			exit(0);
		}
		
	}
}
