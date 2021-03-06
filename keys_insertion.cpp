//insertion of keygates
#include <bits/stdc++.h>
using namespace std;

int in,o,n,m,k,total; //no of input,output,internal nodes, edges, keygates
//update the graph
void insertion(vector<vector<int>> &netlist,vector<vector<int>> &parent,int position,int gateNo){
   if(position>=total || parent[position].size()==0){
      cout << "Invalid position for keygate" <<endl;
      return;
   }
   //handling multiple parents
   for(int i=0;i<parent[position].size();i++){
      int ind=0;
      if(netlist[parent[position][i]][0]!=position){
         ind=1;
      }
      netlist[parent[position][i]][ind] = gateNo;
      parent[gateNo].push_back(parent[position][i]);
      parent[position][i] = gateNo;
   }
   netlist[gateNo].push_back(position);
}
void dfs(vector<vector<int>> &parent,int node,set<int> &visited){
    if(visited.find(node)!=visited.end()){return;}
    visited.insert(node);

    for(int p : parent[node]){
        dfs(parent,p,visited);
    }
    return;
}

bool FindEdgeType(vector<vector<int>> &parent,int k1,int k2){
    set<int> visited_k1;
    set<int> visited_k2;
    dfs(parent,k1,visited_k1);
    dfs(parent,k2,visited_k2);
    int common = 0;
    for(auto it = visited_k1.begin();it!=visited_k1.end();it++){
        for(auto itr = visited_k2.begin();itr!=visited_k2.end();itr++){
            if((*it)==(*itr)){common++; break;}
        }
    }
    if(common!=0&&visited_k1.size()-common>0&&visited_k2.size()-common>0){
        return true;
    }
    return false;
}
int logic_locking(vector<vector<int>> &netlist,vector<vector<int>> &parent,int keySize){
    int cliqueSize = 0, cctr=1,kc=0;
    vector<int> order_keys(keySize+in,-1);
    set<int> KeyGateLocations; 
    //choose first random key gate and insert it
    int keygate = rand()%keySize; // if start after 7, that is keygate is 2
    KeyGateLocations.insert(keygate+in);
    order_keys[keygate+in] = n+in+o+kc;
    kc++;

    for(int i=2;i<=keySize;i++){
        bool non_mutable = false;
        //for each gate in netlist
        for(int gate = in ;gate<n+in;gate++){
            if(KeyGateLocations.find(gate)==KeyGateLocations.end()){
                bool edgetypes = true; //every edge is mutable
                for(auto it = KeyGateLocations.begin();it!=KeyGateLocations.end();it++){
                    edgetypes = edgetypes & (FindEdgeType(parent,gate,(*it))); 
                }
                if(edgetypes){
                    KeyGateLocations.insert(gate);
                    kc++;
                    order_keys[gate] = n+in+o+kc;
                    non_mutable = true;
                    cctr++;
                    cliqueSize = max(cctr,cliqueSize);
                    break;
                }
            }
        }
        if(!non_mutable){
            keygate = rand()%keySize; // not in keyGateLocations
            while(KeyGateLocations.find(keygate+in)!=KeyGateLocations.end()){
                keygate++;
                if(keygate==keySize){keygate=0;}
            }
            cctr=1;
            KeyGateLocations.insert(in+keygate);
            order_keys[in+keygate] = n+in+o+kc;
            kc++;
            cliqueSize = max(cctr,cliqueSize);
        }
    }
    for(int i=0;i<order_keys.size();i++){
        if(order_keys[i]!=-1){
            insertion(netlist,parent,i,order_keys[i]);
        }
        cout << order_keys[i] <<" ";} cout <<endl;
    return cliqueSize;
}

void read_data(){
   //no of input,output,internal nodes, edges, keygates
   cin >> in>> o >> n >> m;
   /*
      0 , in-1  inputs
      in, n+in-1 internal nodes
      n + in , n+in+o-1 output nodes
      n+in+o , total-1 keygates
   */
   k = n;
   cout << "Initialized" <<endl;
   map<int,string> operand;
   total = in + o + n + k;
   vector<vector<int>> netlist(total);
   vector<vector<int>> parent(total);
   int u,v;
   string s;
   //operands of gates
   for(int i=0;i<n;i++){
      cin >> s;
      operand[in+i] = s; 
   }
   cout << "Operands declared" <<endl;
   //netlist formation, taking edges as input
   for(int i=0;i<m;i++){
      cin >> u >> v;
      netlist[v].push_back(u);
      parent[u].push_back(v);
   }
   cout << "Netlist created" <<endl;
   cout << logic_locking(netlist,parent,k) <<endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
    
    //input
    read_data();
return 0;
}