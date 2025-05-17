#include <iostream>

using namespace std;


struct punto {
	float x_ = 0.0;
	float y_ = 0.0;
};

//BASE JERARQUIA



class figura {
	string nombre = "figura";

	friend ostream& operator<<(ostream& os, const figura& f) {
		os << f.nombre << endl;
		return os;
	}
public:
	void centrar(punto p) const {
		this->dibujar(p);
	}

public:
//	figura() = delete;
	virtual const figura& dibujar(punto p) const{
		cout << "dibujando figura en (" << p.x_ << "," << p.y_ << ")"<<endl;
		return *this;
	}
};

class cuadrado : public figura {
protected:
	const cuadrado& dibujar(punto p)const override final{
		cout << "dibujando cuadrado en (" << p.x_ << "," << p.y_ << ")" << endl;
		return *this;
	}
};

class triangulo : public figura {
protected:
	const triangulo& dibujar (punto p)const override final{
		cout << "dibujando triangulo en (" << p.x_ << "," << p.y_ << ")" << endl;
		return *this;
	}
};


//gestor
void gestor_dibujos(const figura& f, punto p) {
	f.dibujar(p);
};

//caso de uso
int main() {
	figura f;
	f.centrar({5,6});

	cuadrado c;
	c.centrar({ 5,6 });
	
}