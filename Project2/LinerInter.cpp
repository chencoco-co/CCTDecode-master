#include"CCT_Decode.h"
//双线性插值函数
Mat LinerInter(Mat& srcImage, double kx, double ky)
{
    int rows = cvRound(srcImage.rows * kx);
    int cols = cvRound(srcImage.cols * ky);
    Mat resultImg(rows, cols, srcImage.type());
    int i, j;
    int xi;
    int yi;
    int x11;
    int y11;
    double xm;
    double ym;
    double dx;
    double dy;

    for (i = 0; i < rows; i++)
    {
        xm = i / kx;
        xi = (int)xm;
        x11 = xi + 1;
        dx = xm - xi;
        for (j = 0; j < cols; j++)
        {
            ym = j / ky;
            yi = (int)ym;
            y11 = yi + 1;
            dy = ym - yi;
            //判断边界
            if (x11 > (srcImage.rows - 1))
            {
                x11 = xi - 1;
            }
            if (y11 > (srcImage.cols - 1))
            {
                y11 = yi - 1;
            }
            //bgr
            resultImg.at<Vec3b>(i, j)[0] = (int)(srcImage.at<Vec3b>(xi, yi)[0] * (1 - dx) * (1 - dy) + srcImage.at<Vec3b>(x11, yi)[0] * dx * (1 - dy) + srcImage.at<Vec3b>(xi, y11)[0] * (1 - dx) * dy + srcImage.at<Vec3b>(x11, y11)[0] * dx * dy);
            resultImg.at<Vec3b>(i, j)[1] = (int)(srcImage.at<Vec3b>(xi, yi)[1] * (1 - dx) * (1 - dy) + srcImage.at<Vec3b>(x11, yi)[1] * dx * (1 - dy) + srcImage.at<Vec3b>(xi, y11)[1] * (1 - dx) * dy + srcImage.at<Vec3b>(x11, y11)[1] * dx * dy);
            resultImg.at<Vec3b>(i, j)[2] = (int)(srcImage.at<Vec3b>(xi, yi)[2] * (1 - dx) * (1 - dy) + srcImage.at<Vec3b>(x11, yi)[2] * dx * (1 - dy) + srcImage.at<Vec3b>(xi, y11)[2] * (1 - dx) * dy + srcImage.at<Vec3b>(x11, y11)[2] * dx * dy);
        }
    }
    return resultImg;
}