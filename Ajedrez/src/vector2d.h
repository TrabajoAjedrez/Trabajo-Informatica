#pragma once
class Vector2D {
public:
	int x,y;

public:
	Vector2D(int x, int y) : x(x), y(y){};
	Vector2D() = default;

	bool operator==(const Vector2D& other) const {
		return x == other.x && y == other.y;
	}
};