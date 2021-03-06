#include <bits/stdc++.h>
using namespace std;

// struct defination
struct instruction{
    int Tid;
    char Opr;
    int DataItem;
    instruction(int Tid, int Opr, int DataItem){
        this->Tid = Tid;
        this->Opr = Opr;
        this->DataItem = DataItem;
    }
};




//class declarartion
class Schedule{


public:
    void readInput();   // to read the given input file
    void isRecoverable();  // to check whether schedule is recoverable or not
    void isCascadeless();  // to check whether schedule is cascadeless or not
    void isSerializable();
    void DFS(int u, map <int, int> &visited, bool &Serial);

private:
    vector <int> order;
    set <int> TransactionsList;
    map< int, vector<int> > graph;
    vector<instruction> transaction; 
    // it stores all the trasaction details
    map<int,int> commit;
    // it stores the position of commit operation of a trasaction 
    vector< pair<int,pair<int,int> > > firstReadArray; 
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid  
};


void Schedule::readInput(){
	int Tid, DataItem=-1;
    char Opr;
    while(cin>>Tid){
        TransactionsList.insert(Tid);
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
        }       
        transaction.push_back(instruction(Tid,Opr,DataItem));
    }
    for(set <int> :: iterator it = TransactionsList.begin(); it != TransactionsList.end(); it++){
        if (commit.find(*it) == commit.end())
            commit[*it] = INT_MAX;
    }
}


void Schedule::isRecoverable(){
	for(int i=0; i<transaction.size(); i++){
        if(transaction[i].Opr!='W')
            continue;
        int DataItem=transaction[i].DataItem;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].DataItem!=DataItem || transaction[j].Tid == transaction[i].Tid)
                continue;
            if(transaction[j].Opr=='R'){
                if (commit[transaction[j].Tid] < commit[transaction[i].Tid]){
                    cout<<"No"<<endl;
                    cout<<"Transaction "<< transaction[j].Tid <<" is reading " << DataItem << " written by Transaction "<< transaction[i].Tid <<endl;
                    cout<<"And Transaction "<< transaction[i].Tid <<" commits after Transaction "<< transaction[j].Tid << endl;
                return;
                }
            }
            else if (transaction[j].Opr=='W')
                break;
            
        }
        
    }
    cout<<"Yes, the given schedule is recoverable."<<endl;
}


void Schedule::isCascadeless(){
    for(int i=0; i<transaction.size(); i++){
        if(transaction[i].Opr!='W')
            continue;
        int DataItem=transaction[i].DataItem;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].DataItem!=DataItem || transaction[j].Tid == transaction[i].Tid)
                continue;
            if(transaction[j].Opr=='R'){
                if (j < commit[transaction[i].Tid]){
                    cout<<"No"<<endl;
                    cout<<"Transaction "<< transaction[j].Tid <<" is reading "<< DataItem << " written by Transaction "<< transaction[i].Tid <<endl;
                    cout<<"And Transaction "<< transaction[i].Tid <<" commits after " <<  DataItem << " is read by " << transaction[j].Tid << endl;
                return;
                }
            }
            else if (transaction[j].Opr=='W')
                break;
            
        }
        
    }
    cout<<"Yes, the given schedule is cascadeless."<<endl;
}

void Schedule::isSerializable(){

    bool co = true;
    bool flag = true;
    for(int i = 0; i < transaction.size(); i++){
        if(transaction[i].Opr=='C')
            continue;
        int DataItem=transaction[i].DataItem;
        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].DataItem!=DataItem || transaction[j].Tid == transaction[i].Tid)
                continue;
            if ((transaction[i].Opr == 'R' && transaction[j].Opr == 'R') || transaction[j].Opr == 'C')
                continue;
            graph[transaction[i].Tid].push_back(transaction[j].Tid);

            if(commit[transaction[i].Tid] <= commit[transaction[j].Tid])
            continue;
            else co = false;
        }
    }

    map <int, int> visited;
    bool Serial = true;
    for(set <int> :: iterator it = TransactionsList.begin(); it != TransactionsList.end(); it++){
        if (!visited[*it])
            DFS(*it, visited, Serial);
    }

    if (Serial){

        cout << "Conflict serializable: Yes" << endl;

        if(co)
        {
            cout<<"\tCommit ordering: Yes\n"<<endl;
        }
        else
        {
            cout<<"\tCommit ordering: No \n"<<endl;
        }
        // reverse(order.begin(), order.end());
        // for(int i = 0; i < order.size(); i++)
        //     cout << order[i] << " ";
        // cout << endl;
    }
    else
        {
            cout << "Conflict serializable: No" << endl;
            cout<<"\tCommit ordering : NOT applicable \n"<<endl;
        }
}

void Schedule :: DFS(int u, map <int, int> &visited, bool &Serial){
    visited[u] = 1;
    for(int i = 0; i < graph[u].size(); i++){
        int v = graph[u][i];
        if (visited[v] == 1)
            Serial = false;
        else if (!visited[v]){
            DFS(v, visited, Serial);
        }
    }
    visited[u] = 2;
    order.push_back(u);
}


//main function
int main(){

    Schedule obj;
    obj.readInput();   
    // obj.isRecoverable();  
    // obj.isCascadeless();
    obj.isSerializable();

    return 0;

}
