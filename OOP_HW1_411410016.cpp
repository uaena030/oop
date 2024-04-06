#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int step;

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
    int gatemember[2];
};

int *BFS(int src, int end, const vector<PQB> maze, int size){
    queue<int> path;
    path.push(src);
    vector<bool> visited(size, false);
    vector<int> ancient(size, -1);
    visited[src] = true;
    // bfs
    while (!path.empty()){
        int node = path.front();
        path.pop();
        if (node == end){
            break;
        }
        for (int i : maze[node].pneighbor){
            if (visited[i] == false){//bug appear
                ancient[i] = node;
                path.push(i);
                visited[i] = true;
            }
        }
    }
    // generate route
    vector<int> rroute;
    int i = end;
    while (i != src)
    {
        rroute.push_back(i);
        i = ancient[i];
    }
    rroute.push_back(i);
    int *route = new int[rroute.size()];
    for (int i = 0; i < rroute.size(); i++)
    {
        route[i] = rroute[rroute.size() - 1 - i];
    }
    step = rroute.size();
    return route;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    vector<LQB> myLQB;
    vector<gate> mygate;
    vector<PQB> myPQB;
    int logQubits, gates, precedence, phyQubits, phyLinks;
    cin >> logQubits >> gates >> precedence >> phyQubits >> phyLinks;
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
    }
    //initialize link map
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
    //output initial map
    for(int i = 1; i < myPQB.size(); i++){
        cout << i << " " << i << '\n';
    }
    //precedence judge
    int nowloop = 1;
    while(nowloop != gates + 1){
        int Isnei = 0;
        //their physical qubits
        int forwardB = myLQB[mygate[nowloop].gatemember [0]].PQBID;
        int backwardB = myLQB[mygate[nowloop].gatemember[1]].PQBID;
        for (int j = 0; j < myPQB[forwardB].pneighbor.size(); j++){//can do cnot
                if (myPQB[forwardB].pneighbor[j] == backwardB){
                    Isnei = 1;
                    cout << "CNOT q" << mygate[nowloop].gatemember[0] << " q" << mygate[nowloop].gatemember[1] << '\n';
                    nowloop++;
                    break;
                }
        }

        //can't do cnot, swap
        if(Isnei == 0){
                //bfs(using gatemember)
                int* result = BFS(forwardB, backwardB, myPQB, (phyQubits + 1));
                int coresult[step];
                for(int copy = 0; copy < step; copy++){
                    coresult[copy] = result[copy];
                }
                //swap path point
                while(step >= 3){
                    int tempend = result[step - 1];
                    result[step - 1] = result[step - 2];
                    result[step - 2] = tempend;
                    //find who is now at coreresult[step - 1] and coreresult[step - 2]
                    int swA, swB;
                    for(int hard = 1; hard <= logQubits; hard++){
                        int complete = -1;
                        if(complete == 1){
                            break;
                        }
                        if(myLQB[hard].PQBID == coresult[step - 1]){
                            swA = hard;
                            complete++;
                        }
                        else if(myLQB[hard].PQBID == coresult[step - 2]){
                            swB = hard;
                            complete++;
                        }
                    }
                    cout << "SWAP q" << swA << " q" << swB << '\n';
                    // remapping logical qubits
                    int tempQB = myLQB[swA].PQBID;
                    myLQB[swA].PQBID = myLQB[swB].PQBID;
                    myLQB[swB].PQBID = tempQB;
                    step--;
                }
        }
    }
}