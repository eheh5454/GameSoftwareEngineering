#include "stdafx.h"
#include "Object.h"
#include "Renderer.h"
#include <iostream>
#include "stdlib.h"
#include "ScenceMgr.h"
using namespace std;

Object::Object(float _x, float _y, float _z, float _size, float _r, float _g, float _b, float _a,float speed, float _Life,float _level): arrow_id(0), arrow_time(0.f), team(0), particletime(0), evolutiontime(0), teer(1)
{
	
	x = _x;
	y = _y;
	z = _z;
	size = _size;
	r = _r;
	g = _g;
	b = _b;
	a = _a;
	level = _level;
	type = "";
	//0~1까지의 실수난수를 생성해서 0.5를 빼서 랜덤방향을 생성하고, 방향을 speed에 곱해준다.
	xspeed = speed * (float)((rand() % 1000 + 1) * 0.001 - 0.5f);
	yspeed = speed * (float)((rand() % 1000 + 1) * 0.001 - 0.5f);
	Lifetime = 10000.0f;
	Life = _Life;

}

Object::~Object() {}


float Object::Getx() {
	return x;
}
float Object::Gety() {
	return y;
}
float Object::Getz() {
	return z;
}
float Object::Getsize() {
	return size;
}
float Object::Getr() {
	return r;
}
float Object::Getg() {
	return g;
}
float Object::Getb() {
	return b;
}
float Object::Geta() {
	return a;
}


void Object::Update(float elaspedtime) {
	if (teer == 1 && type == "agumon" && evolutiontime > 10.f) {
		Life += 50.f;
		teer = 2;
		
	}
	if (teer == 2 && evolutiontime > 20.f){
		Life += 50.f;
		teer = 3;
		
	}
	if (type == "agumon") {
		if (y > 200.f) {
			yspeed = 0;
		}
	}
	this->x = x + xspeed * (elaspedtime * 0.001f);
    this->y = y + yspeed * (elaspedtime * 0.001f);
	evolutiontime += elaspedtime * 0.001f;
	
	if (x > 250)
	{
		xspeed = -xspeed;
	}
	if (x < -250)
	{
		xspeed = -xspeed;
	}
	if (y > 400)
	{
		yspeed = -yspeed;
	}
	if (y < -400)
	{
		yspeed = -yspeed;
	}
	//Lifetime -= 0.5f;
}

void Object::bullet_arrow_Update(float elaspedtime)
{
	//elaspedtime을 second로 변환 
	float second = elaspedtime * 0.001f;
	particletime += second;
	this->x = x + xspeed * second;
	this->y = y + yspeed * second;
	//Lifetime -= 0.5f;
}


int Object::Settype(string type)
{
	this->type = type;
	return 0;
}


string Object::Gettype()
{
	return type;
}
