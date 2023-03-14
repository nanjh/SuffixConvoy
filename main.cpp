#include "suffixArray.h"
#include "File.h"
#include <vector>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
//#include <sys/io.h>
#include <dirent.h>
#include <sys/types.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <map>
#include <set>
#include <map>
#include <list>
#include <algorithm>

using namespace std;
set<ll> st;

//string S[N];
vector<string> vec;
vector<vector<ll>> candidate;
vector<ll> can_len;
vector<bool> dominate;
map<ll, set<ll>> c_mp;
clock_t sTime, eTime;
double refineTime;


struct cmp_key {
    bool operator()(const vector<ll> &k1, const vector<ll> &k2) const {
        if (k1.size() != k2.size()) {
            return k1.size() < k2.size();
        }
        for (ll i = 0; i < k1.size(); i++) {
            if (k1[i] != k2[i]) {
                return k1[i] < k2[i];
            }
        }
        return false;
    }
};

class Mycompare {
public:
    bool operator()(ll v1, ll v2) {
        return rL[v1] < rL[v2];
    }
};

map<vector<ll>, list<ll>, cmp_key> mp;
map<vector<ll>, list<ll>, cmp_key> mpp;

bool cmp2(ll a, ll b) {
    return Id[SA_A[a]] < Id[SA_A[b]];
}

bool cmpl(ll a, ll b) {
    return rL[a] < rL[b];
}

ll v_sub(vector<ll> a, vector<ll> b) {
    if (a.size() >= b.size()) {
        return -1;
    }
    ll ans = -1;
    ll i = 0, j = 0;
    for (j = 0; j < b.size(); j++) {
        if (i != a.size() && a[i] == b[j]) {
            if (!i) ans = j;
            i++;
        }
    }
    if (i == a.size()) return ans;
    else return -1;
}

void dfs(vector<vector<ll>> &res, vector<ll> &cmp, ll pos, ll target, map<ll, vector<ll>> mp) {
    if (pos == target) {
        res.push_back(cmp);
        return;
    } else {
        vector<ll> vec = mp[pos];
        for (ll i = 0; i < vec.size(); i++) {
            cmp.push_back(vec[i]);
            dfs(res, cmp, pos + 1, target, mp);
            cmp.pop_back();
        }
    }
}

void refine(vector<ll> vec, ll min_size, ll m_size, ll k_size, ll pos, ll expos) {
    if (pos >= m_size) {
        ll ans = pos;
        sort(vec.begin(), vec.end(), cmp2);
        set<ll> c_set;
        for (ll i = 0; i < vec.size(); i++) {
            c_set.insert(Id[SA_A[vec[i]]]);
        }
        if (c_set.size() < k_size) {
            return;
        }
        vector<ll> c_convoy;
        for (auto it: c_set) {
            c_convoy.push_back(it);
        }
        for (ll i = 0; i < vec.size(); i++) {
            vec[i] -= ans;
        }
       
        vector<ll> pconvoy;
        for (ll i = 0; i < ans; i++) {
            pconvoy.push_back(r[vec[0] + i]);
        }
        map<ll, vector<ll>> c_mp;
        ll step = -1;
        for (ll i = 0; i < vec.size(); i++) {
            if (!i || SA_A[vec[i]] != SA_A[vec[i - 1]]) {
                step++;
            }
            c_mp[step].push_back(vec[i]);
        }
        vector<vector<ll>> res;
        vector<ll> cmp;
        dfs(res, cmp, 0, c_convoy.size(), c_mp);
        sTime = clock();
        for (auto vec1: res) {
            bool is_flag = 1;
            auto iter = mpp.find(pconvoy);
            if (iter != mpp.end()) {
                list<ll> &p_can = iter->second;
                auto it = p_can.begin();
                while (it != p_can.end()) {
                    ll p_it = *it;
                    if (vec1.size() <= candidate[p_it].size()) {
                        ll step = 0;
                        for (ll i = 0; i < candidate[p_it].size(); i++) {
                            if (step < vec1.size() && vec1[step] == candidate[p_it][i]) {
                                step++;
                            }
                        }
                        if (step == vec1.size()) {
                            is_flag = 0;
                            break;
                        } else {
                            it++;
                        }
                    } else {
                        ll step = 0;
                        for (ll i = 0; i < vec1.size(); i++) {
                            if (step < candidate[p_it].size() && vec1[i] == candidate[p_it][step]) {
                                step++;
                            }
                        }
                        if (step == candidate[p_it].size()) {
                            vector<ll> c_vec;
                            for (ll i = 0; i < candidate[p_it].size(); i++) {
                                c_vec.push_back(Id[SA_A[candidate[p_it][i]]]);
                            }
                            if (mp.find(c_vec) != mp.end()) {
                                list<ll> &lt = mp[c_vec];
                                for (auto c_lt = lt.begin(); c_lt != lt.end(); c_lt++) {
                                    if (*(c_lt) == p_it) {
                                        lt.erase(c_lt);
                                        break;
                                    }
                                }
                            }
                            it = p_can.erase(it);
                        } else {
                            it++;
                        }
                    }
                }
            }
            //�ж��Ƿ���Ҫ����
            if (is_flag) {
                candidate.push_back(vec1);
                can_len.push_back(ans);
                mp[c_convoy].push_back(candidate.size() - 1);
                mpp[pconvoy].push_back(candidate.size() - 1);
            }
        }
        eTime = clock();
        refineTime += (double) (eTime - sTime) / CLOCKS_PER_SEC;
        for (ll i = 0; i < vec.size(); i++) {
            vec[i] += ans;
        }
    }
    if (pos == min_size) {
        return;
    }
    sort(vec.begin(), vec.end(), cmpl);
    ll i = 0, j = 0;
    bool is_j = 0;
    vector<ll> vec1;
    for (i = 0; i < vec.size(); i++) {
        if (i) vec1.erase(vec1.begin());
        while (j < vec.size() && ((rL[vec[j]] - rL[vec[i]]) <= expos)) {
            vec1.push_back(vec[j] + 1);
            is_j = 1;
            j++;
        }
        if (is_j && vec1.size() >= k_size) {
            is_j = 0;
            refine(vec1, min_size, m_size, k_size, pos + 1, expos);
        }
        if (j == vec.size()) {
            break;
        }
    }
    return;
}

void solve(ll *r, ll m_size, ll k_size, ll expos) {
    ll ans1 = 0;
    for (ll i = 0; i < vec.size(); i++) {
        for (ll j = 0; j <= SS_len[i]; j++) {
            SA_A[ans1++] = i;
        }
    }
    ll i, j;
    ll maxn = m_size;
    ll minn = N;
    for (ll k = 1; k < rlen; k++) {
        if (heigt[k] >= m_size) {
            minn = min(minn, heigt[k]);
        }
        maxn = max(maxn, heigt[k]);
    }
    for (ll threshold = minn; threshold <= maxn; threshold++) {
        for (i = 1; i < rlen;) {
            if (heigt[i] >= threshold) {
                st.insert(SA_A[SA[i - 1]]);
                j = 0;
                ll min_size = heigt[i];
                while (i + j < rlen && heigt[i + j] >= threshold) {
                    min_size = min(min_size, heigt[i + j]);
                    st.insert(SA_A[SA[i + j]]);
                    j++;
                }
                if (st.size() >= k_size) {
                    vector<ll> vec1;
                    for (ll k = i - 1; k < i + j; k++) {
                        vec1.push_back(SA[k]);
                    }
                    refine(vec1, min_size, m_size, k_size, 0, expos);
                }
                st.clear();
                i += j + 1;
            } else i++;
        }
    }
}

void refineSub_new(ll k, ll expos) {
    ll count = 0;
    map<vector<ll>, set<ll, Mycompare>, cmp_key> new_mp;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        list<ll> list = it->second;
        vector<ll> vec;
        for (ll i: list) {
            ll ans = can_len[i];
            for (ll j = 0; j < ans; j++) {
                vec.push_back(r[candidate[i][0] + j]);
            }

            for (ll j = 0; j < candidate[i].size(); j++) {
                new_mp[vec].insert(candidate[i][j]);
            }
            vec.clear();
        }
    }

    for (auto it = new_mp.begin(); it != new_mp.end(); it++) {
        set<ll, Mycompare> st = it->second;
        vector<ll> v1;
        vector<ll> v2 = it->first;
        auto it2 = st.begin();
        bool flag = 1;
        for (auto it1 = st.begin(); it1 != st.end(); it1++) {
            if (it1 != st.begin()) v1.erase(v1.begin());
            while (rL[*it2] - rL[*it1] <= expos && it2 != st.end()) {
                flag = 0;
                v1.push_back(*it2);
                it2++;
            }
            if (!flag && v1.size() >= k) {
                count++;
/*                cout<<"[";
                for(ll i=0;i<v1.size();i++){
                    if(i) cout<<" ";
                    cout<<Id[SA_A[v1[i]]];
                }
                cout<<"]:";
                for(ll i=0;i<v2.size();i++){
                    cout<<v2[i]<<" ";
                }
                cout<<endl;*/
                flag = 1;
            }
            if (it2 == st.end()) {
                break;
            }
        }
    }
    cout << count << endl;
}

void refineSub() {
    //v_can����δ���ǰ�����ϵ֮ǰ��convoy�ĳ�������
    vector<vector<ll>> v_can;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        v_can.push_back(it->first);
    }

    
    for (auto vec: v_can) {
        list<ll> list = mp[vec];
        for (ll i: vec) {
            for (auto it: list) {
                c_mp[i].insert(it);
            }
        }
    }
/*    ll maxn=0;
    for(auto it:c_mp){
        set<ll> exp = it.second;
        if(maxn<exp.size()){
            maxn = exp.size();
        }
    }
    cout<<maxn<<endl;*/
    //ʹ�õ��ű��󽻼����ҵ���Ӧ�������ϵĸ���
    for (auto vec: v_can) {
        set<ll> res;
        set<ll> exp = c_mp[vec[0]];
        for (ll i = 1; i < vec.size(); i++) {
            res.clear();
            set_intersection(exp.begin(), exp.end(), c_mp[vec[i]].begin(), c_mp[vec[i]].end(),
                             inserter(res, res.begin()));
            exp = res;
        }
        for (auto it: res) {
            if (candidate[it].size() != vec.size()) {
                ll ans = 0;
                list<ll> &l1 = mp[vec];
                auto iter = l1.begin();
                while (iter != l1.end()) {
                    ans = 0;
                    bool flag = 1;
                    for (ll i = 0; i < vec.size(); i++) {
                        while (Id[SA_A[candidate[it][ans]]] != vec[i]) {
                            ans++;
                        }
                        if (!(candidate[it][ans] <= candidate[*iter][i] &&
                              candidate[it][ans] + can_len[it] >= candidate[*iter][i] + can_len[*iter])) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag) {
                        l1.erase(iter++);
                    } else {
                        iter++;
                    }
                }
            }
        }
    }
    auto it = mp.begin();
    while (it != mp.end()) {
        list<ll> l1 = it->second;
        if (!l1.size()) {
            it = mp.erase(it);
        } else it++;
    }

}

void refine_new3() {
    for (auto it = mpp.begin(); it != mpp.end(); it++) {
        list<ll> &p_list = it->second;
        list<ll> new_list;
        for (auto it1 = p_list.begin(); it1 != p_list.end(); it1++) {
            ll pastcan = *it1;
            bool flag = 1;
            auto it2 = new_list.begin();
            while (it2 != new_list.end()) {
                ll newcan = *it2;
                if (candidate[pastcan].size() <= candidate[newcan].size()) {
                    ll step = 0;
                    for (ll i = 0; i < candidate[newcan].size(); i++) {
                        if (step < candidate[pastcan].size() && candidate[pastcan][step] == candidate[newcan][i]) {
                            step++;
                        }
                    }
                    if (step == candidate[pastcan].size()) {
                        flag = 0;
                        break;
                    } else {
                        it2++;
                    }
                } else {
                    ll step = 0;
                    for (ll i = 0; i < candidate[pastcan].size(); i++) {
                        if (step < candidate[newcan].size() && candidate[newcan][step] == candidate[pastcan][i]) {
                            step++;
                        }
                    }
                    if (step == candidate[newcan].size()) {
                        it2 = new_list.erase(it2);
                    } else {
                        it2++;
                    }
                }
            }
            if (flag) {
                new_list.push_back(pastcan);
            }
        }
        it->second = new_list;
    }
}

void printCandidate() {
    ll count = 0;
    ll countobj = 0;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        vector<ll> vec = it->first;
        list<ll> vec1 = it->second;
        printf("[");
        for (ll i = 0; i < vec.size(); i++) {
            printf("%lld ", vec[i]);
        }
        countobj++;
        printf("] : ");
        //printf(" %lld ",vec1.size());
        for (auto &ans: vec1) {
            count++;
            printf("{");
            for (ll k = 0; k < can_len[ans]; k++) {
                if (k) {
                    printf(",");
                }
                printf("%lld", r[candidate[ans][0] + k]);
            }
            printf("}");
        }
        printf("\n");
    }
    printf("�������ϵ�����:%lld\n", countobj);
    printf("����convoy������Ϊ:%lld\n", count);
}

void pCan() {
    ll count = 0;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        vector<ll> vec = it->first;
        list<ll> vec1 = it->second;

        for (auto i: vec1) {
            count++;
                     for(auto i:vec){
                         printf("%lld ",i);
                     }
                     printf(" :");
                     for(ll j=0;j<can_len[i];j++) {
                         printf("%lld ", r[candidate[i][0] + j]);
                     }
                     printf("\n");
        }
    }
    //printf("�������ϵ�����:%lld\n",countobj);
    printf("total convoy : %lld\n", count);
}

void printCsv() {
    ofstream dataFile;
    dataFile.open("E:\\Code\\SuffixArray_hu\\dataset\\Singapore_m=2_k=3_e=600_1.csv");
    dataFile << "Object" << "," << "Convoy" << endl;
    for (auto it = mpp.begin(); it != mpp.end(); it++) {
        vector<ll> vec = it->first;
        list<ll> l1 = it->second;
        for (auto iter: l1) {
            for (ll i = 0; i < candidate[iter].size(); i++) {
                if (i) dataFile << " ";
                dataFile << Id[SA_A[candidate[iter][i]]];
            }
            dataFile << ",";
            for (ll i = 0; i < vec.size(); i++) {
                if (i) dataFile << " ";
                dataFile << vec[i];
            }
            dataFile << endl;
        }

    }
}

void refine_new4() {
    for (auto it = mp.begin(); it != mp.end(); it++) {
        list<ll> lt = it->second;
        list<ll> new_list;
        for (auto iter = lt.begin(); iter != lt.end(); iter++) {
            ll pastcan = *iter;
            bool is_flag = 1;
            auto iter2 = new_list.begin();
            while (iter2 != new_list.end()) {
                ll newcan = *iter2;
                if (candidate[pastcan][0] <= candidate[newcan][0] &&
                    candidate[pastcan][0] + can_len[pastcan] >= candidate[newcan][0] + can_len[newcan]) {
                    bool flag = 1;
                    for (ll i = 1; i < candidate[pastcan].size(); i++) {
                        if (!(candidate[pastcan][i] <= candidate[newcan][i] &&
                              candidate[pastcan][i] + can_len[pastcan] >= candidate[newcan][i] + can_len[newcan])) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag) {
                        iter2 = new_list.erase(iter2);
                    } else {
                        iter2++;
                    }
                } else if (candidate[pastcan][0] >= candidate[newcan][0] &&
                           candidate[pastcan][0] + can_len[pastcan] <= candidate[newcan][0] + can_len[newcan]) {
                    bool flag = 1;
                    for (ll i = 1; i < candidate[pastcan].size(); i++) {
                        if (!(candidate[pastcan][i] >= candidate[newcan][i] &&
                              candidate[pastcan][i] + can_len[pastcan] <= candidate[newcan][i] + can_len[newcan])) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag) {
                        is_flag = 0;
                        break;
                    } else {
                        iter2++;
                    }
                } else {
                    iter2++;
                }
            }
            if (is_flag) {
                new_list.push_back(pastcan);
            }
        }
        it->second = new_list;
    }
}

void dupConvoy() {
    vector<vector<ll>> objSet;
    for (auto it = mp.begin(); it != mp.end(); it++) {
        objSet.push_back(it->first);
    }
    map<ll, set<ll>> invertMap;
    for (ll i = 0; i < objSet.size(); i++) {
        vector<ll> objVec = objSet[i];
        for (ll id: objVec) {
            invertMap[id].insert(i);
        }
    }
    for (ll ans = 0; ans < objSet.size(); ans++) {
        vector<ll> objVec = objSet[ans];
        list<ll> &objId = mp[objVec];
        set<ll> faSet = invertMap[objVec[0]];
        for (ll i = 1; i < objVec.size(); i++) {
            set<ll> midSet;
            set_intersection(faSet.begin(), faSet.end(), invertMap[objVec[i]].begin(), invertMap[objVec[i]].end(),
                             inserter(midSet, midSet.begin()));
            faSet = midSet;
        }
        for (ll fa: faSet) {
            if (fa != ans) {
                list<ll> faObjId = mp[objSet[fa]];
                auto it = objId.begin();
                while(it != objId.end()) {
                    ll id = *it;
                    for(auto faId : faObjId) {

                    }
                }
            }
        }
    }
}

int main() {
    clock_t start_time, end_time, end_time1;
    start_time = clock();
    string path = "../dataset/video_sg/t14_c600_len16";
    GetFileNames(path, vec);
    getData(vec);
    gethzsz(r, 1005010, rlen);
    getheigt(r, rlen);
    end_time1 = clock();
    //printf("��ȡ�ļ������׺���� : %lf\n", (double) (end_time1 - start_time) / CLOCKS_PER_SEC);
    solve(r, 3, 2, 600);
    //refine_new3()
    sTime = clock();
    //printf("��ѯʱ�� : %lf\n", (double) (sTime - end_time1) / CLOCKS_PER_SEC - refineTime);
    refine_new4();
    eTime = clock();
    //refineTime += (double) (eTime - sTime) / CLOCKS_PER_SEC;
    //printf("ɸѡ : %lf\n", refineTime);
    //refineSub();
    //printCandidate();
    //refineSub_new(2,600);
    pCan();
    //printCsv();
    end_time = clock();
    printf("running time : %lf(s)\n", (double) (end_time - start_time) / CLOCKS_PER_SEC);
    return 0;
}
