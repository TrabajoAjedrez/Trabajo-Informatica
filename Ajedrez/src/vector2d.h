#pragma once  
class Vector2D {  
public:  
   int x, y;  

public:  
   Vector2D(int x, int y) : x(x), y(y){};  
   Vector2D() = default;  

   // Sobrecarga del operador == para comparar dos objetos Vector2D  
   bool operator==(const Vector2D& other)const;
	    
};