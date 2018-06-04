#include <vector>
#include <iostream> //cout
#include "evolucionSimple.hpp"

using namespace std;

int main(int argc, char** argv)
{
    const vector<int> VOLUMENES = {1, 3, 2, 3, 2, 3, 3};
    const vector<int> KILATES = {2, 100, 5, 3, 50, 16, 60};
    int TAMANIO_CAMADA = 10;
    int CAPACIDAD_MOCHILA = 6;
    int MAXIMA_GENERACION = 100;
    int PROBABILIDAD_MUTAR = 100; //1 entre 100
    int numGeneracion = 0;
    vector<vector<int>> camada = generaCamadaAleatoriaInicial(TAMANIO_CAMADA,static_cast<int>(VOLUMENES.size()));
    vector<int> evaluaciones = evaluaAdaptacionIndividuos(camada, VOLUMENES, KILATES, CAPACIDAD_MOCHILA);
    while(!condicionDeParada(evaluaciones, 0.9f) && (numGeneracion < MAXIMA_GENERACION) ){
        numGeneracion++;
        camada = creaNuevaCamada(camada, evaluaciones, PROBABILIDAD_MUTAR);
        evaluaciones.clear();//libera evaluaciones de la anterior iteracion
        evaluaciones = evaluaAdaptacionIndividuos(camada, VOLUMENES, KILATES, CAPACIDAD_MOCHILA);
    }
    vector<int> elite = seleccionaElite(camada, evaluaciones);
    for (auto gen: elite)
        cout << gen << ", ";
    cout << "\n";

    //LIBERA MEMORIA
    evaluaciones.clear();//libera evaluaciones de la ultima iteracion
    for ( auto indiv : camada ) indiv.clear();
    camada.clear();
}
