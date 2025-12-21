#pragma once
#include <string>

#include "nodes.h"
#include<tiny/term.h>

namespace enginend {
    struct node2d :public node {
       Vector2 pos;
       node2d(){}
       node2d(float x,float y):pos(Vector2{x,y}){}
    };
    struct rect :virtual public node2d{
       Vector2 size;
       rect(){}
       rect(float x,float y,float w, float h):size(Vector2{w,h}){
          this->pos=Vector2{x,y};
       }
    };
    struct textured :virtual public rect{
       int ID;
       textured(){}
       textured(int id,float x,float y,float w, float h):ID(id){
          this->pos=Vector2{x,y};this->size=Vector2{w,h};
       }
       void boot()override{}
       void tick()override{}
       void draw()override{DrawTexture(LoadTexture(""),pos.x,pos.y,WHITE);}
       void exit()override{}
    };
    struct colored :virtual public rect{
       Color c;
       colored(){}
       colored(Color color,float x,float y,float w, float h):c(color){
          this->pos=Vector2{x,y};this->size=Vector2{w,h};
       }
       void boot()override{}
       void tick()override{}
       void draw()override{DrawRectangle(pos.x,pos.y,size.x,size.y,c);}
       void exit()override{}
    };
    struct tinted :virtual public colored, virtual public textured{
       tinted(){}
       tinted(int id,Color color,float x,float y,float w, float h):
          node2d(x, y),
          rect(x, y, w, h),
          colored(color, x, y, w, h),
          textured(id, x, y, w, h)
       {}
       void boot()override{this->colored::boot();this->textured::boot();}
       void tick()override{this->colored::tick();this->textured::tick();}
       void draw()override{DrawTexture(LoadTexture(""),pos.x,pos.y,c);}
       void exit()override{this->colored::exit();this->textured::exit();}
    };
    struct text :public tinted {
    protected:
       std::string result;
    public:
       Font font;
       float fontsize;
       Color textcolor;
       std::string content;
       text(){ fontsize = 20; }
       text(int id,Color textcol,Color color,float x,float y,float w,float h,Font f,float fs,std::string txt):
          font(f),fontsize(fs),content(txt)
       {
          this->pos=Vector2{x,y};this->size=Vector2{w,h};this->ID=id;this->c=color;this->textcolor=textcol;
          
          result=content;
          size_t initpos = 0;
          while((initpos = result.find("\n", initpos)) != std::string::npos) {
             result.replace(initpos, 1, "\\n");
             initpos += 2;
          }
       }
       void boot()override{this->tinted::boot();}
       void tick()override {
          this->tinted::tick();
          if (result!=content) {
             result=content;
             size_t initpos = 0;
             while((initpos = result.find("\n", initpos)) != std::string::npos) {
                result.replace(initpos, 1, "\\n");
                initpos += 2;
             }
          }
       }
       void draw()override {
          Vector2 minsize=MeasureTextEx(font, content.c_str(), fontsize, 1);
          Vector2 charsize=MeasureTextEx(font, " ", fontsize, 1);
          float p=charsize.x>charsize.y?charsize.x/minsize.x:charsize.y/minsize.y;
          p=p*2;
          int minh=(minsize.y>size.y)?minsize.y:size.y;
          int minw=(minsize.x>size.x)?minsize.x:size.x;
          DrawRectangle(pos.x-charsize.x,pos.y-charsize.y,minw+p,minh+p,c);
          DrawTextEx(font, content.c_str(), pos, fontsize, 1, textcolor);
       }
       void exit()override{this->tinted::exit();}
    };
    struct button :virtual public tinted{
       std::function<void()> function;
       bool pressed;
       button():pressed(false){}
       button(int id,Color color,float x,float y,float w,float h,std::function<void()> func):function(func),pressed(false){
          this->pos=Vector2{x,y};this->size=Vector2{x,y};this->ID=id;this->c=color;
       }
       void boot()override{this->tinted::boot();}
       void tick()override{
          this->tinted::tick();
          Vector2 mouse=GetMousePosition();
          if(CheckCollisionPointRec(mouse,{pos.x,pos.y,size.x,size.y})&&IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
             if(function)function();
          }
       }
       void draw()override{DrawRectangle(pos.x,pos.y,size.x,size.y,c);}
       void exit()override{this->tinted::exit();}
    };
    struct slider :virtual public tinted{
       float value;
       float minvalue;
       float maxvalue;
       slider():value(0),minvalue(0),maxvalue(1){}
       slider(int id,Color color,float x,float y,float w,float h,float min,float max,float val):value(val),minvalue(min),maxvalue(max){
          this->pos=Vector2{x,y};this->size=Vector2{x,y};this->ID=id;this->c=color;
       }
       void boot()override{this->tinted::boot();}
       void tick()override{
          this->tinted::tick();
          Vector2 mouse=GetMousePosition();
          if(CheckCollisionPointRec(mouse,{pos.x,pos.y,size.x,size.y})&&IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
             float t=(mouse.x-pos.x)/size.x;
             value=minvalue+t*(maxvalue-minvalue);
             if(value<minvalue)value=minvalue;
             if(value>maxvalue)value=maxvalue;
          }
       }
       void draw()override{
          DrawRectangle(pos.x,pos.y,size.x,size.y,DARKGRAY);
          float t=(value-minvalue)/(maxvalue-minvalue);
          DrawRectangle(pos.x,pos.y,size.x*t,size.y,c);
       }
       void exit()override{this->tinted::exit();}
    };
    struct textfield :public text{
       textfield(){}
       textfield(int id,Color textcol,Color color,float x,float y,float w,float h,Font f,float fs,std::string txt):
          text(id,textcol,color,x,y,w,h,f,fs,txt){}
       void boot()override{this->text::boot();}
       void tick()override{this->text::tick();}
       void draw()override{
          Vector2 p=pos;
          float lineheight=fontsize+2;
          std::string line="";
          Vector2 charsize=MeasureTextEx(font, " ", fontsize, 0);
          Vector2 minsize=MeasureTextEx(font, content.c_str(), fontsize, charsize.x/2);
          float po=charsize.x>charsize.y?charsize.x/charsize.y:charsize.y/charsize.x;po=po*5;
          int minh=(minsize.y>size.y)?minsize.y:size.y;
          int minw=(minsize.x>size.x)?minsize.x:size.x;
          DrawRectangle(pos.x-(po/2),pos.y-(po/2),minw+(po*1.1),minh+(po*1.1),c);
          DrawTextEx(font,content.c_str(),p,fontsize,charsize.x/2,this->textcolor);
       }
       void exit()override{this->text::exit();}
    };
    struct textinput :public text{
       bool active;
       int cursorpos;
       textinput():active(false),cursorpos(0){}
       textinput(int id,Color textcol,Color color,float x,float y,float w,float h,Font f,float fs):active(false),cursorpos(0){
          this->pos=Vector2{x,y};this->size=Vector2{x,y};this->ID=id;this->c=color;this->font=f;this->content="";
          this->textcolor=textcol;this->fontsize=fs;
       }
       void boot()override{this->text::boot();}
       void tick()override{
          this->text::tick();
          Vector2 mouse=GetMousePosition();
          if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
             active=CheckCollisionPointRec(mouse,{pos.x,pos.y,size.x,size.y});
          }
          if(active){
             int key=GetCharPressed();
             while(key>0){
                if(key>=32&&key<=125){
                   content+=static_cast<char>(key);
                   cursorpos++;
                }
                key=GetCharPressed();
             }
             if(IsKeyPressed(KEY_BACKSPACE)&&content.length()>0){
                content.pop_back();
                cursorpos--;
             }
          }
       }
       void draw()override{
          this->text::draw();
          if(active)DrawRectangle(pos.x+MeasureText(content.c_str(),fontsize),pos.y,2,fontsize,{0,0,0,127});
       }
       void exit()override{this->text::exit();}
    };
    struct textinputfield :public textfield{
       bool active;
       int cursorpos;
       textinputfield():active(false),cursorpos(0){}
       textinputfield(int id,Color textcol,Color color,float x,float y,float w,float h,Font f,float fs):active(false),cursorpos(0),
          textfield(id,textcol,color,x,y,w,h,f,fs,""){}
       void boot()override{this->textfield::boot();}
       void tick()override{
          this->textfield::tick();
          Vector2 mouse=GetMousePosition();
          if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
             active=CheckCollisionPointRec(mouse,{pos.x,pos.y,size.x,size.y});
          }
          if(active){
             int key=GetCharPressed();
             while(key>0){
                if(key>=32&&key<=125){
                   content+=static_cast<char>(key);
                   cursorpos++;
                }
                key=GetCharPressed();
             }
             if(IsKeyPressed(KEY_BACKSPACE)&&content.length()>0){
                content.pop_back();
                cursorpos--;
             }
             if(IsKeyPressed(KEY_ENTER)){
                content+='\n';
                cursorpos++;
             }
          }
       }
       void draw()override{
          this->textfield::draw();
          if(active){
             Vector2 p=pos;
             float lineheight=fontsize+2;
             std::string line="";
             for(char ch:content){
                if(ch=='\n'){
                   p.y+=lineheight;
                   line="";
                }else{
                   line+=ch;
                }
             }
             DrawRectangle(p.x+MeasureText(line.c_str(),fontsize),p.y,2,fontsize,BLACK);
          }
       }
       void exit()override{this->textfield::exit();}
    };
}