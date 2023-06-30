#include"CCT_Decode.h"
//二维矩阵的转置函数
vector<vector<int>> transpose(vector<vector<int>>& A) 
{
    int leny = A[0].size();
    int lenx = A.size();
    vector<vector<int>>v(leny, vector<int>());
    if (A.empty())return vector<vector<int>>();
    for (int i = 0; i < lenx; i++)
        for (int j = 0; j < leny; j++) {
            v[j].push_back(A[i][j]);

        }
    return v;
}
