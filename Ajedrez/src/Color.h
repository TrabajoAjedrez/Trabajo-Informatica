#pragma once
#include "freeglut.h"

typedef unsigned char uchar;
class ClassColor
{
public:
	uchar r;
	uchar g;
	uchar b;

	ClassColor();
	ClassColor(uchar r, uchar g, uchar b);
	void pon_color() const {
		glColor3ub(r, g, b);
	}

};

