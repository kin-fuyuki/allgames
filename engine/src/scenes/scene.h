#include "nodes.h"
#include <list>
struct scene{
	std::list<node> nodes;
    virtual void init() {}
    virtual void render() {}
    virtual void update() {}
    virtual void close() {}
};