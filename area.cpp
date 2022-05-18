#include<iostream>
#include<algorithm>
using namespace std;
#define maxN 100000<<2
struct rect{
    int x, y1, y2, val;
}rec[maxN];

int lazy[maxN];
int yy[maxN];
int length[maxN];

void push_up(int vertex, int l, int r)
{
    if(lazy[vertex]){//如果一個節點有需要被計算長度
        length[vertex] = yy[r] - yy[l];
    }else if(l == r){//保險一下，確保當前不是一個點
        length[vertex] = 0;
    }else{
        //結合左右子節點的答案
        length[vertex] = length[vertex*2+1] + length[vertex*2+2];
    }
}

void update(int vertex, int ql, int qr, int l, int r, int val)
{
    if(ql == l && qr == r){//當前節點區間範圍和欲查詢範圍相同
        lazy[vertex] += val;
        push_up(vertex, l, r);
        return;
    }
    if(l+1==r) return;//最小區間，在此題中一個"點"沒有意義。因此最深只遞迴到長度為一的線
    int mid = (l + r) / 2;
    if(qr <= mid){/*如果欲查詢的區間範圍完全在前半段*/
        update(vertex*2+1, ql, qr, l, mid, val);
    }else if(ql > mid){/*如果欲查詢的區間範圍完全在後半段*/
        update(vertex*2+2, ql, qr, mid, r, val);
    }else{
        /*如果欲查詢的範圍橫跨左右半段*/
        update(vertex*2+1, ql, mid, l, mid, val);
        update(vertex*2+2, mid, qr, mid, r, val);
    }
    push_up(vertex, l, r);
}

int main()
{
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int x1, x2, y1, y2;
        cin >> x1 >> x2 >> y1 >> y2;
        rec[i*2].x = x1; rec[i*2].y1 = y1; rec[i*2].y2 = y2; rec[i*2].val = 1;
        yy[i*2] = y1;
        rec[i*2+1].x = x2; rec[i*2+1].y1 = y1; rec[i*2+1].y2 = y2; rec[i*2+1].val = -1;
        yy[i*2+1] = y2;
    }
    sort(rec, rec + 2 * t, [](rect& a, rect& b){return a.x < b.x;});
    sort(yy, yy + 2 * t);
    long long len = unique(yy, yy+2*t) - yy;
    long long ans = 0;
    long long x = 0, y = 0;
    for(int i = 0; i < 2 * t; i++){
        ans += (rec[i].x - x) * y;
        int yl = lower_bound(yy, yy + len, rec[i].y1) - yy;//離散化
        int yr = lower_bound(yy, yy + len, rec[i].y2) - yy;//離散化
        update(0, yl, yr, 0, len-1, rec[i].val);
        x = rec[i].x;
        y = length[0];
    }
    cout << ans;
}