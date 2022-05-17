#include<iostream>
#include<vector>
using namespace std;
class SegmentTree
{
private:
    /* data */
    #define maxN 1000
    int maxVal[maxN] = {0};
    int minVal[maxN] = {0};
    int sumVal[maxN] = {0};
    int lazyAdd[maxN] = {0};
    int lazyChg[maxN] = {0};
    int st[maxN] = {0};
public:

    SegmentTree(vector<int>& v, int l, int r)
    {
        buildTree(v, 0, l, r);
    };
    ~SegmentTree(){};

    void push_up(int vertex);
    void push_down(int vertex, int l, int r);
    void buildTree(vector<int>& v, int vertex, int l, int r);
    void updateAdd(int vertex, int ql, int qr, int l, int r, int addVal);
    void updateChg(int vertex, int ql, int qr, int l, int r, int chgVal);
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
    //先加後改
    //將lazy tag傳遞給子節點
    if(lazyAdd[vertex] != 0){
        lazyAdd[vertex * 2 + 1] += lazyAdd[vertex];
        lazyAdd[vertex * 2 + 2] += lazyAdd[vertex];
        maxVal[vertex * 2 + 1] += lazyAdd[vertex];
        maxVal[vertex * 2 + 2] += lazyAdd[vertex];
        minVal[vertex * 2 + 1] += lazyAdd[vertex];
        minVal[vertex * 2 + 2] += lazyAdd[vertex];
        int middle = (r + l) / 2;
        sumVal[vertex * 2 + 1] += lazyAdd[vertex] * (middle - l + 1);
        sumVal[vertex * 2 + 2] += lazyAdd[vertex] * (r - middle);
        
        lazyAdd[vertex] = 0;
    }    
    if(lazyChg[vertex] == 0) return;
    else{
        lazyChg[vertex * 2 + 1] = lazyChg[vertex];
        lazyChg[vertex * 2 + 2] = lazyChg[vertex];
        maxVal[vertex * 2 + 1] = lazyChg[vertex];
        maxVal[vertex * 2 + 2] = lazyChg[vertex];
        minVal[vertex * 2 + 1] = lazyChg[vertex];
        minVal[vertex * 2 + 2] = lazyChg[vertex];
        int middle = (r + l) / 2;
        sumVal[vertex * 2 + 1] = lazyChg[vertex] * (middle - l + 1);
        sumVal[vertex * 2 + 2] = lazyChg[vertex] * (r - middle);
        
        lazyChg[vertex] = 0;
        lazyAdd[vertex] = 0;
    }
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

void SegmentTree::updateAdd(int vertex, int ql, int qr, int l, int r, int addVal)
{
    if(ql == l && qr == r){//當前區間為欲更新區間，直接更新後結束
        lazyAdd[vertex] += addVal;
        maxVal[vertex] += addVal;
        minVal[vertex] += addVal;
        sumVal[vertex] += addVal * (r - l + 1);
        return ;
    }
    push_down(vertex, l, r);//向下傳遞lazy tag
    int middle = (l + r) / 2;
    if(qr <= middle){/*如果欲查詢的區間範圍完全在前半段*/
        updateAdd(vertex * 2 + 1, ql, qr, l, middle, addVal);
    }else if(ql > middle){/*如果欲查詢的區間範圍完全在後半段*/
        updateAdd(vertex * 2 + 2, ql, qr, middle + 1, r, addVal);
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        updateAdd(vertex * 2 + 1, ql, middle, l, middle, addVal);
        updateAdd(vertex * 2 + 2, middle + 1, qr, middle + 1, r, addVal);
    }
    //向上合併子節點的結果
    push_up(vertex);
}

void SegmentTree::updateChg(int vertex, int ql, int qr, int l, int r, int chgVal)
{
    if(ql == l && qr == r){//當前區間為欲更新區間，直接更新後結束
        lazyChg[vertex] = chgVal;
        maxVal[vertex] = chgVal;
        minVal[vertex] = chgVal;
        sumVal[vertex] = chgVal * (r - l + 1);
        return ;
    }
    push_down(vertex, l, r);//向下傳遞lazy tag
    int middle = (l + r) / 2;
    if(qr <= middle){/*如果欲查詢的區間範圍完全在前半段*/
        updateChg(vertex * 2 + 1, ql, qr, l, middle, chgVal);
    }else if(ql > middle){/*如果欲查詢的區間範圍完全在後半段*/
        updateChg(vertex * 2 + 2, ql, qr, middle + 1, r, chgVal);
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        updateChg(vertex * 2 + 1, ql, middle, l, middle, chgVal);
        updateChg(vertex * 2 + 2, middle + 1, qr, middle + 1, r, chgVal);
    }
    //向上合併子節點的結果
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
        return std::min(mini(vertex * 2 + 1, ql, middle, l, middle), mini(vertex * 2 + 2, middle + 1, qr, middle + 1, r));
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
    vector<int> v;
    for(int i = 0; i < 10; i++){
        v.push_back(i+1);
    }
    SegmentTree s(v, 0, 9);
    s.buildTree(v, 0, 0, v.size()-1);
    cout << s.sum(0, 0, 9, 0, 9) << endl;
    s.updateAdd(0, 2, 2, 0, v.size()-1, 30);
    s.updateChg(0, 2, 2, 0, v.size()-1, 90);
    s.updateAdd(0, 2, 2, 0, v.size()-1, 30);
    // s.updateAdd(0, 2, 4, 0, v.size()-1, 20); //60 + 90 - 45 - 20//60 + 60 - 27 + 12
    // s.updateAdd(0, 3, 5, 0, v.size()-1, 30);
    // s.updateAdd(0, 1, 5, 0, v.size()-1, -9);
    // s.updateAdd(0, 7, 8, 0, v.size()-1, -10);
    int l, r;
    cin >> l >> r;
    cout << s.maxi(0, l-1, r-1, 0, v.size()-1) << " "  << s.mini(0, l-1, r-1, 0, v.size()-1) << " " << s.sum(0, l-1, r-1, 0, v.size()-1) << endl;;
}