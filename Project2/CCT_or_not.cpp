#include"CCT_Decode.h"
//判断是否为CCT图片
bool CCT_or_not(cv::Mat cct_bimg)
{
    int sample_num = 36;
    int img_width = cct_bimg.cols;
    int img_height = cct_bimg.rows;
    int x0 = img_width / 2;
    int y0 = img_height / 2;
    //CCT中心圆的半径
    double r1 = x0 / 3.0;
    //存放三个圆周上采样点像素值的列表
    vector<int> pixels_0_5_r;
    vector<int> pixels_1_5_r;
    vector<int> pixels_2_5_r;
    //遍历三个圆周处的采样点像素
    for (int j = 0; j < sample_num; j++)
    {
        double xi = cos(10.0 * j / 180 * PI);
        double yi = sin(10.0 * j / 180 * PI);
        double x_0_5_r = 0.5 * r1 * xi + x0;
        double y_0_5_r = 0.5 * r1 * yi + y0;
        double x_1_5_r = 1.5 * r1 * xi + x0;
        double y_1_5_r = 1.5 * r1 * yi + y0;
        double x_2_5_r = 2.5 * r1 * xi + x0;
        double y_2_5_r = 2.5 * r1 * yi + y0;
        int pixel_value_0_5_r = cct_bimg.at<uchar>(round(y_0_5_r), round(x_0_5_r));
        int pixel_value_1_5_r = cct_bimg.at<uchar>(round(y_1_5_r), round(x_1_5_r));
        int pixel_value_2_5_r = cct_bimg.at<uchar>(round(y_2_5_r), round(x_2_5_r));
        pixels_0_5_r.push_back(pixel_value_0_5_r);
        pixels_1_5_r.push_back(pixel_value_1_5_r);
        pixels_2_5_r.push_back(pixel_value_2_5_r);
    }
    if (accumulate(pixels_0_5_r.begin(), pixels_0_5_r.end(), 0) == sample_num && accumulate(pixels_1_5_r.begin(), pixels_1_5_r.end(), 0) == 0 && accumulate(pixels_2_5_r.begin(), pixels_2_5_r.end(), 0) >= 2)
    {
        return 1;
    }
    else if (accumulate(pixels_0_5_r.begin(), pixels_0_5_r.end(), 0) == 0 && accumulate(pixels_1_5_r.begin(), pixels_1_5_r.end(), 0) == sample_num && accumulate(pixels_2_5_r.begin(), pixels_2_5_r.end(), 0) <= sample_num - 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}