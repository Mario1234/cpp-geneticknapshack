#ifndef EVOLUCIONSIMPLE_HPP_INCLUDED
#define EVOLUCIONSIMPLE_HPP_INCLUDED

#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int sumaIntsVector(const vector<int> & lista);

vector<vector<int>> generaCamadaAleatoriaInicial(int tamanioCamada, int tamanioIndividuo);

vector<int> evaluaAdaptacionIndividuos(vector<vector<int>> & camada, const vector<int> & volumenes, const vector<int> & kilates, int capacidadMochila);

vector<int> seleccionaElite(const vector<vector<int>> & camada, const vector<int> & evaluaciones);

int indiceAlAzarRuleta(int tamaniCamda, const vector<int> & evaluaciones);

pair<int,int> siguienteEmparejamiento(vector<vector<int>> & camada, vector<int> & evaluaciones);

vector<int> procrear(const vector<int> & padre, const vector<int> & madre);

void mutarAlAzar(vector<int> & individuo, int probabilidadMutar);

bool condicionDeParada(const vector<int> & evaluaciones, float porcentajeConvergenciaMaximo);

vector<vector<int>> creaNuevaCamada(vector<vector<int>> & camada, vector<int> & evaluaciones, float probabilidadMutar);

#endif // EVOLUCIONSIMPLE_HPP_INCLUDED
