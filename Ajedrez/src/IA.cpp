#include "IA.h"
#include "historialmovimientos.h"
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
	ClassPieza::Color enemigo = (turno == ClassPieza::Color::AZUL) ? ClassPieza::Color::ROJO : ClassPieza::Color::AZUL;
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					if (esPropia && !reyEnJaque)
						continue;

					int score = 0;

					// Bonificación por captura al paso
					Vector2D casillaEnPasante = tablero->getCasillaObjetivoEnPasante();
					ClassPieza::Color colorPeonEnPasante = tablero->getColorPeonVulnerableEnPasante();

					if (pieza->getTipo() == ClassPieza::Peon &&
						destino == casillaEnPasante &&
						objetivo == nullptr &&
						colorPeonEnPasante != turno) {
						score -= 6;
					}

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					// Bonificación por jaque
					if (reglasTemp.hayJaque(copia, enemigo)) {
						score -= 25;
					}

					// Bonificación por jaque mate
					if (reglasTemp.hayJaqueMate(copia, enemigo)) {
						score -= 100;
					}

					// Penalización por repetición
					if (HistorialMovimientos::esRepetido(origen, destino)) {
						score += 15;
					}

					// Bonificación por capturar pieza enemiga valiosa
					if (objetivo && objetivo->getColor() != turno) {
						switch (objetivo->getTipo()) {
						case ClassPieza::Reina: score -= 20; break;
						case ClassPieza::Torre: score -= 10; break;
						case ClassPieza::Alfil:
						case ClassPieza::Caballo: score -= 5; break;
						case ClassPieza::Peon: score -= 2; break;
						default: break;
						}
					}

					// Penalización leve si es una captura propia (solo permitida en jaque)
					if (esPropia) {
						score += 8;
					}

					movimientosEvaluados.emplace_back(origen, destino, score);
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
		HistorialMovimientos::registrarMovimiento(origen, destino);
		reglas.set_turno();
		std::cout << "IA Agresiva movió de " << origen << " a " << destino
			<< " con score: " << std::get<2>(*mejor) << std::endl;
		return true;
	}
}

std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
return false;
}

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					// Solo permitir capturas propias si estamos en jaque
					if (esPropia && !reyEnJaque)
						continue;

					int score = 0;

					// Bonificación por captura al paso
					Vector2D casillaEnPasante = tablero->getCasillaObjetivoEnPasante();
					ClassPieza::Color colorPeonEnPasante = tablero->getColorPeonVulnerableEnPasante();

					if (pieza->getTipo() == ClassPieza::Peon &&
						destino == casillaEnPasante &&
						objetivo == nullptr &&
						colorPeonEnPasante != turno) {
						score -= 6; // bonificación por captura al paso
					}

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					// Penalizar si el movimiento es repetido
					if (HistorialMovimientos::esRepetido(origen, destino)) {
						score += 15;
					}

					// Bonificación por capturar pieza enemiga valiosa
					if (objetivo && objetivo->getColor() != turno) {
						switch (objetivo->getTipo()) {
						case ClassPieza::Reina: score -= 20; break;
						case ClassPieza::Torre: score -= 10; break;
						case ClassPieza::Alfil:
						case ClassPieza::Caballo: score -= 5; break;
						case ClassPieza::Peon: score -= 2; break;
						default: break;
						}
					}

					// Penalización leve si es una captura propia (solo permitida en jaque)
					if (esPropia) {
						score += 8;
					}

					movimientosEvaluados.emplace_back(origen, destino, score);
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
			HistorialMovimientos::registrarMovimiento(origen, destino);
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					// Solo permitir capturas propias si estamos en jaque
					if (esPropia && !reyEnJaque)
						continue;

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					int score = 0;

					// Penalizar si el movimiento es repetido
					if (HistorialMovimientos::esRepetido(origen, destino)) {
						score += 15;
					}

					// Bonificación por capturar pieza enemiga valiosa
					if (objetivo && objetivo->getColor() != turno) {
						switch (objetivo->getTipo()) {
						case ClassPieza::Reina: score -= 20; break;
						case ClassPieza::Torre: score -= 10; break;
						case ClassPieza::Alfil:
						case ClassPieza::Caballo: score -= 5; break;
						case ClassPieza::Peon: score -= 2; break;
						default: break;
						}
					}

					// Penalización leve si es una captura propia (solo permitida en jaque)
					if (esPropia) {
						score += 8;
					}

					movimientosEvaluados.emplace_back(origen, destino, score);
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
			HistorialMovimientos::registrarMovimiento(origen, destino);
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::pair<Vector2D, Vector2D>> movimientosSalvadores;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosCaptura;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosNormales;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					// Solo permitir capturas propias si estamos en jaque
					if (esPropia && !reyEnJaque)
						continue;

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					if (reyEnJaque) {
						// Si el rey puede capturar una pieza enemiga que lo amenaza y no queda amenazado
						if (pieza->getTipo() == ClassPieza::Rey && objetivo && objetivo->getColor() != turno) {
							if (!reglasTemp.PosAmenzada(destino, copia, copia.getPieza(destino))) {
								movimientosSalvadores.insert(movimientosSalvadores.begin(), { origen, destino });
								continue;
							}
						}

						// Permitir capturas propias si salvan al rey
						movimientosSalvadores.push_back({ origen, destino });
					}
					else if (objetivo && objetivo->getColor() != turno) {
						movimientosCaptura.push_back({ origen, destino });
					}
					else {
						movimientosNormales.push_back({ origen, destino });
					}
				}
			}
		}
	}

	// 1. Si estamos en jaque, priorizar cualquier movimiento que lo evite
	if (!movimientosSalvadores.empty()) {
		auto [origen, destino] = movimientosSalvadores.front();
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva salió del jaque moviendo de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 2. Si no hay jaque, priorizar capturas enemigas
	for (const auto& [origen, destino] : movimientosCaptura) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 3. Si no hay capturas, hacer un movimiento normal
	for (const auto& [origen, destino] : movimientosNormales) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::pair<Vector2D, Vector2D>> movimientosSalvadores;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosCaptura;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosNormales;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					ClassPieza* objetivo = tablero->getPieza(destino);

					if (reyEnJaque) {
						// Si el rey puede capturar una pieza enemiga que lo amenaza y no queda amenazado
						if (pieza->getTipo() == ClassPieza::Rey && objetivo && objetivo->getColor() != turno) {
							if (!reglasTemp.PosAmenzada(destino, copia, copia.getPieza(destino))) {
								// Captura segura del atacante: priorizar
								movimientosSalvadores.insert(movimientosSalvadores.begin(), { origen, destino });
								continue;
							}
						}

						// Cualquier movimiento legal que saque del jaque
						movimientosSalvadores.push_back({ origen, destino });
					}
					else if (objetivo && objetivo->getColor() != turno) {
						movimientosCaptura.push_back({ origen, destino });
					}
					else {
						movimientosNormales.push_back({ origen, destino });
					}
				}
			}
		}
	}

	// 1. Si estamos en jaque, priorizar cualquier movimiento que lo evite
	if (!movimientosSalvadores.empty()) {
		auto [origen, destino] = movimientosSalvadores.front();
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva salió del jaque moviendo de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 2. Si no hay jaque, priorizar capturas
	for (const auto& [origen, destino] : movimientosCaptura) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 3. Si no hay capturas, hacer un movimiento normal
	for (const auto& [origen, destino] : movimientosNormales) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	std::vector<std::pair<Vector2D, Vector2D>> movimientosSalvadores;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosCaptura;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosNormales;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassTablero copia = *tablero;
					copia.moverPieza(origen, destino);
					ClassReglas reglasTemp;

					if (reglasTemp.hayJaque(copia, turno)) continue;

					ClassPieza* objetivo = tablero->getPieza(destino);

					if (reyEnJaque) {
						// Si estamos en jaque, cualquier movimiento legal que lo evite es válido
						movimientosSalvadores.emplace_back(origen, destino);
					}
					else if (objetivo && objetivo->getColor() != turno) {
						movimientosCaptura.emplace_back(origen, destino);
					}
					else {
						movimientosNormales.emplace_back(origen, destino);
					}
				}
			}
		}
	}

	// 1. Si estamos en jaque, priorizar cualquier movimiento que lo evite
	if (!movimientosSalvadores.empty()) {
		auto [origen, destino] = movimientosSalvadores.front();
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva salió del jaque moviendo de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 2. Si no hay jaque, priorizar capturas
	for (const auto& [origen, destino] : movimientosCaptura) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// 3. Si no hay capturas, hacer un movimiento normal
	for (const auto& [origen, destino] : movimientosNormales) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::pair<Vector2D, Vector2D>> movimientosCaptura;
	std::vector<std::pair<Vector2D, Vector2D>> movimientosNormales;

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					if (objetivo && objetivo->getColor() != turno) {
						// Simular captura
						ClassTablero copia = *tablero;
						if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
							continue;

						copia.moverPieza(origen, destino);
						ClassReglas reglasTemp;
						Vector2D posRey = reglasTemp.buscarRey(copia, turno);
						ClassPieza* rey = copia.getPieza(posRey);

						// Si el rey queda vulnerable, solo permitir si la pieza capturada es valiosa
						bool reyAmenazado = rey && reglasTemp.PosAmenzada(posRey, copia, rey);
						bool capturaValiosa = objetivo->getTipo() == ClassPieza::Reina ||
							objetivo->getTipo() == ClassPieza::Torre;

						if (!reyAmenazado || capturaValiosa) {
							movimientosCaptura.emplace_back(origen, destino);
						}
					}
					else if (!objetivo) {
						// Movimiento normal
						if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
							movimientosNormales.emplace_back(origen, destino);
						}
					}
				}
			}
		}
	}

	// Priorizar capturas
	for (const auto& [origen, destino] : movimientosCaptura) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva capturó de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	// Si no hay capturas, hacer un movimiento normal
	for (const auto& [origen, destino] : movimientosNormales) {
		if (tablero->moverPieza(origen, destino)) {
			reglas.set_turno();
			std::cout << "IA Agresiva movió de " << origen << " a " << destino << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva no encontró movimientos válidos." << std::endl;
	return false;
}*/

/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	int mejorScore = 1000;
	std::pair<Vector2D, Vector2D> mejorMovimiento = { {-1, -1}, {-1, -1} };

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);
			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);
				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);

					// Permitir capturas propias solo si son ventajosas
					if (objetivo && objetivo->getColor() == turno) {
						if (!capturaPropiaVentajosa(tablero, reglas, origen, destino))
							continue;
					}

					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					// Simular el movimiento y evaluarlo
					ClassTablero copia = *tablero;
					if (!copia.moverPieza(origen, destino)) continue;

					ClassReglas reglasTemp;
					int score = evaluarMovimientoAgresivo(&copia, reglasTemp);

					if (score < mejorScore) {
						mejorScore = score;
						mejorMovimiento = { origen, destino };
					}
				}
			}
		}
	}

	if (mejorMovimiento.first.x != -1) {
		if (tablero->moverPieza(mejorMovimiento.first, mejorMovimiento.second)) {
			reglas.set_turno();
			std::cout << "IA Agresiva mejorada movió de " << mejorMovimiento.first
				<< " a " << mejorMovimiento.second << " con score: " << mejorScore << std::endl;
			return true;
		}
	}

	std::cout << "IA Agresiva mejorada no encontró movimientos válidos." << std::endl;
	return false;
}*/


/*bool IA::movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas) {
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
		return tipo == ClassPieza::Pieza_t::Reina ||
			tipo == ClassPieza::Pieza_t::Torre ||
			tipo == ClassPieza::Pieza_t::Alfil;
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
						int score = 0;

						// Bonificación por captura al paso
						Vector2D casillaEnPasante = tablero->getCasillaObjetivoEnPasante();
						ClassPieza::Color colorPeonEnPasante = tablero->getColorPeonVulnerableEnPasante();

						if (pieza->getTipo() == ClassPieza::Peon &&
							destino == casillaEnPasante &&
							objetivo == nullptr &&
							colorPeonEnPasante != turno) {
							score -= 6; // bonificación por captura al paso
						}

						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);
						ClassReglas reglasTemp;

						if (reglasTemp.hayJaque(copia, turno)) continue;

						if (HistorialMovimientos::esRepetido(origen, destino)) {
							score += 15;
						}

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

						if (estaAmenazada && !reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score -= 5;
						}

						if (objetivo && objetivo->getColor() != turno &&
							reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += 15;
						}

						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 2;
						}

						Vector2D posRey = reglasTemp.buscarRey(copia, turno);
						ClassPieza* rey = copia.getPieza(posRey);
						if (rey && reglasTemp.PosAmenzada(posRey, copia, rey)) {
							score += 20;
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
			HistorialMovimientos::registrarMovimiento(origen, destino);
			reglas.set_turno();
			std::cout << "IA Defensiva mejorada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Defensiva mejorada no encontró movimientos seguros." << std::endl;
	return false;
}

/*
bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto esPiezaValiosa = [](ClassPieza::Pieza_t tipo) {
		return tipo == ClassPieza::Pieza_t::Reina ||
			   tipo == ClassPieza::Pieza_t::Torre ||
			   tipo == ClassPieza::Pieza_t::Alfil;
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

						// Penalizar si el movimiento es repetido
						if (HistorialMovimientos::esRepetido(origen, destino)) {
							score += 15;
						}

						// Penalizar si deja piezas valiosas en peligro
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

						// Penalización si el rey queda vulnerable tras el movimiento
						Vector2D posRey = reglasTemp.buscarRey(copia, turno);
						ClassPieza* rey = copia.getPieza(posRey);
						if (rey && reglasTemp.PosAmenzada(posRey, copia, rey)) {
							score += 20;
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
			HistorialMovimientos::registrarMovimiento(origen, destino);
			reglas.set_turno();
			std::cout << "IA Defensiva mejorada movió de " << origen << " a " << destino
				<< " con score: " << std::get<2>(*mejor) << std::endl;
			return true;
		}
	}

	std::cout << "IA Defensiva mejorada no encontró movimientos seguros." << std::endl;
	return false;
}*/

/*bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	std::vector<std::tuple<Vector2D, Vector2D, int>> movimientosEvaluados;

	auto esPiezaValiosa = [](ClassPieza::Pieza_t tipo) {
		return tipo == ClassPieza::Pieza_t::Reina ||
			tipo == ClassPieza::Pieza_t::Torre ||
			tipo == ClassPieza::Pieza_t::Alfil;
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
										score += 10;
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

						// NUEVO: Penalización si el rey queda vulnerable tras el movimiento
						Vector2D posRey = reglasTemp.buscarRey(copia, turno);
						ClassPieza* rey = copia.getPieza(posRey);
						if (rey && reglasTemp.PosAmenzada(posRey, copia, rey)) {
							score += 20;
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

		Vector2D origen =  std::get<0>(*mejor);
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
}*/

/*bool IA::movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas) {
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
}*/
bool IA::movimientoTactico(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();
	ClassPieza::Color enemigo = (turno == ClassPieza::Color::AZUL) ? ClassPieza::Color::ROJO : ClassPieza::Color::AZUL;
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

	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					if (esPropia && !reyEnJaque) {
						if (valorPieza(objetivo->getTipo()) >= valorPieza(pieza->getTipo()))
							continue;
					}

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						// Bonificación por jaque
						if (reglasTemp.hayJaque(copia, enemigo)) {
							score -= 25;
						}

						// Bonificación por jaque mate
						if (reglasTemp.hayJaqueMate(copia, enemigo)) {
							score -= 100;
						}

						if (objetivo && objetivo->getColor() != turno) {
							score -= valorPieza(objetivo->getTipo()) * 10;
						}

						if (objetivo && valorPieza(pieza->getTipo()) > valorPieza(objetivo->getTipo())) {
							score += 5;
						}

						if (reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += valorPieza(pieza->getTipo()) * 5;
						}

						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						if (pieza->getTipo() == ClassPieza::Pieza_t::Peon) {
							int filaInicial = (turno == ClassPieza::Color::AZUL) ? tablero->getFilas() - 2 : 1;
							if (origen.x == filaInicial && std::abs(destino.x - origen.x) == 2) {
								score -= 3;
							}
						}

						if ((pieza->getTipo() == ClassPieza::Pieza_t::Caballo || pieza->getTipo() == ClassPieza::Pieza_t::Alfil) &&
							((turno == ClassPieza::Color::AZUL && origen.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && origen.x <= 1))) {
							score -= 4;
						}

						if (pieza->getTipo() != ClassPieza::Pieza_t::Peon &&
							((turno == ClassPieza::Color::AZUL && destino.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && destino.x <= 1))) {
							score += 2;
						}

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

						int amenazas = 0;
						auto amenazasPosibles = pieza->obtenerMovimientosPosibles(copia);
						for (const auto& pos : amenazasPosibles) {
							ClassPieza* p = copia.getPieza(pos);
							if (p && p->getColor() != turno) amenazas++;
						}
						if (amenazas >= 2) score -= 6;

						if (std::find(historialTactico.begin(), historialTactico.end(), std::make_pair(origen, destino)) != historialTactico.end()) {
							score += 15;
						}

						if (esPropia && reglas.PosAmenzada(destino, *tablero, objetivo)) {
							score -= 5;
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

	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					if (esPropia && !reyEnJaque) {
						if (valorPieza(objetivo->getTipo()) >= valorPieza(pieza->getTipo()))
							continue;
					}

					if (ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno)) {
						ClassTablero copia = *tablero;
						copia.moverPieza(origen, destino);

						ClassReglas reglasTemp;
						if (reglasTemp.hayJaque(copia, turno)) continue;

						int score = 0;

						if (objetivo && objetivo->getColor() != turno) {
							score -= valorPieza(objetivo->getTipo()) * 10;
						}

						if (objetivo && valorPieza(pieza->getTipo()) > valorPieza(objetivo->getTipo())) {
							score += 5;
						}

						if (reglasTemp.PosAmenzada(destino, copia, pieza)) {
							score += valorPieza(pieza->getTipo()) * 5;
						}

						if (esCentro(destino, tablero->getFilas(), tablero->getColumnas())) {
							score -= 3;
						}

						if (pieza->getTipo() == ClassPieza::Pieza_t::Peon) {
							int filaInicial = (turno == ClassPieza::Color::AZUL) ? tablero->getFilas() - 2 : 1;
							if (origen.x == filaInicial && std::abs(destino.x - origen.x) == 2) {
								score -= 3;
							}
						}

						if ((pieza->getTipo() == ClassPieza::Pieza_t::Caballo || pieza->getTipo() == ClassPieza::Pieza_t::Alfil) &&
							((turno == ClassPieza::Color::AZUL && origen.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && origen.x <= 1))) {
							score -= 4;
						}

						if (pieza->getTipo() != ClassPieza::Pieza_t::Peon &&
							((turno == ClassPieza::Color::AZUL && destino.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && destino.x <= 1))) {
							score += 2;
						}

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

						int amenazas = 0;
						auto amenazasPosibles = pieza->obtenerMovimientosPosibles(copia);
						for (const auto& pos : amenazasPosibles) {
							ClassPieza* p = copia.getPieza(pos);
							if (p && p->getColor() != turno) amenazas++;
						}
						if (amenazas >= 2) score -= 6;

						if (std::find(historialTactico.begin(), historialTactico.end(), std::make_pair(origen, destino)) != historialTactico.end()) {
							score += 15;
						}

						if (esPropia && reglas.PosAmenzada(destino, *tablero, objetivo)) {
							score -= 5;
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
}*/

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

	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					// Permitir capturar pieza propia si estamos en jaque o si puede ser ventajoso
					if (esPropia && !reyEnJaque) {
						if (valorPieza(objetivo->getTipo()) >= valorPieza(pieza->getTipo()))
							continue;
					}

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

						// Bonificación por avance doble de peón
						if (pieza->getTipo() == ClassPieza::Pieza_t::Peon) {
							int filaInicial = (turno == ClassPieza::Color::AZUL) ? tablero->getFilas() - 2 : 1;
							if (origen.x == filaInicial && std::abs(destino.x - origen.x) == 2) {
								score -= 3;
							}
						}

						// Bonificación por desarrollar piezas menores
						if ((pieza->getTipo() == ClassPieza::Pieza_t::Caballo || pieza->getTipo() == ClassPieza::Pieza_t::Alfil) &&
							((turno == ClassPieza::Color::AZUL && origen.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && origen.x <= 1))) {
							score -= 4;
						}

						// Penalización por retroceder sin progreso
						if (pieza->getTipo() != ClassPieza::Pieza_t::Peon &&
							((turno == ClassPieza::Color::AZUL && destino.x >= tablero->getFilas() - 2) ||
								(turno == ClassPieza::Color::ROJO && destino.x <= 1))) {
							score += 2;
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

						// Bonificación si la captura propia evita jaque futuro
						if (esPropia && reglas.PosAmenzada(destino, *tablero, objetivo)) {
							score -= 5;
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
}*/

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

	bool reyEnJaque = reglas.hayJaque(*tablero, turno);

	for (int i = 0; i < tablero->getFilas(); ++i) {
		for (int j = 0; j < tablero->getColumnas(); ++j) {
			Vector2D origen(i, j);
			ClassPieza* pieza = tablero->getPieza(origen);

			if (pieza && pieza->getColor() == turno) {
				auto movimientos = pieza->obtenerMovimientosPosibles(*tablero);

				for (const auto& destino : movimientos) {
					ClassPieza* objetivo = tablero->getPieza(destino);
					bool esPropia = objetivo && objetivo->getColor() == turno;

					// Permitir capturar pieza propia si estamos en jaque o si puede ser ventajoso
					if (esPropia && !reyEnJaque) {
						// Solo permitir si la pieza propia es de menor valor o desbloquea algo
						if (valorPieza(objetivo->getTipo()) >= valorPieza(pieza->getTipo()))
							continue;
					}

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

						// Bonificación si la captura propia evita jaque futuro
						if (esPropia && reglas.PosAmenzada(destino, *tablero, objetivo)) {
							score -= 5;
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
}*/

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
bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
	ClassPieza::Color turno = reglas.getColorTurno();

	struct EvaluacionEstrategia {
		TipoIA tipo;
		int score;
	};

	std::vector<EvaluacionEstrategia> evaluaciones;

	// Evaluar cada estrategia (ya incluyen jaque y mate en sus evaluaciones)
	int scoreAgresivo = evaluarMovimientoAgresivo(tablero, reglas);
	int scoreDefensivo = evaluarMovimientoDefensivo(tablero, reglas);
	int scoreTactico = evaluarMovimientoTactico(tablero, reglas);

	evaluaciones.push_back({ TipoIA::Agresiva, scoreAgresivo });
	evaluaciones.push_back({ TipoIA::Defensiva, scoreDefensivo });
	evaluaciones.push_back({ TipoIA::Tactica, scoreTactico });

	// Historial para evitar repeticiones
	static std::deque<TipoIA> historial;
	const int maxHistorial = 5;

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

	// Ejecutar la estrategia elegida
	bool exito = false;

	switch (mejor->tipo) {
	case TipoIA::Agresiva:
		std::cout << "[IA Adaptativa] Modo elegido: Agresivo" << std::endl;
		exito = movimientoAgresivo(tablero, reglas);
		break;
	case TipoIA::Defensiva:
		std::cout << "[IA Adaptativa] Modo elegido: Defensivo" << std::endl;
		exito = movimientoDefensivo(tablero, reglas);
		break;
	case TipoIA::Tactica:
		std::cout << "[IA Adaptativa] Modo elegido: Táctico" << std::endl;
		exito = movimientoTactico(tablero, reglas);
		break;
	default:
		std::cout << "[IA Adaptativa] Modo elegido: Aleatorio (fallback)" << std::endl;
		exito = movimientoAleatorio(tablero, reglas);
		break;
	}

	// Verificar si se debe promocionar una pieza
	if (exito) {
		for (int i = 0; i < tablero->getFilas(); ++i) {
			for (int j = 0; j < tablero->getColumnas(); ++j) {
				Vector2D pos(i, j);
				ClassPieza* pieza = tablero->getPieza(pos);
				if (pieza && pieza->getTipo() == ClassPieza::Peon && pieza->getColor() == turno) {
					int filaPromo = (turno == ClassPieza::Color::AZUL) ? 0 : tablero->getFilas() - 1;
					if (pieza->getPos().x == filaPromo) {
						int filas = tablero->getFilas();
						int columnas = tablero->getColumnas();
						int variante = (filas == 5 && columnas == 4) ? 1 : 2;
						char seleccion = (variante == 1) ? 'd' : 't'; // Silverman -> Reina, Demi -> Torre

						if (mejor->tipo == TipoIA::Tactica) {
							scoreTactico -= 20;
						}

						tablero->promocionarPieza(*pieza, seleccion, variante);
						std::cout << "[IA Adaptativa] Promoción automática realizada a "
							<< ((seleccion == 'd') ? "Reina" : "Torre") << std::endl;
					}
				}
			}
		}
	}

	return exito;
}

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
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

	// Historial para evitar repeticiones
	static std::deque<TipoIA> historial;
	const int maxHistorial = 5;

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

	// Ejecutar la estrategia elegida
	bool exito = false;

	switch (mejor->tipo) {
	case TipoIA::Agresiva:
		std::cout << "[IA Adaptativa] Modo elegido: Agresivo" << std::endl;
		exito = movimientoAgresivo(tablero, reglas);
		break;
	case TipoIA::Defensiva:
		std::cout << "[IA Adaptativa] Modo elegido: Defensivo" << std::endl;
		exito = movimientoDefensivo(tablero, reglas);
		break;
	case TipoIA::Tactica:
		std::cout << "[IA Adaptativa] Modo elegido: Táctico" << std::endl;
		exito = movimientoTactico(tablero, reglas);
		break;
	default:
		std::cout << "[IA Adaptativa] Modo elegido: Aleatorio (fallback)" << std::endl;
		exito = movimientoAleatorio(tablero, reglas);
		break;
	}

	// Verificar si se debe promocionar una pieza
	if (exito) {
		for (int i = 0; i < tablero->getFilas(); ++i) {
			for (int j = 0; j < tablero->getColumnas(); ++j) {
				Vector2D pos(i, j);
				ClassPieza* pieza = tablero->getPieza(pos);
				if (pieza && pieza->getTipo() == ClassPieza::Peon && pieza->getColor() == turno) {
					int filaPromo = (turno == ClassPieza::Color::AZUL) ? 0 : tablero->getFilas() - 1;
					if (pieza->getPos().x == filaPromo) {
						int filas = tablero->getFilas();
						int columnas = tablero->getColumnas();
						int variante = (filas == 5 && columnas == 4) ? 1 : 2;
						char seleccion = (variante == 1) ? 'd' : 't'; // Silverman -> Reina, Demi -> Torre

						// Bonificación por promoción inminente
						if (mejor->tipo == TipoIA::Tactica) {
							scoreTactico -= 20;
						}

						tablero->promocionarPieza(*pieza, seleccion, variante);
						std::cout << "[IA Adaptativa] Promoción automática realizada a "
							<< ((seleccion == 'd') ? "Reina" : "Torre") << std::endl;
					}
				}
			}
		}
	}

	return exito;
}*/

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
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

	// Historial para evitar repeticiones
	static std::deque<TipoIA> historial;
	const int maxHistorial = 5;

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

	// Ejecutar la estrategia elegida
	bool exito = false;
	Vector2D origen, destino;

	switch (mejor->tipo) {
	case TipoIA::Agresiva:
		std::cout << "[IA Adaptativa] Modo elegido: Agresivo" << std::endl;
		exito = movimientoAgresivo(tablero, reglas);
		break;
	case TipoIA::Defensiva:
		std::cout << "[IA Adaptativa] Modo elegido: Defensivo" << std::endl;
		exito = movimientoDefensivo(tablero, reglas);
		break;
	case TipoIA::Tactica:
		std::cout << "[IA Adaptativa] Modo elegido: Táctico" << std::endl;
		exito = movimientoTactico(tablero, reglas);
		break;
	default:
		std::cout << "[IA Adaptativa] Modo elegido: Aleatorio (fallback)" << std::endl;
		exito = movimientoAleatorio(tablero, reglas);
		break;
	}

	// Verificar si se debe promocionar una pieza
	if (exito) {
		for (int i = 0; i < tablero->getFilas(); ++i) {
			for (int j = 0; j < tablero->getColumnas(); ++j) {
				Vector2D pos(i, j);
				ClassPieza* pieza = tablero->getPieza(pos);
				if (pieza && pieza->getTipo() == ClassPieza::Peon && pieza->getColor() == turno) {
					int filaPromo = (turno == ClassPieza::Color::AZUL) ? 0 : tablero->getFilas() - 1;
					if (pieza->getPos().x == filaPromo) {
						int filas = tablero->getFilas();
						int columnas = tablero->getColumnas();
						int variante = (filas == 5 && columnas == 4) ? 1 : 2;
						char seleccion = (variante == 1) ? 'd' : 't'; // Silverman -> Reina, Demi -> Torre
						tablero->promocionarPieza(*pieza, seleccion, variante);
						std::cout << "[IA Adaptativa] Promoción automática realizada a "
							<< ((seleccion == 'd') ? "Reina" : "Torre") << std::endl;
					}
				}
			}
		}
	}

	return exito;
}*/

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
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

	// Historial para evitar repeticiones
	static std::deque<TipoIA> historial;
	const int maxHistorial = 5;

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
}*/

/*bool IA::movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas) {
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
					if (!ValidadorDeMovimientos::esMovimientoLegal(*tablero, origen, destino, turno))
						continue;

					ClassPieza* objetivo = tablero->getPieza(destino);

					if (objetivo && objetivo->getColor() != turno) {
						// Capturar pieza enemiga
						int valor = objetivo->getTipo() == ClassPieza::Pieza_t::Rey ? 100 : 10;
						mejorScore = std::min(mejorScore, -valor);
					}
					else if (pieza->getTipo() == ClassPieza::Pieza_t::Rey) {
						// Mover el rey a una casilla segura
						mejorScore = std::min(mejorScore, 5);
					}
				}
			}
		}
	}

	return mejorScore;
}

/*
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
}*/
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
bool IA::capturaPropiaVentajosa(ClassTablero* tablero, ClassReglas& reglas, const Vector2D& origen, const Vector2D& destino) {
	ClassPieza* pieza = tablero->getPieza(origen);
	ClassPieza* objetivo = tablero->getPieza(destino);

	if (!pieza || !objetivo || pieza->getColor() != objetivo->getColor())
		return false;

	// Simular el movimiento
	ClassTablero copia = *tablero;
	copia.moverPieza(origen, destino);

	ClassReglas reglasTemp;
	return !reglasTemp.hayJaque(copia, pieza->getColor()); // Si elimina el jaque, es ventajoso
}

