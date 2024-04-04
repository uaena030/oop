#include <iostream>
#include <vector>
#include <utility>
#include <map>
using namespace std;


class PQB{
    public:
    vector<int> pneighbor;
};

class LQB{//can be swapped
    public:
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
        myLQB[i].PQBID = i;
    }
    //precedence judge
    int nowloop = 1;
    while(nowloop != (precedence + 1)){
        int Isnei = 0;
        for (int j = 0; j < myPQB [mygate[preIDlist[nowloop]].gatemember [0]].pneighbor.size(); j++){//can do cnot
                if (myPQB[mygate[preIDlist[nowloop]].gatemember [0]].pneighbor[j] == mygate[preIDlist[nowloop]].gatemember[1]){
                    Isnei = 1;
                    nowloop++;
                    cout << "CNOT q" << mygate[preIDlist[nowloop]].gatemember[0] << " q" << mygate[preIDlist[nowloop]].gatemember[1] << endl;
                    break;
                }
        }

        //can't do cnot, swap
        if(Isnei == 0){//assume now is gate4 (2, 3), nowloop is 4
            if(){//can swap(swap logical qubit)
                //bfs
            }
            else{//can't swap && can't cnot

            }
        }
    }

    //output
    for(int i = 1; i <= myPQB.size(); i++){
        cout << i << " " << myPQB[i].LQBID;
    }
    
}