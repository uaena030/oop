#include <iostream>
#include <vector>
#include <utility>
#include <map>
using namespace std;

int step;

class PQB{
    public:
    int LQBID;//initial mapping, will not changed
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

typedef struct node{
    int x;
    int y;
} node;

int *BFS(int src, int end, int **maze, int size)
{ // 1 0
    node queue[size * size + 1];
    int head = 0, tail = 1;
    queue[0].x = src;
    queue[0].y = src; //(src, src) is starting point
    int node_x, node_y;
    int visited[size], ancient[6000];
    for (int i = 0; i < size; i++)
    {
        visited[i] = false;
    }
    visited[src] = true;
    // bfs
    while (head != tail)
    {
        node_x = queue[head].x;
        node_y = queue[head].y;
        if (node_x == end && node_y == end)
        {
            break;
        }
        for (int i = 0; i < size; i++)
        {
            if (maze[node_y][i] == 1 && visited[i] == false){//bug appear
                ancient[i] = node_y;
                queue[tail].x = node_y; // 0 1
                queue[tail].y = i;      // 1
                visited[i] = true;
                tail++;
            }
        }
        head++;
    }
    // generate route
    int rroute[6000];
    for (int i = 0; i < 6000; i++)
    {
        rroute[i] = -1;
    }
    int i = end, j = 0;
    while (i != src)
    {
        rroute[j] = i;
        j++;
        i = ancient[i];
    }
    rroute[j] = i;
    int *route = new int[6000];
    for (int i = 0; i < j + 1; i++)
    {
        route[i] = rroute[j - i];
    }
    step = j + 1;
    return route;
}

int main(){
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
    int **clink = new int *[phyQubits + 1];
    for (int k = 0; k < phyQubits + 1; ++k){
        clink[k] = new int[phyQubits + 1];
    }
    for(int i = 0; i < phyQubits + 1; i++){
        for(int j = 0 ; j < phyQubits + 1; j++){
            clink[i][j] = false;
        }
    }
    for(int i = 1; i <= phyLinks; i++){//physical links input
        int p1, p2, trash;
        cin >> trash >> p1 >> p2;
        myPQB[p1].pneighbor.push_back(p2);
        myPQB[p2].pneighbor.push_back(p1);
        clink[p1][p2] = true;
        clink[p2][p1] = true;
    }
    //baseline assume that logqubits equal to phyqubits
    for(int i = 1; i <= myPQB.size(); i++){
        myLQB[i].PQBID = i;
        myPQB[i].LQBID = i;
    }
    //output initial map
    for(int i = 1; i < myPQB.size(); i++){
        cout << i << " " << myPQB[i].LQBID << endl;
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
                    cout << "CNOT q" << mygate[nowloop].gatemember[0] << " q" << mygate[nowloop].gatemember[1] << endl;
                    nowloop++;
                    break;
                }
        }

        //can't do cnot, swap
        if(Isnei == 0){
                //bfs(using gatemember)
                int* result = BFS(forwardB, backwardB, clink, (phyQubits + 1));
                int coresult[step];
                for(int copy = 0; copy < step; copy++){
                    coresult[copy] = result[copy];
                }
                //swap path point
                int tempend = result[step - 1];
                result[step - 1] = result[step - 2];
                result[step - 2] = tempend;
                //find who is now at coreresult[step - 1] and coreresult[step - 2]
                int swA, swB;
                for(int hard = 1; hard <= logQubits; hard++){
                    if(myLQB[hard].PQBID == coresult[step - 1]){
                        swA = hard;
                    }
                    else if(myLQB[hard].PQBID == coresult[step - 2]){
                        swB = hard;
                    }
                }
                cout << "SWAP q" << swA << " q" << swB << endl;
                // remapping logical qubits
                int tempQB = myLQB[swA].PQBID;
                myLQB[swA].PQBID = myLQB[swB].PQBID;
                myLQB[swB].PQBID = tempQB;
        }
    }
}