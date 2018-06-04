# cpp-geneticknapshack
Algoritmo genético que resuelve el problema de la mochila 0/1 escrito en C++

Genetic Algorithm that solves knapshack 0/1 problem, written in C++


Siguiendo los conceptos de Algoritmos evolutivos genéticos como allele, gene, chromosome, etc...
Se han cambiado los nombres comunes siguiendo esta leyenda:
Population -> camada
Chromosome -> individuo
Gene -> indiceGen
Allele -> individuo[indiceGen] que es el valor de ese gen en ese cromosoma
Genotype -> vector<int> con valores de 0 o 1
Phenotype -> en este caso Phenotype = Genotype
Fitness Function -> la funcion evaluaAdaptacionIndividuos, evalua cuantos mas kilates entren en la mochila mejor
Objective Function -> en este caso es la fitness function, el objetivo de la mochila coincide con el de fitness function
Genetic Operators -> seleccionaElite, mutar, procrear, etc...
Steady State -> este algoritmo cambia toda la poblacion anterior por la nueva excepto el cromosoma elite(solo uno)
Fitness Selection -> seleccionaElite
Survivor Selection -> Fitness Based Selection, por lo que el unico superviviente es el cromosoma elite
Termination Condition -> si el porcentaje de convergencia iguala o supera el 90%
Mutation -> funcion mutar, con una probabilidad de mutacion de 1%
