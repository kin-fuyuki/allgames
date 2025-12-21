#include "../gr.h"
#include "../aud.h"
#include "../net.h"
struct node{
public:
	
    virtual void init() {}
    virtual void render() {}
    virtual void update() {}
    virtual void close() {}
};