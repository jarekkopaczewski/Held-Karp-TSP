#include "HeldKarp.h"
#include <iostream>
#include <bitset>
#include <algorithm>
#include <Windows.h>
#include <vector>
#include "Graph.h"
#include <string>
using namespace std;

HeldKarp::HeldKarp()
{  
}

HeldKarp::~HeldKarp()
{
}

int HeldKarp::findSolution(Graph* graph)
{
    vector<vector<int>> distance = graph->getVector();
    int size = graph->getSize();
    int smallSize = size - 1;
    int numberOfCombinations = pow(2,size-1);
    int result = INT_MAX;
    vector<vector<int>> optimal(numberOfCombinations, vector<int>(size, INT_MAX));

    // kolejne permutacje s¹ interpretowane jako binarna reprezentacja liczb 
    // jedynki reprezentuj¹ce kolejne potêgi dwójki oznaczaj¹ obecnoœæ wierzcho³ka w danej permutacji
    // np. permutacja 10011 oznacza obecnoœæ wierzcho³ka 1,2 oraz 5
    // poni¿sza pêtla for ma za zadaie sprawdzenie ka¿dej z permutacji, dlatego zadany
    // jest zakres od 1 do 2^(liczby wierzcho³ków)
    for (int permutations = 1; permutations < numberOfCombinations; permutations++)
    {
        // dla ka¿dej permutacji sprawdzana jest mo¿liwoœæ po³¹czenia z ka¿dym wiercho³kiem 
        for (int currentVert = 0; currentVert < smallSize; currentVert++)
        {   
            // wyra¿enie 2^(aktualny wierzcho³ek) umo¿liwia wykonanie logicznej operacji 'and'
            // w celu sprawdzenia czy permutacja zawiera aktualny wierzcho³ek
            // kolejne kroki wykonywane s¹ jedynie w momencie podobieñstwa w celu skrócenia czasu wykonania programu
            int power = 1 << currentVert;
            if (permutations & power)
            {
                // warunek jest spe³niony je¿eli dla permutacji zawieraj¹cych jeden wierzcho³ek
                // kod ma za zadanie rozpocz¹æ now¹ grupê œcie¿ek od danego wierzcho³ka
                if (permutations == power)
                {
                    // jako best value przypisywana jest poprostu wartoœæ drogi od pocz¹tkowego
                    // wierzcho³ka do aktualnie zadanego oraz dodawana jest œcie¿ka do tablicy string
                    optimal[permutations][currentVert] = distance[smallSize][currentVert];
                }
                else
                {
                    // za pomoc¹ operacjie logicznej 'xor' uzyskiwana jest liczba reprezentuj¹ca poprzedni¹ permutacje
                    // np dla aktualnej permutacji 000111 i wiercho³ka 000001 wynikiem bêdzie 000110
                    // oznacza to do³¹cznie wierzcho³ka {1} do permutacji {2,3}
                    int preVert = permutations ^ power;
                    // pêtla wykonuje przejœcie po wierzcho³kach od 1..n w celu znalezienia pokrywaj¹cych siê elementów
                    // oraz wyznaczenia minimum
                    for (int prev = 0; prev < smallSize; ++prev)
                    {
                        // operacja 'and' ma na celu sprawdzenie czy poprzednia permutacja zawiera wiercho³ek 'prev' 
                        // w tym celu obliczana jest wartoœæ 2^prev
                        if (preVert & 1 << prev)
                        {   
                            // obliczanie wartoœci dla aktualnego wariantu
                            int current = distance[prev][currentVert] + optimal[preVert][prev];
                            // sprawdzenie czy aktualny wariant jest lepszy od poprzedniego
                            // lub jeszcze nie posiada jeszcze wartoœci i ma przypisane INT_MAX
                            if (current < optimal[permutations][currentVert])
                                optimal[permutations][currentVert] = current;
                            
                        }
                    }
                }
            }
        }
    }

    // wyszukanie minimum w wariantach ostatniej permutacji
    for (int vert = 0; vert < smallSize; ++vert)
    {
        // dodawnie powrotu do pocz¹tkowego wierzcho³ka
        int currentCost = distance[vert][smallSize] + optimal[static_cast<unsigned __int64>(numberOfCombinations) - 1][vert];
        if (currentCost < result)
            result = currentCost;
    }
    return result;
}
