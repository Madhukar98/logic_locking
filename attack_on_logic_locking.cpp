//Attack on Logic Locking - consideration of 2 input gates
#include <bits/stdc++.h>
using namespace std;
int in,o,n,m,k,total; //no of input,output,internal nodes, edges, keygates
//insert keygates
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
int dfs(vector<vector<int>> &netlist,int node,map<int,int> &value,map<int,string> &operand,
               vector<int> &visited){
   //cout << node <<endl;
   if(netlist[node].size()==0){return value[node];} // if input node
   if(visited[node]!=-1){return visited[node];} // if already computed
   if(netlist[node].size()==1){ // if a key gate
      if(operand[node]=="X"){
         return visited[node] =  dfs(netlist,netlist[node][0],value,operand,visited) ^ value[node];
      }
      else if(operand[node]=="A"){
         return visited[node] =  dfs(netlist,netlist[node][0],value,operand,visited) && value[node];
      }
      else if(operand[node]=="O"){
         return visited[node] =  dfs(netlist,netlist[node][0],value,operand,visited) || value[node];
      }
      else if(operand[node]=="N"){
         return visited[node] =  !(dfs(netlist,netlist[node][0],value,operand,visited) && value[node]);
      }
   }
   else{ // if normal node
      if(operand[node]=="A"){
         return visited[node] = dfs(netlist,netlist[node][0],value,operand,visited) &&
                dfs(netlist,netlist[node][1],value,operand,visited);
      }
      else if(operand[node]=="O"){
         return visited[node] = dfs(netlist,netlist[node][0],value,operand,visited) || 
                                 dfs(netlist,netlist[node][1],value,operand,visited);
      }
      else if(operand[node]=="X"){
         return visited[node] =  dfs(netlist,netlist[node][0],value,operand,visited) ^
                         dfs(netlist,netlist[node][1],value,operand,visited);
      }
      else if(operand[node]=="N"){
         return visited[node] = !(dfs(netlist,netlist[node][0],value,operand,visited) &&
                            dfs(netlist,netlist[node][1],value,operand,visited));
      }
   }
   return 0;
}
//find output corresponding to each input
string solve_circuit(vector<vector<int>> &netlist,map<int,int> &value,map<int,string> &operand){
   //run dfs for each output and concatenate the answer
   string res = "";
   vector<int> visited(total,-1);
   for(int i = n+in ;i< n+in+o;i++){
      //cout << i <<endl;
      int tv = dfs(netlist,netlist[i][0],value,operand,visited);
      res.push_back('0'+tv);
   }
   return res;
}
// provide all input for getting all output
vector<string> all_output(vector<vector<int>> &netlist,map<int,int> &value,map<int,string> &operand){
   vector<string> res;
   for(int i=0;i<pow(2,in);i++){ 
      int temp_in  = 0,temp=i;
      while(temp_in<in){
         value[temp_in] = temp%2; 
         temp /= 2;
         temp_in++;
      }
      //cout <<endl;
      res.push_back(solve_circuit(netlist,value,operand));
   }
   value.clear();
   return res;
}
// compare the values for the keygate
bool compare(const vector<string> &a,const vector<string> &b){
   for(int i=0;i<a.size();i++){if(a[i]!=b[i]){return false;}}
   return true;
}
// assign keygates
void update_keygates(map<int,int> &value,int i){
   int ks = 0;
   while(ks<k){
      value[total-ks-1] = i%2;
      i = i/2;
      ks++;
   }
}
void dfs_isolated(vector<vector<int>> &parent,map<pair<int,int>,int> &visited_edges,
                        set<int> &res,int node,int key,bool &status){
   if(parent[node].size()==0){return;}
   for(int p : parent[node]){
      if(p>=n+in&&p<n+in+o){return;}
      if(visited_edges.find({node,p})!=visited_edges.end()){
         auto it = visited_edges.find({node,p});
         res.erase((*it).second);
         status = false;
      }
      visited_edges.insert({{node,p},key});
      dfs_isolated(parent,visited_edges,res,p,key,status);
   }
}
void find_isolated(vector<vector<int>> &parent){
   set<int> res;
   map<pair<int,int>,int> visited_edges;
   bool status = true;
   for(int i=n+in+o;i<total;i++){
      status = true;
      dfs_isolated(parent,visited_edges,res,i,i,status);
      if(status){res.insert(i);}
   }
   cout <<"Isolated nodes are : ";
   for(auto it = res.begin();it!=res.end();it++){
      cout <<(*it)<<" ";
   }
   cout <<endl;
}
void read_data(){
   //no of input,output,internal nodes, edges, keygates
   cin >> in>> o >> n >> m >>k;
   /*
      0 , in-1  inputs
      in, n+in-1 internal nodes
      n + in , n+in+o-1 output nodes
      n+in+o , total-1 keygates
   */
   cout << "Initialized" <<endl;
   map<int,int> value;
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
   //actual output
   vector<string> actual_result = all_output(netlist,value,operand);
   //for(int l = 0;l<actual_result.size();l++){cout << actual_result[l] <<endl;}
   cout << "True result stored" <<endl;
   //keygates as input, where to insert
   //key gates position and gate type
   int position;
   for(int i=0;i<k;i++){
      cin >> position >> s;
      operand[n+in+o+i] = s;
      insertion(netlist,parent,position,n+in+o+i);
   }
   cout << "KeyGates Inserted" <<endl;
   //to find isolated KeyGates
   find_isolated(parent);
   for(int i=0;i<pow(2,k);i++){
      update_keygates(value,i);
      vector<string> keyruns = all_output(netlist,value,operand);
      if(compare(keyruns,actual_result)){
         cout << "KEY Found: " <<i<<endl;
      }
   }
}
int main() {
   ios_base::sync_with_stdio(false);
   #ifndef ONLINE_JUDGE
   freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
   
   read_data();
return 0;
}