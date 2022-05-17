// https://zerojudge.tw/ShowProblem?problemid=d539
// https://66lemon66.blogspot.com/2021/01/zerojudge-d539-max-c.html
#include<iostream>
#include<vector>
using namespace std;

vector<int> segmentTree(2e6, 0);
vector<int> v;

void buildTree(int left, int right, int vertex = 0)//建立線段樹
{
    /*邊界條件*/
    if(left == right){
        segmentTree[vertex] = v[left];
        return;
    }
    /*對左子樹和右子樹遞迴建樹*/
    int middle = (left + right) / 2;
    buildTree(left, middle, 2*vertex + 1);
    buildTree(middle + 1, right, 2*vertex + 2);
    /*各節點儲存其所代表區間中元素的最大值*/
    segmentTree[vertex] = max(segmentTree[2*vertex+1], segmentTree[2*vertex+2]);
}

/*left跟right代表當前節點的區間範圍，tLeft跟tRigth代表欲查詢的區間範圍，vertex為當前在線段樹的節點*/
int query(int left, int right, int tLeft, int tRight, int vertex)
{
    /*當遞迴到滿足查詢範圍*/
    if(left == tLeft && right == tRight){
        return segmentTree[vertex];
    }
    int middle = (left + right) / 2;
    
    if(tRight <= middle){/*如果欲查詢的區間範圍完全在前半段*/
        return query(left, middle, tLeft, tRight, vertex * 2 + 1);
    }else if(tLeft > middle){/*如果欲查詢的區間範圍完全在後半段*/
        return query(middle + 1, right, tLeft, tRight, vertex * 2 + 2);
    }
    /*如果欲查詢的範圍橫跨左右半段*/
    return max(query(left, middle, tLeft, middle, vertex * 2 + 1), query(middle + 1, right, middle + 1, tRight, vertex * 2 + 2));
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int num;
    cin >> num;
    for(int i = 0; i < num; i++){
        int t;
        cin >> t;
        v.push_back(t);
    }
    /*建樹(邊界採左閉右閉)*/
    buildTree(0, num-1);
    // for(int i = 0; i < 40; i++)
    //     cout << segmentTree[i] << " ";
    // cout << endl;
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int l, r;
        cin >> l >> r;
        if(l > r)
            swap(l, r);
        /*印出各欲查詢區段的最大元素*/
        cout << query(0, num - 1, l-1, r-1, 0) << endl;
    }
    return 0;
}