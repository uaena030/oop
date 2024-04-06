
                cout << "SWAP q" << swA << " q" << swB << endl;
                // remapping logical qubits
                int tempQB = myLQB[swA].PQBID;
                myLQB[swA].PQBID = myLQB[swB].PQBID;
                myLQB[swB].PQBID = tempQB;
        }
    }
}