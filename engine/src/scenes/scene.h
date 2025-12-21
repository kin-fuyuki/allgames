#include "nodes.h"
#include <list>


namespace enginend {
	struct scene{
		std::list<enginend::node*> nodes;
		virtual void boot() {
			int i=0;
			tiny::echo((char*)"initializing scene");
			for (enginend::node* n : nodes) {
				tiny::echo((char*)"initializing object of ID: %i",i++);
				n->boot();
			}
		}
		virtual void draw() {
			
			ClearBackground(BLANK);
			BeginDrawing();
			for (enginend::node* n : nodes) {
				n->draw();
			}
			EndDrawing();
		}
		virtual void tick() {
			for (enginend::node* n : nodes) {
				n->tick();
			}
		}
		virtual void exit() {
			for (enginend::node* n : nodes) {
				n->exit();
			}
		}
	};
}