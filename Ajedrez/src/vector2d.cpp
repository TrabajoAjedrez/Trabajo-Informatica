#include "vector2d.h"  
#include "freeglut.h"  

bool Vector2D::operator==(const Vector2D& other) const {  
   return x == other.x && y == other.y;  
}