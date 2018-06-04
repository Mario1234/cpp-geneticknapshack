#include "evolucionSimple.hpp"

//la mochila 0/1 se tiene que maximizar el valor en kilates de la mochila
//tomando como limite la capacidad en volumen(o peso segun el enunciado) de la mochila
//se usa un array de bits(0/1) para representar si un elemento esta o no dentro de la mochila en ese intento(candidato a solucion)

//suma enteros de un vector
int sumaIntsVector(const vector<int> & lista){
    int sum_of_elems=0;
    for (auto& n : lista)
        sum_of_elems += n;
    return sum_of_elems;
}

//Punto de vista Algoritmos Geneticos
//camada tiene individuos/chromosomas que son los intentos de esa ronda(los candidatos),
//y cada individuo tiene genes con valor asociado(alelos) que son bits de elementos seleccionados(metidos en la mochila)
vector<vector<int>> generaCamadaAleatoriaInicial(int tamanioCamada, int tamanioIndividuo){
    vector<vector<int>> camadaAleIni;
    for(int i=0;i<tamanioCamada;i++){
        vector<int> individuo;
        for(int j=0;j<tamanioIndividuo;j++){
            individuo.push_back(rand()%2);
        }
        camadaAleIni.push_back(individuo);
    }
    return camadaAleIni;
}

//en el caso del problema de la mochila la funcion de fitness(esta funcion) utiliza el valor de kilates de la mochila como valoracion de cada candidato
vector<int> evaluaAdaptacionIndividuos(vector<vector<int>> & camada, const vector<int> & volumenes, const vector<int> & kilates, int capacidadMochila){
    vector<int> evaluacionIndividuos;
    int tamanioCamada = camada.size();
    for(int indiceIndividuo=0;indiceIndividuo<tamanioCamada;indiceIndividuo++){
        int kilatesActual = 0;
        int volumenActual = capacidadMochila+1;
        //mientras este excediendo la capacidad de la mochila, sacar elementos al azar
        while(volumenActual > capacidadMochila){
            kilatesActual = 0;
            volumenActual = 0;
            vector<int> elementosDentro;//1s
            int tamanioIndividuo = camada[indiceIndividuo].size();
            for(int indiceGen=0;indiceGen<tamanioIndividuo;indiceGen++){
                if(camada[indiceIndividuo][indiceGen]==1){
                    volumenActual += volumenes[indiceGen];
                    kilatesActual += kilates[indiceGen];
                    elementosDentro.push_back(indiceGen);
                }
            }
            //si excede en esta vuelta, sacar un elemento al azar
            if(volumenActual > capacidadMochila){
                int indiceGenAzar = elementosDentro[rand()%elementosDentro.size()];
                camada[indiceIndividuo][indiceGenAzar]=0;
            }
            //libera memoria
            elementosDentro.clear();
        }
        //como ya no excede la capacidad de la mochila lo toma como candidato valido, guarda su valor en kilates
        evaluacionIndividuos.push_back(kilatesActual);
    }
    return evaluacionIndividuos;
}

//en el caso de la mochila, la funcion de seleccion de los individuos que forman la elite de la adaptacion solo devuelve el mejor adaptado(uno)
vector<int> seleccionaElite(const vector<vector<int>> & camada, const vector<int> & evaluaciones){
    int indiceMejor = 0;
    int tamanioEvaluados = evaluaciones.size();//es lo mismo que el tamaniCamda camada.size()
    //el de mayor kilates es el mejor, guarda su indice
    for(int indiceIndividuo=0;indiceIndividuo<tamanioEvaluados;indiceIndividuo++){
        if(evaluaciones[indiceIndividuo] > evaluaciones[indiceMejor]){
            indiceMejor = indiceIndividuo;
        }
    }
    return camada[indiceMejor];
}

//Roulette Wheel Selection
int indiceAlAzarRuleta(int tamaniCamda, const vector<int> & evaluaciones){
    int acumAux = 0;
    int totalEval = sumaIntsVector(evaluaciones);
    int limiteArbitrario = rand()%(totalEval+1);//uno al azar desde 0 hasta totalEval, inclusives
    for(int indiceIndiv=0;indiceIndiv<tamaniCamda;indiceIndiv++){
        acumAux += evaluaciones[indiceIndiv];
        if(acumAux >= limiteArbitrario){
            return indiceIndiv;
        }
    }
    return -1;
}

//la famosa funcion selectparents en el problema de la mochila se implementa siguiendo una Roulette Wheel Selection
pair<int,int> siguienteEmparejamiento(vector<vector<int>> & camada, vector<int> & evaluaciones){
    int tamaniCamda = camada.size();
    int indiceIndiv1 = indiceAlAzarRuleta(tamaniCamda,evaluaciones);//escoge primer indiv con ruleta al azar, seleccionado primer procreador
    int evalIndiv1 = 0;
    vector<int> indiv1 = camada[indiceIndiv1];//guardamos puntero al vector
    //erase hace llamada implicita a delete camada[indiceIndiv1]; ? parece que hay que hacerlo explicito
    camada.erase(camada.begin()+indiceIndiv1);//borra el candidato de la lista, para que no procree consigo mismo
    evalIndiv1 = evaluaciones[indiceIndiv1];//copia de seguridad
    evaluaciones.erase(evaluaciones.begin()+indiceIndiv1);//borra la valoracion de ese candidato
    int indiceIndiv2 = indiceAlAzarRuleta(tamaniCamda,evaluaciones);
    //devuelve el primer individuo que se borro para hacer la seleccion del segundo individuo
    if(&indiv1==NULL)cout << "nulo!";
    else if(indiv1.size()==0)cout << "vacio!";
    camada.push_back(indiv1);
    evaluaciones.push_back(evalIndiv1);
    pair<int,int> pareja(indiceIndiv1,indiceIndiv2);
    return pareja;
}

vector<int> procrear(const vector<int> & padre, const vector<int> & madre){
    int limiteArbitrario = rand()%(padre.size());
    vector<int> hijo;
    hijo.insert( hijo.end(), padre.begin(), padre.begin()+ limiteArbitrario);
    hijo.insert( hijo.end(), madre.begin()+limiteArbitrario, madre.end() );
    return hijo;
}

//muta el individuo/chromosoma si la "tirada de azar" entra en p (probabilidad estadistica)
//p = probabilidadMutar
void mutarAlAzar(vector<int> & individuo, int probabilidadMutar){
    int tamanioIndividuo = individuo.size();
    for(int indiceGen=0;indiceGen<tamanioIndividuo;indiceGen++){
        int tiradaMutar = rand()%probabilidadMutar+1;
        if(tiradaMutar==1){
            individuo[indiceGen]=1-individuo[indiceGen];//modifica el alelo, muta de 1 a 0 o de 0 a 1
        }
    }
}

//si el array evaluacion converge a un valor entonces terminar proceso bucle de evolucion
//si el porcentaje de evaluaciones con el mismo valor llega al porcentajeConvergenciaMaximo X entonces parar
bool condicionDeParada(const vector<int> & evaluaciones, float porcentajeConvergenciaMaximo){
    int valorMaximo = *max_element( evaluaciones.begin(), evaluaciones.end() );
    int numFrecHistograma = valorMaximo+1;
    vector<int> histograma(numFrecHistograma,0);
    int tamanioEvaluaciones = static_cast<int>(evaluaciones.size());
    for( int i=0; i<tamanioEvaluaciones; i++ )
        histograma[ evaluaciones[i] ]++;
    int cuentaModa = *max_element( histograma.begin(), histograma.end() );//devuelve la cuenta mayor del muestreo de frecuencias
    //si se quiere el valor de la frecuencia con mayor cuenta = la moda, entonces usar: int moda = max_element( histograma.begin(), histograma.end() )-histograma.begin();
    float porcentajeConvergencia = (float)cuentaModa/(float)evaluaciones.size();
    //libera memoria
    histograma.clear();
    if(porcentajeConvergencia>=porcentajeConvergenciaMaximo){
        return true;
    }
    else{
        return false;
    }
}

vector<vector<int>> creaNuevaCamada(vector<vector<int>> & camada, vector<int> & evaluaciones, float probabilidadMutar){
    int tamanioCamada = camada.size();
    vector<vector<int>> nuevaCamada;
    vector<int> indivElite (seleccionaElite(camada,evaluaciones));
    nuevaCamada.push_back(indivElite);
    pair<int,int> pareja;
    while(static_cast<int>(nuevaCamada.size())<tamanioCamada){
        pareja = siguienteEmparejamiento(camada, evaluaciones);

        vector<int> hijo = procrear(camada[pareja.first],camada[pareja.second]);
        mutarAlAzar(hijo,probabilidadMutar);
        nuevaCamada.push_back(hijo);
    }
    //liberar memoria
    for ( auto indiv : camada ) indiv.clear();
    camada.clear();
    return nuevaCamada;
}
