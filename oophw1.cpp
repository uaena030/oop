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
    vector<int> precedence;
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
        myPQB[i].LQBID = i;
    }
    //output initial map
    for(int i = 1; i < myPQB.size(); i++){
        cout << i << " " << myPQB[i].LQBID << endl;
    }
    //precedence judge
    int nowloop = 1;
    while(nowloop != (precedence + 1)){
        int Isnei = 0, i = myLQB[nowloop].PQBID;
        for (int j = 0; j < myPQB [mygate[preIDlist[i]].gatemember [0]].pneighbor.size(); j++){//can do cnot
                if (myPQB[mygate[preIDlist[i]].gatemember [0]].pneighbor[j] == mygate[preIDlist[i]].gatemember[1]){
                    Isnei = 1;
                    nowloop++;
                    cout << "CNOT q" << mygate[preIDlist[i]].gatemember[0] << " q" << mygate[preIDlist[i]].gatemember[1] << endl;
                    break;
                }
        }

        //can't do cnot, swap
        if(Isnei == 0){//assume now is gate4 (2, 3), nowloop is 4
            //for(int swaploop = 1; swaploop <= myLQB.size(); swaploop++){
                //bfs(using pneighbor)
                int initial = mygate[preIDlist[i]].gatemember[0];
                int end = mygate[preIDlist[i]].gatemember[1];
                int **clink = new int *[phyQubits];
                for (int k = 0; k < phyQubits; ++k)
                {
                    clink[k] = new int[phyQubits];
                }
                for(int cpylink = 0; cpylink < myPQB.size(); cpylink++){
                    if(myPQB[cpylink].pneighbor.empty()){
                        break;
                    }
                    else{
                        for(int cpydlink = 0; cpydlink < myPQB[cpylink].pneighbor.size(); cpydlink++){
                            clink[cpylink][myPQB[cpylink].pneighbor[cpydlink]] = true;
                        }
                    }
                }
                int* result = BFS(initial, end, clink, phyQubits);
                int coresult[step];
                for(int copy = 0; copy < step; copy++){
                    coresult[copy] = result[copy];
                }
                int tempend = result[step - 1];
                for(int changepath = 1; changepath < step - 1; changepath++){
                    result[changepath + 1] = result[changepath];
                }
                result[1] = tempend;
                cout << "SWAP q" << result[0] << " q" << result[1] << endl;
                // remapping logical qubits
                for(int remapp = 0; remapp < step; remapp++){
                    myLQB[coresult[remapp]].PQBID = result[remapp];
                }
            //}
        }
    }
}