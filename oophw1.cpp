#include <iostream>
#include <vector>
#include <utility>
using namespace std;


class PQB{
    public:
    int ID, LQBID;
    vector<int> pneighbor;
};

class LQB{
    int ID, PQBID;
};

class gate{
    public:
    vector<int> precedence;
    int gatemember[2];
};

int main(){
    vector<LQB> myLQB;
    vector<gate> mygate;
    vector<PQB> myPQB;
    int logQubits, gates, precedence, phyQubits, phyLinks;
    cin >> logQubits >> gates >> precedence >> phyQubits >> phyLinks;
    myLQB.resize(logQubits + 1);
    mygate.resize(gates + 1);
    myPQB.resize(phyQubits + 1);
    for(int i = 1; i <= gates; i++){//gate input
        int trash;
        cin >> trash >> mygate[i].gatemember[0] >> mygate[i].gatemember[1];
    }
    for(int i = 1; i <= precedence; i++){//precedence input
        int current, next, trash;
        cin >> trash >> current >> next;
        mygate[current].precedence.push_back(next); 
    }
    for(int i = 1; i <= phyLinks; i++){
        int p1, p2, trash;
        cin >> trash >> p1 >> p2;
        myPQB[p1].pneighbor.push_back(p2);
        myPQB[p2].pneighbor.push_back(p1);
    }
}