#include <iostream>
#include <vector>
#include <utility>
#include <map>
using namespace std;


class PQB{
    public:
    int LQBID;
    vector<int> pneighbor;
};

class LQB{//can be swapped
    int PQBID;
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
    int preIDlist[precedence + 1];
    myLQB.resize(logQubits + 1);
    mygate.resize(gates + 1);
    myPQB.resize(phyQubits + 1);
    for(int i = 1; i <= gates; i++){//gate input(may not exist in phyical links)
        int trash;
        cin >> trash >> mygate[i].gatemember[0] >> mygate[i].gatemember[1];
    }
    for(int i = 1; i <= precedence; i++){//precedence input
        int current, next, trash;
        cin >> trash >> current >> next;
        mygate[current].precedence.push_back(next);
        preIDlist[trash] = current;
    }
    for(int i = 1; i <= phyLinks; i++){//physical links input
        int p1, p2, trash;
        cin >> trash >> p1 >> p2;
        myPQB[p1].pneighbor.push_back(p2);
        myPQB[p2].pneighbor.push_back(p1);
    }
    //baseline assume that logqubits equal to phyqubits
    for(int i = 1; i <= myPQB.size(); i++){
        myPQB[i].LQBID = i;
    }
    //precedence judge
    for(int i = 1; i <= precedence; i++){
        int Isnei = 0;
        for (int j = 0; j < myPQB [mygate[preIDlist[i]].gatemember [0]].pneighbor.size(); j++){//can do cnot
                if (myPQB[mygate[preIDlist [i]].gatemember [0]].pneighbor[j] == mygate[preIDlist[i]].gatemember[1]){
                    Isnei = 1;
                    cout << "CNOT q" << mygate[preIDlist[i]].gatemember[0] << " q" << mygate[preIDlist[i]].gatemember[1] << endl;
                    break;
                }
        }

        //can't do cnot, swap
        if(Isnei == 0){
            if(){//can swap(swap logical qubit)
                //bfs
            }
            else(){//can't swap && can't cnot

            }
        }
    }

    //output
    for(int i = 1; i <= myPQB.size(); i++){
        cout << i << " " << myPQB[i].LQBID;
    }
    
}