/**
 * @file Win.h
 * @author hufeiyu
 * @brief 胡牌算法
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef WIN_H
#define WIN_H
#include <vector>
#include "Mahjong.h"
#include <string>
using namespace std;


struct WinMessage{
    vector<int> winPlayerMahjong;
    vector<PongKongChow*> winPKC;
    int winnerNum = -1;
    string winnerName;
};

/*


大致思想：先找到一对将，其余的进行判断 能不能全部分配成刻子或者顺子，能则胡，不能则不胡
*/
class Win {
public:
    static bool judgeWin(vector<int> &myMahjong);

private:

    static int findNext(vector<int> &arr,int index, vector<int> &visit);
    static int findCount(vector<int> &arr, int index, vector<int> &visit);
    static int findJiang(vector<int> &allMahjong, vector<int> &visit, int index);
    static bool judge(vector<int> &allMahjong, vector<int> &visit);
};

#endif