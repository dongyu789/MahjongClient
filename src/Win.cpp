#include "Win.h"
#include <vector>
using namespace std;



int Win::findNext(vector<int> &arr,int index, vector<int> &visit) {
    //找到比当前数大一的数字的下标,没有返回-1
    int num = arr[index];
    int size = arr.size();
    for(int i = index+1; i< size; i++) {
        if(visit[i] == 0) {
            if(arr[i] == num + 1) {
                return i;
            } else if(arr[i] > num+1) {
                return -1;
            }
        }
    }
    return -1;
}


int Win::findCount(vector<int> &arr, int index, vector<int> &visit) {
    //找到和当前数字一样的数量，判断是顺子还是刻子，顺子返回0，刻子返回1，没有返回-1；
    int count = 0;
    int num = arr[index];
    int size = arr.size();
    for(int i = 0; i< size; i++) {
        if(visit[i] == 0) {
            if(arr[i] == num){
                count++;
            }
        }
    }
    return count;
}
int Win::findJiang(vector<int> &allMahjong, vector<int> &visit, int index) {
    //用这个函数去掉一个将,从index开始,返回从哪里开始去掉的索引
    int size = allMahjong.size();
    for(int i = index; i < size -1; i++) {
        if(allMahjong[i] == allMahjong[i+1]) {
            //满足条件
            if(visit[i] == 0 && visit[i+1] == 0) {
                visit[i] = 1;
                visit[i+1] = 1;
                return i;
            }
        }
    }
    return -1;
}

bool Win::judge(vector<int> &allMahjong, vector<int> &visit) {
    //全部麻将和标记位置,这里的已经去掉将了,进行判断
    int count = 0;
    int size = allMahjong.size();
    for(auto i : visit) {
        count += i;
    }
    if(count == size) {
        return true;
    }

    for(int i = 0; i < size; i++) {
        if(visit[i] == 1){
            continue;
        }
        if(findCount(allMahjong, i, visit) >= 3) {
            //凑刻子,因为是排序好的，不用考虑四个拆开的情况
            visit[i] = 1;
            visit[i+1] = 1;
            visit[i+2] = 1;
            return judge(allMahjong, visit);

        } else {
            //凑顺子
            int next = findNext(allMahjong, i, visit);
            if(next == -1) {
                return false;
            }
            int nextnext = findNext(allMahjong, next, visit);
            if(nextnext == -1) {
                return false;
            }

            //找到对应的顺子
            visit[i] = 1;
            visit[next] = 1;
            visit[nextnext] = 1;
            return judge(allMahjong, visit);
        }
    }


    return false;
}

bool Win::judgeWin(vector<int> &myMahjong) {

    //去掉将
    int jiangIndex = 0;
    int old = -1;
    int size = myMahjong.size();
    if(size <= 1){
        return false;
    }

    if(size == 2) {
        return myMahjong[0] == myMahjong[1];
    }
    vector<int> visit(size, 0);
    int ans = false;

    for(int i = 0; i < size; i++) {
        //去掉将
        visit = vector<int> (size, 0);
        
        jiangIndex = findJiang(myMahjong, visit, i);
        if(myMahjong[jiangIndex] == myMahjong[old]) {
            continue;
        } else {
            old = jiangIndex;
        }

        if(jiangIndex == -1) {
            return false;
        }

        ans = judge(myMahjong, visit);
        if(ans ==false) {
            continue;
        } else {
            return ans;
        }
    }
    return ans;
}