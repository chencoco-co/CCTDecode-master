#include"CCT_Decode.h"
//十进制转二进制
vector<int> I2B(int value, int N)
{
    vector<int> B;
    for (int j = 0; j < N; j++)
    {
        if (value > 0)
        {
            B.push_back(value % 2);
            value = value / 2;
        }
        else
        {
            B.push_back(0);
        }
    }
    return B;
}