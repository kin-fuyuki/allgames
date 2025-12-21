#include "nodes.h"
#include <list>


namespace enginend {
	struct scene{
		std::list<enginend::node*> nodes;
		virtual void boot() {
			for (enginend::node* n : nodes) {
				n->boot();
			}
		}
		virtual void draw() {
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