#include "IA.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <deque>
#include <set>

std::deque<std::pair<Vector2D, Vector2D>> historialTactico; // Para versiones 3 y 4 IA táctica

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

