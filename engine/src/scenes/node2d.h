#include "nodes.h"

struct node2d :public node {
	vec2 pos;
	node2d(){}
	node2d(float x,float y):pos(x,y){}
};
struct rect :virtual public node2d{
	vec2 size;
	rect(){}
	rect(float x,float y,float w, float h):size(w,h){
		this->pos=vec2(x,y);
	}
};
struct textured :virtual public rect{
	int ID;
	textured(){}
	textured(int id,float x,float y,float w, float h):ID(id){
		this->pos=vec2(x,y);this->size=vec2(w,h);
	}
};
struct colored :virtual public rect{
	col c;
	colored(){}
	colored(col color,float x,float y,float w, float h):c(color){
		this->pos=vec2(x,y);this->size=vec2(w,h);
	}
};
struct tinted :virtual public colored, virtual public textured{
	tinted(){}
	
	tinted(int id,col color,float x,float y,float w, float h):
		node2d(x, y),
		rect(x, y, w, h),
		colored(color, x, y, w, h),
		textured(id, x, y, w, h)
	{
		
	}
};
struct text :virtual tinted {
	Font font;
	std::string text;
};