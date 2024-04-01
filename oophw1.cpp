#include <iostream>
#include <vector>
#include <utility>
using namespace std;


class PQB{
    int ID, LQBID, nPQBIDs;
};

class LQB{
    int ID, PQBID;
};

class gate{
    int precedence, LQBIDs;
};

int main(){
    vector<int> i;
    i.resize(5);
    i[0] = 1;
    i[1] = 2;
    i[2] = 3;
    i[3] = 4;
    i[4] = 5;
    cin >> i[0];
    for(int j = 0; j < i.size(); j++){
        cout << i[j] << " ";
    }
    cout << endl;
}