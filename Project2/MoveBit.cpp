#include"CCT_Decode.h"
//�ƶ�Ѱ����С�Ķ�������
vector<int> MoveBit(vector<int> lst, int k, int  N)
{
    vector<int> temp = lst;
    for (int i = 0; i < k; i++)
    {
        int c = temp[0];
        for (int j = 0; j < N - 1; j++)
        {
            temp[j] = temp[j + 1];
        }
        temp[N - 1] = c;
    }
    return temp;
}