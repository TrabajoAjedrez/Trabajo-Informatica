#pragma once
#include <iostream>

class Vector2D {
public:
	int x, y;

public:
	Vector2D(int x, int y) : x(x), y(y) {};
	Vector2D() = default;

	bool operator==(const Vector2D& other) const {
		return x == other.x && y == other.y;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}
	Vector2D operator + (const Vector2D&) const; //suma de vectores


};

//inline
inline Vector2D Vector2D::operator + (const Vector2D& v) const {
	return { x + v.x , y + v.y }; //sumar vectores
}
