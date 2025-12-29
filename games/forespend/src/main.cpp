#include <atomic>
#include <incmgr.h>
#include <thread>

#include "server/server.h"
#include "client/client.h"
PLATFORM platform=LINUX;
std::string androidpackage="kn.kinfuyuki.forespend";
inline const char* COMMONCONFIG(){return "common.tdf";}
tiny::ErrorLevel tiny::level{4};
int main(int argc, char** argv) {
	enginend::program* game;
	tiny::startup("forespend","0.03g-rewrite");
	bool isserver = false;
	if (argc>1) {
		for (int i=1;i<argc;i++) {
			if (argv[i]=="server ") {
				isserver = true;
			}
		}
	}
	if (isserver) game = new server();else game = new client();
	game->boot();
	std::atomic<bool> running{true};
	std::thread tickthread([game, &running]() {
		double tickrate=1.0/game->tickrate;
		auto lasttick=std::chrono::high_resolution_clock::now();
		
		while (running) {
			auto now=std::chrono::high_resolution_clock::now();
			double elapsed=std::chrono::duration_cast<std::chrono::duration<double>>(now-lasttick).count();

			if (elapsed>=tickrate) {
				game->tick();
				lasttick=now;
			} else {
				std::this_thread::sleep_for(std::chrono::duration<double>(tickrate-elapsed));
			}
		}
	});
	
	double framerate=1.0/game->framerate;
	auto lastframe=std::chrono::high_resolution_clock::now();
	while (!WindowShouldClose()) {
		auto now=std::chrono::high_resolution_clock::now();
		double elapsed=std::chrono::duration_cast<std::chrono::duration<double>>(now-lastframe).count();

		if (elapsed>=framerate) {
			game->draw();
			lastframe=now;
		} else {
			WaitTime(framerate-elapsed);
		}
	}
	game->exit();
	return 0;
}