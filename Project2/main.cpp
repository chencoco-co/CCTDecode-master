#include"CCT_Decode.h"

int main()
{
    Mat img = imread("C:\\Users\\59309\\Desktop\\CCTDecode-master\\test\\data_base\\test_1\\image_4.png", IMREAD_ANYCOLOR);
    if (img.empty()) {
        cout << "ͼƬ������" << endl;
        return -1;
    }
    int N = 8;
    bool color = 0;//0Ϊ�ڵװ�ͼ��1Ϊ�׵׺�ͼ
    double R = 0.6;
    Mat result_img = CCT_extract(img, N, R, color);
    cout << "�������";
    string filename = "C:\\Users\\59309\\Desktop\\CCTDecode-master\\test\\result\\image_4.png";
    imwrite(filename, result_img);
    return 0;
}