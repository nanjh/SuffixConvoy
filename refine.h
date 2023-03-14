//
// Created by lenovo on 2023/1/2.
//
typedef long long ll;
#include<bits/stdc++.h>
void refine_new4(){
    for(auto it = mp.begin(); it != mp.end(); it++){
        list<ll> lt = it->second;
        list<ll> new_list;
        for(auto iter = lt.begin(); iter != lt.end(); iter++){
            ll pastcan = *iter;
            bool is_flag = 1;
            auto iter2 = new_list.begin();
            while(iter2 != new_list.end()){
                ll newcan = *iter2;
                if(candidate[pastcan][0] <= candidate[newcan][0] &&
                   candidate[pastcan][0] + can_len[pastcan] >= candidate[newcan][0] + can_len[newcan]){
                    bool flag = 1;
                    for(ll i = 1;i < candidate[pastcan].size(); i++){
                        if(!(candidate[pastcan][i] <= candidate[newcan][i] &&
                             candidate[pastcan][i] + can_len[pastcan] >= candidate[newcan][i] + can_len[newcan])){
                            flag = 0;
                            break;
                        }
                    }
                    if(flag){
                        iter2 = new_list.erase(iter2);
                    }
                    else{
                        iter2++;
                    }
                }
                else if(candidate[pastcan][0] >= candidate[newcan][0] &&
                        candidate[pastcan][0] + can_len[pastcan] <= candidate[newcan][0] + can_len[newcan]){
                    bool flag = 1;
                    for(ll i = 1;i < candidate[pastcan].size(); i++){
                        if(!(candidate[pastcan][i] >= candidate[newcan][i] &&
                             candidate[pastcan][i] + can_len[pastcan] <= candidate[newcan][i] + can_len[newcan])){
                            flag = 0;
                            break;
                        }
                    }
                    if(flag){
                        is_flag = 0;
                        break;
                    }
                    else{
                        iter2++;
                    }
                }
                else{
                    iter2++;
                }
            }
            if(is_flag){
                new_list.push_back(pastcan);
            }
        }
        it->second = new_list;
    }
}