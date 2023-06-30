#include"CCT_Decode.h"

int B2I(vector<int> array, int N)
{
    int min_value = 1000000;
    int temp = 0;
    for (int i = 0; i < N; i++)
    {
        temp = 0;
        for (int j = 0; j < N; j++)
        {
            if (array[j] == 1)
            {
                temp = temp + pow(2, j);
            }
        }
        if (temp < min_value)
        {
            min_value = temp;
        }
        array = MoveBit(array, 1, N);
    }

    for (int i = 0; i < N; i++)
    {
        temp = 0;
        for (int j = 0; j < N; j++)
        {
            if (array[j] == 1)
            {
                temp = temp + pow(2, j);
            }
        }
        if (temp == min_value)
        {
            break;
        }
        array = MoveBit(array, 1, N);
    }
    return min_value;
}