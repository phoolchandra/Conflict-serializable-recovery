#include <bits/stdc++.h>
using namespace std;


// here is  function class
class Transaction {
    public: 
        void ReadInput(); 
        void RedoPhase();
        void UndoPhase();
        void ShowDataItems();
        int present_in_undolist(int n);
        
    private:

    	// vector<pair<int, pair<char,int> > > transactions; 
        vector<int> commit;
        vector<pair<int, tuple<char,char,int,int> > > logs;  // { T_id, { Operation, DataObject, PreviousValue, NewValue }}
        // wal suggests buffer logs array must be updated before putting record into transactions array. 
        map<char, int> dataitem;

        vector<int> undolist; // tid of transactions to undo
        map<char, int> recovery_data;  // modified by redo or undo phase
};


void Transaction::ShowDataItems() {

    int i=0;
    for(auto it = logs.begin();it != logs.end();it++)
        {
            if(get<0>(it->second)=='W')
            {
                cout<<get<0>(it->second)<<" "<<it->first<<" "<<get<1>(it->second)<<" "<<get<2>(it->second)<<" "<<get<3>(it->second)<<" "<<" \n";
            }
            else
            {
                cout<<get<0>(it->second)<<" "<<it->first<<" "<<" \n";
            }
            i++;
        }
    
    cout<<endl;
    cout<<"DataItems after recovery: "<<endl;
    for (auto it:recovery_data){
        cout<<it.first<<" -> "<<it.second<<endl;
    }
    cout<<endl;
}

void Transaction::ReadInput() {
    int num_data_items; 
    cin>>num_data_items;
    char data_item_name;
    char operation;
    int data_item_initial_value;
    int t_id;
    int new_value;
    while(num_data_items--){
        cin>>data_item_name;
        cin>>data_item_initial_value;
        dataitem[data_item_name] = data_item_initial_value;   
        recovery_data[data_item_name] = data_item_initial_value;
    }

    // handle all logs during input
    while(!cin.eof())   // take all the input till end of file.
    {
        cin>>operation;
        if ( operation == 'S') {
            cin>>t_id;
            logs.push_back({ t_id, { 'S', NULL, NULL , NULL } });
        }   
        else if ( operation == 'R' ) {
            cin>>t_id;
            cin>>data_item_name;
        }
        else if ( operation == 'W') {
            cin>>t_id;
            cin>>data_item_name;
            cin>>new_value;
            logs.push_back({ t_id, { 'W' , data_item_name, dataitem[data_item_name], new_value }});
            dataitem[data_item_name] = new_value;
        }
        else if ( operation == 'C') {
            cin>>t_id;
            commit.push_back(t_id);
            logs.push_back({ t_id, { 'C', NULL, NULL, NULL } });
        }
        else if ( operation == 'A') {
            cin>>t_id;
            commit.push_back(t_id);
            logs.push_back({ t_id, { 'A', NULL, NULL, NULL } });
        }
    }

}
// push the uncommited transactions into undol list overall
void Transaction::RedoPhase() { 
    // vector<pair<int, tuple<char,char,int,int> > > :: iterator it;
    // it = logs.begin();
    for (auto it = logs.begin(); it != logs.end();it++ ){

        if(get<0>(it->second) == 'W') {
            recovery_data[get<1>(it->second)] = get<3>(it->second);
        }
        else if ( get<0>(it->second) == 'S'){
            undolist.push_back(it->first);
        }
        else if (get<0>(it->second) == 'C') {
            int t=it->first;
            undolist.erase(remove(undolist.begin(), undolist.end(),t ),undolist.end());
        }
    }
}

int Transaction::present_in_undolist(int n)
{
    //matching correct value
    for(auto unitr = undolist.begin();unitr != undolist.end();unitr++)
            {
                if(*unitr == n)
                {
                   return 1;
                }
            }

     return 0;       
}

void Transaction::UndoPhase() {
    
    
        // vector<pair<int, tuple<char,char,int,int> > > :: reverse_iterator it;
        for( auto it = logs.rbegin(); it != logs.rend(); it++) {
            int t_id = it->first;
        

            // if(get<0>(it->second)=='S' && present_in_undolist(t_id))
            // {
            //     logs.push_back({ t_id, { 'A', NULL, NULL, NULL } });
            // }
            if(get<0>(it->second)=='W' && present_in_undolist(t_id))
            {
               recovery_data[get<1>(it->second)] = get<2>(it->second);
            }
                 
        }
    
}

int main() {


// declared class object
    Transaction T;

    T.ReadInput();
    T.RedoPhase();
    T.UndoPhase();
    T.ShowDataItems();

    return 0;
}
