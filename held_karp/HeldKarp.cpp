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

    // kolejne permutacje s� interpretowane jako binarna reprezentacja liczb 
    // jedynki reprezentuj�ce kolejne pot�gi dw�jki oznaczaj� obecno�� wierzcho�ka w danej permutacji
    // np. permutacja 10011 oznacza obecno�� wierzcho�ka 1,2 oraz 5
    // poni�sza p�tla for ma za zadaie sprawdzenie ka�dej z permutacji, dlatego zadany
    // jest zakres od 1 do 2^(liczby wierzcho�k�w)
    for (int permutations = 1; permutations < numberOfCombinations; permutations++)
    {
        // dla ka�dej permutacji sprawdzana jest mo�liwo�� po��czenia z ka�dym wiercho�kiem 
        for (int currentVert = 0; currentVert < smallSize; currentVert++)
        {   
            // wyra�enie 2^(aktualny wierzcho�ek) umo�liwia wykonanie logicznej operacji 'and'
            // w celu sprawdzenia czy permutacja zawiera aktualny wierzcho�ek
            // kolejne kroki wykonywane s� jedynie w momencie podobie�stwa w celu skr�cenia czasu wykonania programu
            int power = 1 << currentVert;
            if (permutations & power)
            {
                // warunek jest spe�niony je�eli dla permutacji zawieraj�cych jeden wierzcho�ek
                // kod ma za zadanie rozpocz�� now� grup� �cie�ek od danego wierzcho�ka
                if (permutations == power)
                {
                    // jako best value przypisywana jest poprostu warto�� drogi od pocz�tkowego
                    // wierzcho�ka do aktualnie zadanego oraz dodawana jest �cie�ka do tablicy string
                    optimal[permutations][currentVert] = distance[smallSize][currentVert];
                }
                else
                {
                    // za pomoc� operacjie logicznej 'xor' uzyskiwana jest liczba reprezentuj�ca poprzedni� permutacje
                    // np dla aktualnej permutacji 000111 i wiercho�ka 000001 wynikiem b�dzie 000110
                    // oznacza to do��cznie wierzcho�ka {1} do permutacji {2,3}
                    int preVert = permutations ^ power;
                    // p�tla wykonuje przej�cie po wierzcho�kach od 1..n w celu znalezienia pokrywaj�cych si� element�w
                    // oraz wyznaczenia minimum
                    for (int prev = 0; prev < smallSize; ++prev)
                    {
                        // operacja 'and' ma na celu sprawdzenie czy poprzednia permutacja zawiera wiercho�ek 'prev' 
                        // w tym celu obliczana jest warto�� 2^prev
                        if (preVert & 1 << prev)
                        {   
                            // obliczanie warto�ci dla aktualnego wariantu
                            int current = distance[prev][currentVert] + optimal[preVert][prev];
                            // sprawdzenie czy aktualny wariant jest lepszy od poprzedniego
                            // lub jeszcze nie posiada jeszcze warto�ci i ma przypisane INT_MAX
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
        // dodawnie powrotu do pocz�tkowego wierzcho�ka
        int currentCost = distance[vert][smallSize] + optimal[static_cast<unsigned __int64>(numberOfCombinations) - 1][vert];
        if (currentCost < result)
            result = currentCost;
    }
    return result;
}
