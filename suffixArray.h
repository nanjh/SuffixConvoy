//
// Created by lenovo on 2022/10/8.
//
#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H
typedef long long ll;
const ll N = 10000000;

ll SA[N], heigt[N], c[N], rk[N], X[N], Y[N], r[N],rL[N],rR[N], num, rlen, SA_A[N], pos, pos1;
ll cmp(ll* r, ll a, ll b, ll l)
{
    return r[a] == r[b] && r[a + l] == r[b + l];
}
void gethzsz(ll* r, ll m,ll n)
{
    ll i, j, p; ll* tem, * x = X, * y = Y;//x y分别代表数组X Y的首地址
    for (i = 0; i < m; i++)c[i] = 0; //计数排序法
    for (i = 0; i < n; i++)
        c[r[i]]++, x[i] = r[i];
    for (i = 1; i < m; i++)c[i] += c[i - 1];
    for (i = n - 1; i >= 0; i--) SA[--c[x[i]]] = i;//得到初始SA

    for (j = 1; j <= n; j <<= 1)// j代表当前所求长度
    {
        for (p = 0, i = n - j; i < n; i++)y[p++] = i;//将后缀长度小于等于当前长度的放在
        for (i = 0; i < n; i++)
            if (SA[i] >= j)y[p++] = SA[i] - j;//后移操作 在第一关键序的基础上排第二关键序
        for (i = 0; i < m; i++)c[i] = 0;   //实质是 因为第一关键序字串后移当前长度直接作为第二关键序
        for (i = 0; i < n; i++)c[x[i]]++;
        for (i = 1; i < m; i++)c[i] += c[i - 1];
        for (i = n - 1; i >= 0; i--)
            SA[--c[x[y[i]]]] = y[i];//按第二关键字排序
        tem = x; x = y; y = tem; x[SA[0]] = 0; p = 1;
        for (i = 1; i < n; i++)
            x[SA[i]] = cmp(y, SA[i], SA[i - 1], j) ? p - 1 : p++;
        if (p > n)break;
        m = p;
    }
}
void getheigt(ll* r, ll n)
{
    ll i, j, k = 0; heigt[0] = 0;
    for (i = 0; i < n; i++)rk[SA[i]] = i;
    for (i = 0; i < n; i++)
    {
        if (!rk[i])continue;//排除 0首项
        if (k)--k;
        j = SA[rk[i] - 1];//j=当前一个名次字串前一个字串（ 看是否位该串前缀）
        while (i + k < n && j + k < n && r[i + k] == r[j + k])k++;
        heigt[rk[i]] = k;//当前坐标 的前缀
    }
}

#endif // SUFFIXARRAY_H