#include"CCT_Decode.h"
//�ڰ�ͼ�Ľ���
vector<int> swap0and1(vector<int> code_list)
{
    vector<int> result;
    for (int i = 0; i < code_list.size(); i++)
    {
        if (code_list[i] == 0)
        {
            result.push_back(1);
        }
        else if (code_list[i] == 1)
        {
            result.push_back(0);
        }
    }
    return result;
}
