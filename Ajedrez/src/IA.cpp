#include "IA.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <deque>
#include <set>

std::deque<std::pair<Vector2D, Vector2D>> historialTactico; // Para versiones 3 y 4 IA táctica

/*IA::IA(TipoIA tipoSeleccionado) : tipo(tipoSeleccionado) {
	if (tipo == TipoIA::FIJA) {


		movimientos = {
			// Apertura: avance de peones
			{{1, 0}, {2, 0}}, // Peón rojo
			{{1, 1}, {3, 1}}, // Peón rojo doble
			{{1, 2}, {2, 2}}, // Peón rojo
			{{1, 3}, {2, 3}}, // Peón rojo

			// Desarrollo de piezas
			{{0, 3}, {1, 3}}, // Torre roja
			{{0, 1}, {1, 2}}, // Alfil rojo

			// Capturas simuladas (suponiendo piezas azules en esas posiciones)
			{{1, 2}, {2, 3}}, // Peón rojo captura en diagonal
			{{1, 1}, {2, 2}}, // Peón rojo captura en diagonal
			{{0, 1}, {2, 3}}, // Alfil rojo captura
			{{3, 3}, {4, 3}}, // Torre roja avanza
			{{4, 3}, {4, 2}}, // Torre roja captura lateral

			// Medio juego
			{{2, 0}, {3, 0}}, // Peón rojo avanza
			{{3, 0}, {4, 0}}, // Peón rojo avanza
			{{4, 0}, {5, 1}}, // Peón rojo captura en diagonal
			{{1, 3}, {3, 3}}, // Torre roja avanza
			{{3, 3}, {3, 1}}, // Torre roja captura lateral

			// Final de partida
			{{5, 1}, {6, 2}}, // Peón rojo avanza
			{{6, 2}, {7, 2}}, // Peón rojo llega a promoción
			{{3, 1}, {4, 1}}, // Peón rojo avanza
			{{4, 1}, {5, 1}}, // Peón rojo avanza
			{{5, 1}, {6, 1}}, // Peón rojo avanza
			{{6, 1}, {7, 1}}, // Peón rojo promoción

			// Posiciones de ataque
			{{0, 3}, {2, 3}}, // Torre roja avanza
			{{2, 3}, {2, 1}}, // Torre roja captura
		};


	}
	// Otros tipos de IA se implementarán más adelante
}*/

bool IA::ejecutarMovimiento(ClassTablero* tablero, ClassReglas& reglas) {
	switch (tipo) {
	case TipoIA::Agresiva:    return movimientoAgresivo(tablero, reglas);
	case TipoIA::Defensiva:   return movimientoDefensivo(tablero, reglas);
	case TipoIA::Tactica:     return movimientoTactico(tablero, reglas);
	case TipoIA::Aleatoria:   return movimientoAleatorio(tablero, reglas);
	case TipoIA::Adaptativa:  return movimientoAdaptativo(tablero, reglas);
	default:                  return false;
	}
}

TipoIA IA::elegirEstrategiaAleatoria() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 4); // 5 estrategias (0..4)

	// Depuración
	//int valor = dis(gen);
	//std::cout << "[DEBUG] Estrategia aleatoria seleccionada: " << valor << std::endl;

	switch (dis(gen)) {
	case 0: return TipoIA::Agresiva;
	case 1: return TipoIA::Defensiva;
	case 2: return TipoIA::Tactica;
	case 3: return TipoIA::Aleatoria;
	case 4: return TipoIA::Adaptativa;
	default: return TipoIA::Tactica; // Fallback
	}

	
	return static_cast<TipoIA>(dis(gen));
}


bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);
	std::vector<std::pair<Vector2D, Vector2D>> movimientosRey;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosNormales;

	// 1. Buscar capturas con cualquier pieza (incluyendo el rey)
	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() != turno) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							if (tablero->moverPieza(origen, destino)) {
								reglas.set_turno();
								std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	// 2. Si no hay capturas, buscar movimientos normales
	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (!objetivo || objetivo->getColor() != turno) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							if (pieza->getTipo() == ClassPieza::Pieza_t::Rey) {
								movimientosRey.emplace_back(origen, destino);
							}
							else {
								movimientosNormales.emplace_back(origen, destino);
							}
						}
					}
				}
			}
		}
	}

	// 3. Priorizar movimientos normales (no del rey)
	for (const auto& [origen, destino] : movimientosNormales) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 4. Si el rey está en jaque o no hay otras piezas que puedan moverse, permitir mover el rey
	if (reyEnJaque || movimientosNormales.empty()) {
		for (const auto& [origen, destino] : movimientosRey) {
			if (tablero->moverPieza(origen, destino)) {
				reglas.set_turno();
				std::cout << "IA Agresiva movió el rey de " << origen << " a " << destino << std::endl;
				return true;
			}
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}


// Come piezas y el rey solo se mueve en jaque
/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	// 1. Buscar capturas con cualquier pieza (excepto rey si no está en jaque)
	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				if (pieza->getTipo() == ClassPieza::Pieza_t::Rey && !reyEnJaque)
					continue; // No mover el rey si no está en jaque

				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() != turno) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							if (tablero->moverPieza(origen, destino)) {
								reglas.set_turno();
								std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	// 2. Si no hay capturas, permitir mover el rey solo si está en jaque
	if (reyEnJaque) {
		for (int i = 0; i < tablero->getFilas(); ++i) {
			for (int j = 0; j < tablero->getColumnas(); ++j) {
				Vector2D origen(i, j);
				ClassPieza* pieza = tablero->getPieza(origen);

				if (pieza && pieza->getColor() == turno && pieza->getTipo() == ClassPieza::Pieza_t::Rey) {
					auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
					for (const auto& destino : movimientos) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							if (tablero->moverPieza(origen, destino)) {
								reglas.set_turno();
								std::cout << "IA Agresiva movió el rey de " << origen << " a " << destino << " para escapar del jaque." << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	// 3. Si no hay capturas ni rey en jaque, intentar movimiento aleatorio
	return movimientoAleatorio(tablero, reglas);
}*/


// Intenta comer pero no puede mover el rey
/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno && pieza->getTipo() != ClassPieza::Pieza_t::Rey) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() != turno) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							if (tablero->moverPieza(origen, destino)) {
								reglas.set_turno();
								std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	// Si no hay capturas, hacer cualquier movimiento válido
	return movimientoAleatorio(tablero, reglas);
}*/

/*bool IA::movimientoAleatorio(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::pair<Vector2D, Vector2D>> movimientosValidos;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno && pieza->getTipo() != ClassPieza::Pieza_t::Rey) {
				auto posibles = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : posibles) {
					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						movimientosValidos.emplace_back(origen, destino);
					}
				}
			}
		}
	}

	if (!movimientosValidos.empty()) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, movimientosValidos.size() - 1);

		auto [origen, destino] = movimientosValidos[dis(gen)];
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Aleatoria movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Aleatoria no encontró movimientos válidos." << std::endl;
	return false;
}*/

bool IA::movimientoAleatorio(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::pair<Vector2D, Vector2D>> movimientosValidos;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno && pieza->getTipo() != ClassPieza::Pieza_t::Rey) {
				auto posibles = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : posibles) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					// Solo permitir si la casilla está vacía o contiene una pieza enemiga
					if (!objetivo || objetivo->getColor() != turno) {
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							movimientosValidos.emplace_back(origen, destino);
						}
					}
				}
			}
		}
	}

	if (!movimientosValidos.empty()) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, movimientosValidos.size() - 1);

		auto [origen, destino] = movimientosValidos[dis(gen)];
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Aleatoria movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Aleatoria no encontró movimientos válidos." << std::endl;
	return false;
}

bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto esPiezaValiosa = [](ClassPieza::Pieza_t tipo) {
		return tipo == ClassPieza::Pieza_t::Reina || tipo == ClassPieza::Pieza_t::Torre || tipo == ClassPieza::Pieza_t::Alfil;
	};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			   (pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
	};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				bool estaAmenazada = reglas.PosAmenzada(origen, *tablero, pieza);
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Penalizar si deja piezas valiosas en peligro
						for (int x = 0; x < copia.getFilas(); ++x) {
							for (int y = 0; y < copia.getColumnas(); ++y) {
								Vector2D pos(x, y);
								ClassPieza* p = copia.getPieza(pos);
								if (p && p->getColor() == turno && esPiezaValiosa(p->getTipo())) {
									if (reglasTemp.PosAmenzada(pos, copia, p)) {
										score -= 10;
									}
								}
							}
						}

						// Bonificación si mueve una pieza amenazada a un lugar seguro
						if (estaAmenazada && !reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score -= 5;
						}

						// Penalización si captura pero queda en peligro
						if (objetivo && objetivo->getColor() != turno &&
							reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += 15;
						}

						// Bonificación por controlar el centro
						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 2;
						}

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}

	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
			   return std::get<2>(a) < std::get<2>(b);
			});

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Defensiva mejorada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Defensiva mejorada no encontró movimientos seguros." << std::endl;
	return false;
}

//defiende bien el rey , pero no come piezas y repite movimientos sin sentido si no atacas
/*bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto esPiezaValiosa = [](ClassPieza::Pieza_t tipo) {
		return tipo == ClassPieza::Pieza_t::Reina || tipo == ClassPieza::Pieza_t::Torre || tipo == ClassPieza::Pieza_t::Alfil; };

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						// Simular el movimiento
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						bool enJaque = reglasTemp.hayJaque(copia, turno);
						if (enJaque) continue;

						// Evaluar score: penalizar si deja piezas valiosas en peligro
						int score = 0;
						for (int x = 0; x < copia.getFilas(); ++x) {
							for (int y = 0; y < copia.getColumnas(); ++y) {
								Vector2D pos(x, y);
								ClassPieza* p = copia.getPieza(pos);
								if (p && p->getColor() == turno && esPiezaValiosa(p->getTipo())) {
									if (reglasTemp.PosAmenzada(pos, copia, p)) {
										score -= 10;
									}
								}
							}
						}

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}

	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);
			});
		

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Defensiva movió de " << origen << " a " << destino << " con score defensivo: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Defensiva no encontró movimientos seguros." << std::endl;
	return false;
}*/



/*bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::pair<Vector2D, Vector2D>> movimientosSeguros;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esCapturaPropia = objetivo && objetivo->getColor() == turno;
					if (esCapturaPropia) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						// Simular el movimiento para ver si deja en peligro
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (!reglasTemp.hayJaque(copia, turno)) {
							movimientosSeguros.emplace_back(origen, destino);
						}
					}
				}
			}
		}
	}

	if (!movimientosSeguros.empty()) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, movimientosSeguros.size() - 1);

		auto [origen, destino] = movimientosSeguros[dis(gen)];
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Defensiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Defensiva no encontró movimientos seguros." << std::endl;
	return false;
}*/


	/*if (!tablero || tipo != TipoIA::FIJA) return false;

	ClassPieza::Color turno = reglas.getColorTurno();
	if (turno != ClassPieza::Color::ROJO) return false;

	while (indice < movimientos.size()) {
		auto [origen, destino] = movimientos[indice];

		if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
			if (tablero->moverPieza(origen, destino)) {
				reglas.set_turno();
				++indice;
				std::cout << "IA (" << static_cast<int>(tipo) << ") movió de " << origen << " a " << destino << std::endl;
				return true;
			}
		}

		++indice;
	}

	std::cout << "IA (" << static_cast<int>(tipo) << ") no tiene más movimientos válidos." << std::endl;
	return false;*/

	// Cuarta versión IA táctica mejorada

bool IA::movimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int {
		switch (tipo) {
		case ClassPieza::Pieza_t::Peon:    return 1;
		case ClassPieza::Pieza_t::Caballo: return 3;
		case ClassPieza::Pieza_t::Alfil:   return 3;
		case ClassPieza::Pieza_t::Torre:   return 5;
		case ClassPieza::Pieza_t::Reina:   return 9;
		case ClassPieza::Pieza_t::Rey:     return 100;
		default: return 0;
		}
		};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			(pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
		};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Valor de la pieza capturada
						if (objetivo && objetivo->getColor() != turno) {
							score -= valorPieza(objetivo->getTipo()) * 10;
						}

						// Penalización si la pieza que se mueve es más valiosa que la que se captura
						if (objetivo && valorPieza(pieza->getTipo()) > valorPieza(objetivo->getTipo())) {
							score += 5;
						}

						// Penalización si la pieza queda amenazada
						if (reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += valorPieza(pieza->getTipo()) * 5;
						}

						// Bonificación por controlar el centro
						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						// Bonificación por desbloquear piezas
						if (pieza->getTipo() == ClassPieza::Pieza_t::Peon) {
							for (int dx = -1; dx <= 1; ++dx) {
								for (int dy = -1; dy <= 1; ++dy) {
									Vector2D pos = origen + Vector2D(dx, dy);
									if (tablero->esPosicionValida(pos)) {
										ClassPieza* bloqueada = tablero->getPieza(pos);
										if (bloqueada && bloqueada->getColor() == turno &&
											(bloqueada->getTipo() == ClassPieza::Pieza_t::Torre ||
												bloqueada->getTipo() == ClassPieza::Pieza_t::Alfil ||
												bloqueada->getTipo() == ClassPieza::Pieza_t::Reina)) {
											score -= 4;
										}
									}
								}
							}
						}

						// Bonificación por amenazar múltiples piezas enemigas
						int amenazas = 0;
						auto amenazasPosibles = pieza->obtenerMovimientosPosibles(copia);
						for (const auto& pos : amenazasPosibles) {
							ClassPieza* p = copia.getPieza(pos);
							if (p && p->getColor() != turno) amenazas++;
						}
						if (amenazas >= 2) score -= 6;

						// Penalización por repetir movimientos recientes
						if (std::find(historialTactico.begin(), historialTactico.end(), std::make_pair(origen, destino)) != historialTactico.end()) {
							score += 15;
						}

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}

	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);
			});

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			historialTactico.push_back({ origen, destino });
			if (historialTactico.size() > 10) historialTactico.pop_front();
			std::cout << "IA Táctica avanzada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Táctica avanzada no encontró movimientos útiles." << std::endl;
	return false;
}

	// Tercera versión IA táctica mejorada

/*bool IA::movimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int {
		switch (tipo) {
		case ClassPieza::Pieza_t::Peon:    return 1;
		case ClassPieza::Pieza_t::Caballo: return 3;
		case ClassPieza::Pieza_t::Alfil:   return 3;
		case ClassPieza::Pieza_t::Torre:   return 5;
		case ClassPieza::Pieza_t::Reina:   return 9;
		case ClassPieza::Pieza_t::Rey:     return 100;
		default: return 0;
		}
		};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			(pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
		};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Valor de la pieza capturada
						if (objetivo && objetivo->getColor() != turno) {
							score -= valorPieza(objetivo->getTipo()) * 10;
						}

						// Penalización si la pieza que se mueve es más valiosa que la que se captura
						if (objetivo && valorPieza(pieza->getTipo()) > valorPieza(objetivo->getTipo())) {
							score += 5;
						}

						// Penalización si la pieza queda amenazada
						if (reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += valorPieza(pieza->getTipo()) * 5;
						}

						// Bonificación por controlar el centro
						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						// Bonificación por desbloquear piezas (si libera una torre, alfil o reina)
						if (pieza->getTipo() == ClassPieza::Pieza_t::Peon) {
							for (int dx = -1; dx <= 1; ++dx) {
								for (int dy = -1; dy <= 1; ++dy) {
									Vector2D pos = origen + Vector2D(dx, dy);
									if (tablero->esPosicionValida(pos)) {
										ClassPieza* bloqueada = tablero->getPieza(pos);
										if (bloqueada && bloqueada->getColor() == turno &&
											(bloqueada->getTipo() == ClassPieza::Pieza_t::Torre ||
												bloqueada->getTipo() == ClassPieza::Pieza_t::Alfil ||
												bloqueada->getTipo() == ClassPieza::Pieza_t::Reina)) {
											score -= 4;
										}
									}
								}
							}
						}

						// Bonificación por amenazar múltiples piezas enemigas
						int amenazas = 0;
						for (int x = 0; x < copia.getFilas(); ++x) {
							for (int y = 0; y < copia.getColumnas(); ++y) {
								Vector2D pos(x, y);
								ClassPieza* p = copia.getPieza(pos);
								if (p && p->getColor() != turno) {
									auto amenazasPosibles = pieza->obtenerMovimientosPosibles(copia);
									if (std::find(amenazasPosibles.begin(), amenazasPosibles.end(), pos) != amenazasPosibles.end()) {
										amenazas++;
									}
								}
							}
						}
						if (amenazas >= 2) score -= 6;

						// Penalización por repetir movimientos recientes
						if (std::find(historialTactico.begin(), historialTactico.end(), std::make_pair(origen, destino)) != historialTactico.end()) {
							score += 15;
						}

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}
	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);
			});

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			historialTactico.push_back({ origen, destino });
			if (historialTactico.size() > 10) historialTactico.pop_front(); // Limitar historial
			std::cout << "IA Táctica avanzada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Táctica avanzada no encontró movimientos útiles." << std::endl;
	return false;
}*/


	// Segunda versión táctica mejorada (mejor hasta ahora)
/*
bool IA::movimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int {
		switch (tipo) {
		case ClassPieza::Pieza_t::Peon:    return 1;
		case ClassPieza::Pieza_t::Caballo: return 3;
		case ClassPieza::Pieza_t::Alfil:   return 3;
		case ClassPieza::Pieza_t::Torre:   return 5;
		case ClassPieza::Pieza_t::Reina:   return 9;
		case ClassPieza::Pieza_t::Rey:     return 100;
		default: return 0;
		}
		};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			(pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
		};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Valor de la pieza capturada
						if (objetivo && objetivo->getColor() != turno) {
							score -= valorPieza(objetivo->getTipo()) * 10;
						}

						// Penalización si la pieza que se mueve es más valiosa que la que se captura
						if (objetivo && valorPieza(pieza->getTipo()) > valorPieza(objetivo->getTipo())) {
							score += 5;
						}

						// Bonificación por controlar el centro
						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						// Penalización si la pieza queda amenazada
						if (reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += valorPieza(pieza->getTipo()) * 5;
						}

						// Bonificación por aumentar movilidad futura
						int movilidad = 0;
						for (int x = 0; x < copia.getFilas(); ++x) {
							for (int y = 0; y < copia.getColumnas(); ++y) {
								ClassPieza* p = copia.getPieza({ x, y });
								if (p && p->getColor() == turno) {
									movilidad += p->obtenerMovimientosPosibles(copia).size();
								}
							}
						}
						score -= movilidad / 2;

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}

	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);
			});

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Táctica mejorada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Táctica mejorada no encontró movimientos útiles." << std::endl;
	return false;
}*/

// Primera versión táctica
/*bool IA::movimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto esPiezaValiosa = [](ClassPieza::Pieza_t tipo) {
		return tipo == ClassPieza::Pieza_t::Reina ||
			tipo == ClassPieza::Pieza_t::Torre;
		};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			(pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
		};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() == turno) continue;

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Bonificación por capturar pieza enemiga
						if (objetivo && objetivo->getColor() != turno) {
							score -= 5;
						}

						// Penalización si deja piezas valiosas en peligro
						for (int x = 0; x < copia.getFilas(); ++x) {
							for (int y = 0; y < copia.getColumnas(); ++y) {
								Vector2D pos(x, y);
								ClassPieza* p = copia.getPieza(pos);
								if (p && p->getColor() == turno && esPiezaValiosa(p->getTipo())) {
									if (reglasTemp.PosAmenzada(pos, copia, p)) {
										score += 10;
									}
								}
							}
						}

						// Bonificación por controlar el centro
						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						// Bonificación por mover una pieza que estaba bloqueada (opcional, aquí no se puede saber si estaba bloqueada)
						// if (pieza->getTipo() != ClassPieza::Pieza_t::Peon && origen == destino) {
						// 	score += 5;
						// }

						movimientosEvaluados.emplace_back(origen, destino, score);
					}
				}
			}
		}
	}

	if (!movimientosEvaluados.empty()) {
		auto mejor = std::min_element(movimientosEvaluados.begin(), movimientosEvaluados.end(),
			[](const auto& a, const auto& b) {
				return std::get<2>(a) < std::get<2>(b);
			});

		Vector2D origen = std::get<0>(*mejor);
		Vector2D destino = std::get<1>(*mejor);

		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Táctica movió de " << origen << " a " << destino
				<< " con score táctico: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Táctica no encontró movimientos útiles." << std::endl;
	return false;
}*/

// IA adaptativa cuarta versión, utiliza las tres estrategias anteriores y evalúa cuál es la mejor en cada turno

bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();

	struct EvaluacionEstrategia {
		TipoIA tipo;
		int score;
	};

	std::vector<EvaluacionEstrategia> evaluaciones;

	// Evaluar cada estrategia
	int scoreAgresivo = evaluarMovimientoAgresivo(tablero, reglas);
	int scoreDefensivo = evaluarMovimientoDefensivo(tablero, reglas);
	int scoreTactico = evaluarMovimientoTactico(tablero, reglas);

	evaluaciones.push_back({ TipoIA::Agresiva, scoreAgresivo });
	evaluaciones.push_back({ TipoIA::Defensiva, scoreDefensivo });
	evaluaciones.push_back({ TipoIA::Tactica, scoreTactico });

	// Historial para evitar repeticiones (opcional)
	static std::deque<TipoIA> historial;
	const int maxHistorial = 5;

	// Penalizar estrategias repetidas recientemente
	for (auto& eval : evaluaciones) {
		if (std::find(historial.begin(), historial.end(), eval.tipo) != historial.end()) {
			eval.score += 10; // Penalización por repetición
		}
	}

	// Elegir la mejor estrategia
	auto mejor = std::min_element(evaluaciones.begin(), evaluaciones.end(),
		[](const EvaluacionEstrategia& a, const EvaluacionEstrategia& b) {
			return a.score < b.score;
		});

	// Guardar en historial
	historial.push_back(mejor->tipo);
	if (historial.size() > maxHistorial) historial.pop_front();

	std::cout << "[IA Adaptativa] Evaluaciones -> "
		<< "Agresiva: " << scoreAgresivo << ", "
		<< "Defensiva: " << scoreDefensivo << ", "
		<< "Táctica: " << scoreTactico << std::endl;

	switch (mejor->tipo) {
	case TipoIA::Agresiva:
		std::cout << "[IA Adaptativa] Modo elegido: Agresivo" << std::endl;
		return movimientoAgresivo(tablero, reglas);
	case TipoIA::Defensiva:
		std::cout << "[IA Adaptativa] Modo elegido: Defensivo" << std::endl;
		return movimientoDefensivo(tablero, reglas);
	case TipoIA::Tactica:
		std::cout << "[IA Adaptativa] Modo elegido: Táctico" << std::endl;
		return movimientoTactico(tablero, reglas);
	default:
		std::cout << "[IA Adaptativa] Modo elegido: Aleatorio (fallback)" << std::endl;
		return movimientoAleatorio(tablero, reglas);
	}
}

// IA adaptativa tercera versión
/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();

	struct EvaluacionEstrategia {
		TipoIA tipo;
		int score;
	};

	std::vector<EvaluacionEstrategia> evaluaciones;

	// Evaluar cada estrategia sin ejecutar el movimiento
	int scoreAgresivo = evaluarMovimientoAgresivo(tablero, reglas);
	int scoreDefensivo = evaluarMovimientoDefensivo(tablero, reglas);
	int scoreTactico = evaluarMovimientoTactico(tablero, reglas);

	evaluaciones.push_back({ TipoIA::Agresiva, scoreAgresivo });
	evaluaciones.push_back({ TipoIA::Defensiva, scoreDefensivo });
	evaluaciones.push_back({ TipoIA::Tactica, scoreTactico });

	// Elegir la estrategia con mejor puntuación
	auto mejor = std::min_element(evaluaciones.begin(), evaluaciones.end(),
		[](const EvaluacionEstrategia& a, const EvaluacionEstrategia& b) {
		   return a.score < b.score;
		});

	std::cout << "[IA Adaptativa] Evaluaciones -> "
		<< "Agresiva: " << scoreAgresivo << ", "
		<< "Defensiva: " << scoreDefensivo << ", "
		<< "Táctica: " << scoreTactico << std::endl;

	switch (mejor->tipo) {
	case TipoIA::Agresiva:
		std::cout << "[IA Adaptativa] Modo elegido: Agresivo" << std::endl;
		return movimientoAgresivo(tablero, reglas);
	case TipoIA::Defensiva:
		std::cout << "[IA Adaptativa] Modo elegido: Defensivo" << std::endl;
		return movimientoDefensivo(tablero, reglas);
	case TipoIA::Tactica:
		std::cout << "[IA Adaptativa] Modo elegido: Táctico" << std::endl;
		return movimientoTactico(tablero, reglas);
	default:
		std::cout << "[IA Adaptativa] Modo elegido: Aleatorio (fallback)" << std::endl;
		return movimientoAleatorio(tablero, reglas);
	}
}*/



// IA adaptativa segunda versión

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int materialPropio = 0;
	int materialEnemigo = 0;
	int piezasAmenazadas = 0;
	int piezasDefendidas = 0;
	int controlCentro = 0;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int{
		switch (tipo) {
			case ClassPieza::Pieza_t::Peon: return 1;
			case ClassPieza::Pieza_t::Caballo: return 3;
			case ClassPieza::Pieza_t::Alfil: return 3;
			case ClassPieza::Pieza_t::Torre: return 5;
			case ClassPieza::Pieza_t::Reina: return 9;
			default: return 0;
		}
	};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas)  {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			   (pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
	};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D pos(i, j);
			ClassPieza* pieza = tablero->getPieza(pos);
			if (!pieza) continue;

			int valor = valorPieza(pieza->getTipo());

			if (pieza->getColor() == turno) {
				materialPropio += valor;

				if (reglas.PosAmenzada(pos, *tablero, pieza)) {
					piezasAmenazadas++;
				}

				// Verificar si está defendida por otra pieza propia
				for (int x = 0; x < tablero->getFilas(); ++x) {
					for (int y = 0; y < tablero->getColumnas(); ++y) {
						Vector2D defensor(x, y);
						ClassPieza* aliada = tablero->getPieza(defensor);
						if (aliada && aliada->getColor() == turno && defensor != pos) {
							auto movs = aliada->obtenerMovimientosPosibles(*tablero);
							if (std::find(movs.begin(), movs.end(), pos) != movs.end()) {
								piezasDefendidas++;
							}
						}
					}
				}

				if (esCentro(pos, tablero->getFilas(), tablero->getColumnas())) {
					controlCentro++;
				}

			}
			else {
				materialEnemigo += valor;
			}
		}
	}

	bool enJaque = reglas.hayJaque(*tablero, turno);
	int diferenciaMaterial = materialPropio - materialEnemigo;

	std::cout << "[IA Adaptativa] Material: " << diferenciaMaterial
		<< " | Amenazadas: " << piezasAmenazadas
		<< " | Defendidas: " << piezasDefendidas
		<< " | Centro: " << controlCentro
		<< " | En jaque: " << (enJaque ? "Sí" : "No") << std::endl;

	// Decisión basada en evaluación
	if (enJaque || piezasAmenazadas > piezasDefendidas || diferenciaMaterial > 3) {
		std::cout << "[IA Adaptativa] Modo: Defensivo\n";
		return movimientoDefensivo(tablero, reglas);
	}
	else if (diferenciaMaterial < -3 || controlCentro < 2) {
		std::cout << "[IA Adaptativa] Modo: Agresivo\n";
		return movimientoAgresivo(tablero, reglas);
	}
	else {
		std::cout << "[IA Adaptativa] Modo: Táctico\n";
		return movimientoTactico(tablero, reglas);
	}
}*/

// IA adaptativa primera versión

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int materialPropio = 0;
	int materialEnemigo = 0;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int{
		switch (tipo) {
			case ClassPieza::Pieza_t::Peon: return 1;
			case ClassPieza::Pieza_t::Caballo: return 3;
			case ClassPieza::Pieza_t::Alfil: return 3;
			case ClassPieza::Pieza_t::Torre: return 5;
			case ClassPieza::Pieza_t::Reina: return 9;
			default: return 0;
		}
	};

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			ClassPieza* pieza = tablero->getPieza({ i, j });
			if (pieza) {
				int valor = valorPieza(pieza->getTipo());
				if (pieza->getColor() == turno)
					materialPropio += valor;
				else
					materialEnemigo += valor;
			}
		}
	}

	bool enJaque = reglas.hayJaque(*tablero, turno);
	int diferencia = materialPropio - materialEnemigo;

	if (enJaque || diferencia > 3) {
		std::cout << "[IA Adaptativa] Modo: Defensivo\n";
		return movimientoDefensivo(tablero, reglas);
	}
	else if (diferencia < -3) {
		std::cout << "[IA Adaptativa] Modo: Agresivo\n";
		return movimientoAgresivo(tablero, reglas);
	}
	else {
		std::cout << "[IA Adaptativa] Modo: Táctico\n";
		return movimientoTactico(tablero, reglas);
	}
}*/
int IA::evaluarMovimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int mejorScore = 1000; // Valor alto por defecto

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() != turno &&
						ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						int valor = objetivo->getTipo() == ClassPieza::Pieza_t::Rey ? 100 : 10;
						mejorScore = std::min(mejorScore, -valor); // Capturar es bueno
					}
				}
			}
		}
	}

	return mejorScore;
}
int IA::evaluarMovimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int score = 0;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno &&
				reglas.PosAmenzada(origen, *tablero, pieza)) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);
						ClassReglas reglasTemp;
						if (!reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score -= 5; // Mover a lugar seguro es bueno
						}
					}
				}
			}
		}
	}

	return score == 0 ? 10 : score; // Penaliza si no puede defender nada
}
int IA::evaluarMovimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int mejorScore = 1000;

	auto valorPieza = [](ClassPieza::Pieza_t tipo) -> int{
		switch (tipo) {
			case ClassPieza::Pieza_t::Peon: return 1;
			case ClassPieza::Pieza_t::Caballo:
			case ClassPieza::Pieza_t::Alfil: return 3;
			case ClassPieza::Pieza_t::Torre: return 5;
			case ClassPieza::Pieza_t::Reina: return 9;
			case ClassPieza::Pieza_t::Rey: return 100;
			default: return 0;
		}
	};

	auto esCentro = [](const Vector2D& pos, int filas, int columnas) {
		return (pos.x >= filas / 3 && pos.x <= 2 * filas / 3) &&
			(pos.y >= columnas / 3 && pos.y <= 2 * columnas / 3);
		};

for (int i = 0; i < tablero->getFilas(); ++i) {
	for (int j = 0; j < tablero->getColumnas(); ++j) {
		Vector2D origen(i, j);
		ClassPieza* pieza = tablero->getPieza(origen);
		if (pieza && pieza->getColor() == turno) {
			auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
			for (const auto& destino : movimientos) {
				if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) continue;

				ClassTablero copia = *tablero;
				copia.moverPieza(origen, destino);
				ClassReglas reglasTemp;

				int score = 0;
				ClassPieza* objetivo = tablero->getPieza(destino);
				if (objetivo && objetivo->getColor() != turno)
					score -= valorPieza(objetivo->getTipo()) * 10;

				if (esCentro(destino, tablero->getFilas(), tablero->getColumnas()))
					score -= 2;

				if (reglasTemp.PosAmenzada(destino, copia, pieza))
					score += valorPieza(pieza->getTipo()) * 5;

				mejorScore = std::min(mejorScore, score);
			}
		}
	}
}

return mejorScore;
}

