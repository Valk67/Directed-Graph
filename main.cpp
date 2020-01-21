#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

class TimeStamp{
  public:
    TimeStamp(): d(0), f(0) {};
    int d; //discovery time
    int f; //finish time
};

class Dgraph {
  public:
    Dgraph();
    void printGraph();
    void printTranspose();
    void dfsVisit(int u);
    void dfs();
    void printGSCC();
    void printDistance();
    void printParent();
    void printColor();
    void bfs(int s);
    void transpose();
    //void buildSCC();
    void printToporder();
    void resetVectors();
    void GTdfs();
    void GTdfsVisit(int u);
    void GSCCdfsVisit(int u);
    void GSCCdfs();
    void createGSCC();
    void transposeGSCC();
    int getGSCCtoplastid();
    bool findSink();
    void printGSCCtranspose();



  private:
    vector < vector <int> > Adj;
    vector <int> parent;
    vector <int> distance;
    vector <TimeStamp> stamps;
    vector <int> colors;
    vector <int> scc;
    int size; //total vertices
    vector < vector <int> > GT; //transpose
    vector < vector <int> > GSCC;
    vector < vector <int> > GSCCGT;
    vector <int> toporder; //will be reversed make sure to inverse it
    int curID = 0;
    int time = 0;
    int GSCCtoplastindex = 0;
};


int main () {

  Dgraph g;
  //build scc
  g.dfs();
  //g.printToporder();
  g.transpose();
  g.GTdfs();
  g.createGSCC();
  //g.printGSCC();
  //cout << "hey" << endl;
  g.GSCCdfs();//step 2 and 3
  g.transposeGSCC();
  //cout << " test"  <<endl;
  //g.printGraph();

  bool answer = g.findSink();
  if (answer == false) {
    return -1;
  }
  return 0;
}

bool Dgraph::findSink() {
  bfs(GSCCtoplastindex);
  //cout << "answer" << endl;
  for (int i = 0; i < distance.size(); i++) {
    cout << "distance" << endl;
    cout << distance[i] << endl;
  }
  for (int i = 0; i < distance.size(); i++) {
    if (distance[i] == INT_MAX) {
      cout << -1 << endl;
      return false;
    }
  }
  for (int i = 0; i < scc.size(); i++) {
    if (scc[i] == GSCCtoplastindex) {
      cout << i << endl;
      return true;
    }
  }
  return true;
}


void Dgraph::GSCCdfs() {
  toporder.clear();
  resetVectors();
  for (int i = 0; i < GSCC.size(); i++) {
    if (colors[i] == 'W')
      GSCCdfsVisit(i);//t is passed by reference
  }
 //cout << '\n';
 time = 0; //reset time
 reverse(toporder.begin(),toporder.end());
 GSCCtoplastindex = toporder[toporder.size() - 1];
 //cout << GSCCtoplastindex << endl;
}

void Dgraph::GSCCdfsVisit(int u) {
  //cout << u << " ";
  colors[u] = 'G';
  stamps[u].d = time;  //distance time of u
  time = time + 1;
  for (int i = 0; i < GSCC[u].size(); i++) {
    int v = GSCC[u][i];
    if (colors[v] == 'W') {
      parent[v] = u;
      GSCCdfsVisit(v);
    }
  }
  stamps[u].f = time;//time++;   //finish time is put into stamps
  time++;
  colors[u] = 'B';
  toporder.push_back(u);
}

Dgraph::Dgraph () {
  int N, M = 0; //n = number of vertices. m = number edges
  cin >> N >> M;
  Adj.resize(N);
  int x, y = 0;
  for (int i = 0; i < M; i++) {
    cin >> x >> y;
    Adj[x].push_back(y);
  }
  size = N;

  parent.resize(size);
  stamps.resize(size);  //timestamps
  colors.resize(size);
  distance.resize(size);
  scc.resize(size, 0);
  GT.resize(size);

  for (int i = 0; i < size; i++) {
    parent[i] = i;
    colors[i] = 'W';
    distance[i] = INT_MAX;
  }
}

//------------------------------------------------------
void Dgraph::dfs() {
  resetVectors();
  for (int i = 0; i < size; i++) {
    if (colors[i] == 'W')
      dfsVisit(i);//t is passed by reference
  }
 //cout << '\n';
 time = 0; //reset time
 reverse(toporder.begin(),toporder.end());
}

void Dgraph::dfsVisit(int u) {
  //cout << u << " ";
  colors[u] = 'G';
  stamps[u].d = time;  //distance time of u
  time = time + 1;
  for (int i = 0; i < (int) Adj[u].size(); i++) {
    int v = Adj[u][i];
    if (colors[v] == 'W') {
      parent[v] = u;
      dfsVisit(v);
    }
  }
  stamps[u].f = time;//time++;   //finish time is put into stamps
  time++;
  colors[u] = 'B';
  toporder.push_back(u);
}
//------------------------------------------------------

//------------------------------------------------------

void Dgraph::GTdfs() {
  resetVectors();
  for (int u = 0; u < toporder.size(); u++) { //
    if (colors[toporder[u]] == 'W') {
      GTdfsVisit(toporder[u]);
      curID++;
    }
  }
  time = 0;//reset time

}

void Dgraph::GTdfsVisit(int u) {
  scc[u] = curID;
  colors[u] = 'G';
  stamps[u].d = time;
  time = time + 1;
  for (int i = 0; i < GT[u].size(); i++) {
    int v = GT[u][i];
    if (colors[v] == 'W') {
      parent[v] = u;
      GTdfsVisit(v);
    }
  }
  stamps[u].f = time++;
  colors[u] = 'B';
}

//---------------------------------------------------------

void Dgraph::createGSCC() {  //this creates the edges for to make GCC graph
  GSCC.resize(curID);
  //cout << "curid " << curID << endl;
  for (int i = 0; i < Adj.size(); i++) {
    for (int j = 0; j < Adj[i].size(); j++) {
      int v = Adj[i][j];
      if (scc[i] != scc[v]) {
        GSCC[scc[i]].push_back(scc[v]);
      }
    }
  }
  GSCCGT.resize(GSCC.size());
}





void Dgraph::bfs(int s) {
  queue<int> q;
  distance.clear();
  distance.resize(GSCCGT.size(),INT_MAX);
  for (int i = 0; i < GSCCGT.size(); i++) {
    parent[i] = i;
  }
  distance[s] = 0;
  int u = 0;
  q.push(s);
  while(!q.empty()) {
    u = q.front();
    q.pop();
    //cout << u << " ";

    for (int i = 0; i < GSCCGT[u].size(); i++) {
      int v = GSCCGT[u][i];
      if (distance[v] == INT_MAX) {
        distance[v] = distance[u] + 1;
        parent[v] = u;
        q.push(v);
      }
    }
  }
  //cout << '\n';
}

void Dgraph::resetVectors() {
  for (int i = 0; i < size; i++) {
    //DO NOT RESET TOPSORT
    parent[i] = i;
    colors[i] = 'W';
    distance[i] = INT_MAX;
    stamps[i].d = 0;
    stamps[i].f = 0;
  }
}

void Dgraph::printToporder() {
  for (int i = 0; i < toporder.size(); i++) {
    cout << toporder[i] << " ";
  }
  cout << '\n';
}

void Dgraph::printDistance() {
  for (int i = 0; i < distance.size(); i++) {
    cout << distance[i] << endl;
  }
}

void Dgraph::printColor() {
  for (int i = 0; i < Adj.size(); i++) {
    cout << colors[i] << " ";
  }
  cout << '\n';
}

void Dgraph::printGraph() {
  for (int i = 0; i < Adj.size(); i++) {
    for (int n = 0; n < Adj[i].size(); n++) {
      cout << Adj[i][n] << " ";
    } cout << '\n';
  }
}

void Dgraph::printGSCC() {
  for (int i = 0; i < GSCC.size(); i++) {
    for (int n = 0; n < GSCC[i].size(); n++) {
      cout << GSCC[i][n] << " ";
    } cout << '\n';
  }
}
void Dgraph::printTranspose() {
  for (int i = 0; i < GT.size(); i++) {
    for (int n = 0; n < GT[i].size(); n++) {
      cout << GT[i][n] << " ";
    } cout << '\n';
  }
}

void Dgraph::transpose() {
  for (int i = 0; i < Adj.size(); i++) {
    for (int j = 0; j < Adj[i].size(); j++) {
      int v = Adj[i][j];
      GT[v].push_back(i);
    }
  }  //cout << "transpose size is " << GT.size() << endl;
}

void Dgraph::transposeGSCC() {
  //cout << " testsss" << endl;
  for (int i = 0; i < GSCC.size(); i++) {
    for (int j = 0; j < GSCC[i].size(); j++) {
      int v = GSCC[i][j];
      GSCCGT[v].push_back(i);
    }
  }
  //cout << "gscc" <<endl;
  //printGSCC();
  //cout << "gtscc " << endl;
  //printGSCCtranspose();
}
void Dgraph::printGSCCtranspose() {
  for (int i = 0; i < GSCCGT.size(); i++) {
    for (int n = 0; n < GSCCGT[i].size(); n++) {
      cout << GSCCGT[i][n] << " ";
    } cout << '\n';
  }
}
/*
int Dgraph::getGSCCtoplastid() {
  int x = 0;
  x =  GSCCtoplastindex;
  return x;
}

void Dgraph::dfs() {
  int t = 0;
  for (int i = 0; i < size; i++) {
    colors[i] = 'W';
    parent[i] = i;
  }
  for (int i = 0; i < size; i++) {
    if (colors[i] == 'W')
      dfsVisit(i, t);//t is passed by reference
  }
 cout << '\n';
}

void Dgraph::dfsVisit(int u, int &t) {
  cout << u << " ";
  colors[u] = 'G';
  stamps[u].d = t;  //distance time of u
  t++;
  for (int i = 0; i < (int) Adj[u].size(); i++) {
    int child = Adj[u][i];
    if (colors[child] == 'W') {
      parent[child] = u;
      dfsVisit(child, t);
    }
  }
  stamps[u].f = t;   //finish time is put into stamps
  t++;
  colors[u] = 'B';
  //toporder ******need to do something with toporder
}
*/



/*
void Dgraph::tdfs() {
  resetVectors();
  for (int i = 0; i < size; i++) {
    if (colors[i] == 'W')
      tdfsVisit(i);//t is passed by reference
  }
 cout << '\n';
 time = 0;
}

void Dgraph::tdfsVisit(int u) {
  cout << u << " ";
  colors[u] = 'G';
  stamps[u].d = time;  //distance time of u
  time = time + 1;
  for (int i = 0; i < (int) GT[u].size(); i++) {
    int child = GT[u][i];
    if (colors[child] == 'W') {
      parent[child] = u;
      tdfsVisit(child);
    }
  }
  stamps[u].f = time++;;   //finish time is put into stamps
  colors[u] = 'B';
  time = 0;
  //toporder ******need to do something with toporder
}*/
