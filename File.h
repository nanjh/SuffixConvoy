//
// Created by lenovo on 2022/10/8.
//
#include"suffixArray.h"

#include <string.h>

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
#include <fstream>
#include <iostream>
#include <time.h>
#include <set>
#include <math.h>
#include <algorithm>
#define M 5000+10
typedef long long ll;
using namespace std;

ll SS[M][M][4],Id[N];
ll SS_len[M];
ll maxLen = 0;


/**
 * 读取path路径下的文件并用filenames保存
 * @param path
 * @param filenames
 */
void GetFileNames(string path, vector<string> &filenames) {
    DIR *pDir;
    struct dirent *ptr;
    if (!(pDir = opendir(path.c_str())))
        return;
    while ((ptr = readdir(pDir)) != 0) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0)
            filenames.push_back(path + "/" + ptr->d_name);
    }
    closedir(pDir);
}
/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/
ll getNum(string s){
    ll pos = 0;
    ll j=0;
    for(ll i=s.size()-5;i>=0;i--){
        if(s[i]>='0'&&s[i]<='9'){
            pos+=pow(10,j)*(s[i]-'0');
            j++;
        }
        else break;
    }
    return pos;
}
/*void getAllFiles(string path, vector<string>& files,string fileType)
{
    // 文件句柄
    ll hFile = 0;
    // 文件信息
    struct _finddata_t fileinfo;

    string p;

    if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
        do {
            // 保存文件的全路 径
            files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            //cout<<fileinfo.name<<endl;
        } while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

        _findclose(hFile);
    }
}//获取指定路径下指定格式的所有文件*/


void getData(vector<string>& vec){
    rlen = 0;
    for(ll i = 0;i< vec.size();i++){
        ll ID = getNum(vec[i]);
        Id[i] = ID;
        ifstream fin;
        string buffer;
        fin.open(vec[i],ios::in);
        ll len = 0;
        while(getline(fin,buffer)){
            pos = 0;
            pos1 = 0;
            for(ll j=0;j<buffer.size();j++){
                if(buffer[j]!=','&&buffer[j]!='\r'){
                    pos = 10 * pos + buffer[j]-'0';
                }else{
                    SS[i][len][pos1] = pos;
                    if(pos1==0){
                        r[rlen] = pos;
                    }
                    else if(pos1 == 1){
                        rL[rlen] = pos;
                    }
                    pos = 0;
                    pos1++;
                }
            }
            rR[rlen] = pos;
            rlen++;
            len ++;
        }
        SS_len[i] = len;
        r[rlen++] = 10000000001+i;
    }
}

