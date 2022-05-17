#include<iostream>
#include<vector>
using namespace std;
class SegmentTree
{
private:
    /* data */
    #define maxN 1000
    
    
    int st[maxN] = {0};
public:
int maxVal[maxN] = {0};
int minVal[maxN] = {0};
int sumVal[maxN] = {0};
int lazy[maxN] = {0};
    SegmentTree(){};
    ~SegmentTree(){};

    void push_up(int vertex);
    void push_down(int vertex, int l, int r);
    void buildTree(vector<int>& v, int vertex, int l, int r);
    void update(int vertex, int ql, int qr, int l, int r, int addVal);
    int maxi(int vertex, int ql, int qr, int l, int r);
    int mini(int vertex, int ql, int qr, int l, int r);
    int sum(int vertex, int ql, int qr, int l, int r);
};

void SegmentTree::push_up(int vertex)
{
    //合併子節點的結果
    maxVal[vertex] = max(maxVal[vertex * 2 + 1], maxVal[vertex * 2 + 2]);
    minVal[vertex] = min(minVal[vertex * 2 + 1], minVal[vertex * 2 + 2]);
    sumVal[vertex] = (sumVal[vertex * 2 + 1] + sumVal[vertex * 2 + 2]);
}

void SegmentTree::push_down(int vertex, int l, int r)
{
    //將lazy tag傳遞給子節點
    if(lazy[vertex] == 0) return;
    lazy[vertex * 2 + 1] += lazy[vertex];
    lazy[vertex * 2 + 2] += lazy[vertex];
    maxVal[vertex * 2 + 1] += lazy[vertex];
    maxVal[vertex * 2 + 2] += lazy[vertex];
    minVal[vertex * 2 + 1] += lazy[vertex];
    minVal[vertex * 2 + 2] += lazy[vertex];
    int middle = (r + l) / 2;
    sumVal[vertex * 2 + 1] += lazy[vertex] * (middle - l + 1);
    sumVal[vertex * 2 + 2] += lazy[vertex] * (r - middle);
    lazy[vertex] = 0;
}

void SegmentTree::buildTree(vector<int>& v, int vertex, int l, int r)
{
    if(l == r){
        maxVal[vertex] = v[r];
        minVal[vertex] = v[r];
        sumVal[vertex] = v[r];
        return;
    }
    int middle = (l + r) / 2;
    buildTree(v, vertex * 2 + 1, l, middle);
    buildTree(v, vertex * 2 + 2, middle + 1, r);
    //將子節點的結果合併後回傳給父節點
    push_up(vertex);
}

void SegmentTree::update(int vertex, int ql, int qr, int l, int r, int addVal)
{
    if(ql == l && qr == r){
        lazy[vertex] += addVal;
        maxVal[vertex] += addVal;
        minVal[vertex] += addVal;
        sumVal[vertex] += addVal * (r - l + 1);
        return ;
    }
    push_down(vertex, l, r);
    int middle = (l + r) / 2;
    if(qr <= middle){
        update(vertex * 2 + 1, ql, qr, l, middle, addVal);
    }else if(ql > middle){
        update(vertex * 2 + 2, ql, qr, middle + 1, r, addVal);
    }else{
        update(vertex * 2 + 1, ql, middle, l, middle, addVal);
        update(vertex * 2 + 2, middle + 1, qr, middle + 1, r, addVal);
    }
    push_up(vertex);
}

int SegmentTree::maxi(int vertex, int ql, int qr, int l, int r)
{
    // if(l != r)//當前區間為一個點，不可再往下找最大值(因為該點為leaf)
    //     push_up(vertex);
    
    if(ql == l && qr == r){//當前節點範圍等同欲查詢區間，直接回傳結果
        return maxVal[vertex];
    }
    push_down(vertex, l, r);//將lazy tag往下傳遞

    int middle = (l + r) / 2;
    if(qr <= middle){
        return maxi(vertex * 2 + 1, ql, qr, l, middle);/*如果欲查詢的區間範圍完全在前半段*/
    }else if(ql > middle){
        return maxi(vertex * 2 + 2, ql, qr, middle + 1, r);/*如果欲查詢的區間範圍完全在後半段*/
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        return std::max(maxi(vertex * 2 + 1, ql, middle, l, middle), maxi(vertex * 2 + 2, middle + 1, qr, middle + 1, r));
    }
}

int SegmentTree::mini(int vertex, int ql, int qr, int l, int r)
{
    // if(l != r)//當前區間為一個點，不可再往下找最大值(因為該點為leaf)
    //     push_up(vertex);
    
    if(ql == l && qr == r){//當前節點範圍等同欲查詢區間，直接回傳結果
        return minVal[vertex];
    }
    push_down(vertex, l, r);//將lazy tag往下傳遞

    int middle = (l + r) / 2;
    if(qr <= middle){
        return mini(vertex * 2 + 1, ql, qr, l, middle);/*如果欲查詢的區間範圍完全在前半段*/
    }else if(ql > middle){
        return mini(vertex * 2 + 2, ql, qr, middle + 1, r);/*如果欲查詢的區間範圍完全在後半段*/
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        return std::min(maxi(vertex * 2 + 1, ql, middle, l, middle), maxi(vertex * 2 + 2, middle + 1, qr, middle + 1, r));
    }
}

int SegmentTree::sum(int vertex, int ql, int qr, int l, int r)
{
    // if(l != r)//當前區間為一個點，不可再往下找最大值(因為該點為leaf)
    //     push_up(vertex);
    
    if(ql == l && qr == r){//當前節點範圍等同欲查詢區間，直接回傳結果
        return sumVal[vertex];
    }
    push_down(vertex, l, r);//將lazy tag往下傳遞

    int middle = (l + r) / 2;
    if(qr <= middle){
        return sum(vertex * 2 + 1, ql, qr, l, middle);/*如果欲查詢的區間範圍完全在前半段*/
    }else if(ql > middle){
        return sum(vertex * 2 + 2, ql, qr, middle + 1, r);/*如果欲查詢的區間範圍完全在後半段*/
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        return (sum(vertex * 2 + 1, ql, middle, l, middle) + sum(vertex * 2 + 2, middle + 1, qr, middle + 1, r));
    }
}

int main()
{
    SegmentTree s;
    vector<int> v;
    for(int i = 0; i < 10; i++){
        v.push_back(i+1);
    }
    s.buildTree(v, 0, 0, v.size()-1);
    cout << s.sum(0, 0, 9, 0, 9) << endl;
    s.update(0, 2, 4, 0, v.size()-1, 20); //60 + 90 - 45 - 20//60 + 60 - 27 + 12
    s.update(0, 3, 5, 0, v.size()-1, 30);
    s.update(0, 1, 5, 0, v.size()-1, -9);
    s.update(0, 7, 8, 0, v.size()-1, -10);
    for(int i = 0; i < 30; i++){
        cout << s.maxVal[i] << " ";
    }
    cout << endl;
    int l, r;
    cin >> l >> r;
    cout << s.maxi(0, l-1, r-1, 0, v.size()-1) << " "  << s.mini(0, l-1, r-1, 0, v.size()-1) << " " << s.sum(0, l-1, r-1, 0, v.size()-1) << endl;;
    for(int i = 0; i < 30; i++)
        cout << s.lazy[i] << " ";
    cout << endl;
    for(int i = 0; i < 30; i++){
        cout << s.sumVal[i] << " ";
    }
    // cout << endl;
    // cout << s.maxi(0, 3, 3, 0, v.size()-1) << endl;;
    // for(int i = 0; i < 30; i++)
    //     cout << s.lazy[i] << " ";
    // cout << endl;
    // for(int i = 0; i < 30; i++){
    //     cout << s.maxVal[i] << " ";
    // }
}