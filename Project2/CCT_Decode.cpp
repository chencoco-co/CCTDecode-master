#include"CCT_Decode.h"

int CCT_Decode(Mat cct_img, int N, bool color)
{
    int img_width = cct_img.cols;
    int img_height = cct_img.rows;
    int x0 = img_width / 2;
    int y0 = img_height / 2;
    double r1 = x0 / 3.0;
    vector<vector<int>> code_all;
    for (int j = 0; j<int(360 / N); j++)
    {
        vector<int> code_j;//存放单圈值的list
        for (int k = 0; k < N; k++)
        {
            double x = 2.5 * r1 * cos((360.0 / N * k + j) / 180 * PI) + x0;
            double y = 2.5 * r1 * sin((360.0 / N * k + j) / 180 * PI) + y0;
            int pixel_value = cct_img.at<uchar>(round(y), round(x));
            code_j.push_back(pixel_value);
        }
        int temp1 = B2I(code_j, N);
        vector<int> temp2 = I2B(temp1, N);
        code_all.push_back(temp2);
    }
    vector<vector<int>>code_T = transpose(code_all);
    vector<double> code;
    for (int m = 0; m < code_T.size(); m++)
    {
        double sum = accumulate(begin(code_T[m]), end(code_T[m]), 0.0);
        double mean = sum / code_T[m].size();
        code.push_back(mean);
    }
    vector<int> result;
    for (int i = 0; i < code.size(); i++)
    {
        double test = code[i];
        if (test > 0.5)
        {
            result.push_back(1);
        }
        else if (test <= 0.5)
        {
            result.push_back(0);
        }
    }
    if (color == 1)
    {
        result = swap0and1(result);
    }
    int result_final = B2I(result, result.size());
    return result_final;
}