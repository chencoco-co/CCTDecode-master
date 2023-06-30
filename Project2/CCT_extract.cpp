#include"CCT_Decode.h"

Mat CCT_extract(Mat img, int N, double R, bool color)
{
    vector<int> CodeTable;
    int img_width = img.cols;
    int img_height = img.rows;
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_RGBA2GRAY);
    Mat img_bina;
    threshold(img_gray, img_bina, 0, 1, THRESH_BINARY | THRESH_OTSU);
    Mat img_bina_imshow = img_bina * 255;
    vector<vector<Point>> contours;//向量内每个元素保存了一组由连续的Point点构成的点的集合的向量
    vector<Vec4i> hierarchy;
    findContours(img_bina, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE, Point());
    //绘制轮廓
    //for (int i = 0; i < contours.size(); ++i) {
    //    drawContours(img, contours, i, Scalar(0, 0, 255), 2, 8);
    //}
    //hierarchy向量内每一个元素的4个int型变量——hierarchy[i][0] ~hierarchy[i][3]
    //分别表示第i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号

    //遍历提取出来的轮廓，筛选其中的椭圆轮廓

    if (!contours.empty() && !hierarchy.empty())
    {
        for (int i = 0; i < contours.size(); i++)
        {
            cout << i << endl;
            double area = contourArea((Mat)contours[i]);
            double length = arcLength((Mat)contours[i], true);
            //计算轮廓的圆度
            double R0 = 2 * sqrt(PI * area) / (length + 1);
            if (R0 < R)
            {
                continue;
            }
            if (contours[i].size() < 5)
            {
                continue;
            }
            RotatedRect box1;
            box1 = fitEllipse(Mat(contours[i]));
            RotatedRect box2 = box1;
            box2.size.height = box2.size.height * 2;
            box2.size.width = box2.size.width * 2;
            RotatedRect box3 = box1;
            box3.size.height = box3.size.height * 3;
            box3.size.width = box3.size.width * 3;
            Point2f vs[4];
            box3.points(vs);//计算矩阵的四个顶点
            double a = max(box3.size.height, box3.size.width);//计算椭圆的长轴
            double s = a;
            //裁剪CCT图像
            Mat cct_roi;
            int row_min = round(box1.center.y - s / 2);
            int row_max = round(box1.center.y + s / 2);
            int col_min = round(box1.center.x - s / 2);
            int col_max = round(box1.center.x + s / 2);
            //判断cct_roi是否超出原图像边界
            if (row_min >= 0 && row_max <= img_height && col_min >= 0 && col_max <= img_width)
            {
                //Mat cct_roi = img;
                //从原图像中将cct_roi截取出来
                Mat cct_roi = img(Range(row_min, row_max), Range(col_min, col_max));
                //cct_roi相对于原始影像的偏移量
                double dx = box1.center.x - s / 2;
                double dy = box1.center.y - s / 2;
                //对CCT椭圆区域进行仿射变换将其变为正圆
                Point2f src[5];
                src[0].x = vs[0].x - dx; src[0].y = vs[0].y - dy;
                src[1].x = vs[1].x - dx; src[1].y = vs[1].y - dy;
                src[2].x = vs[2].x - dx; src[2].y = vs[2].y - dy;
                src[3].x = vs[3].x - dx; src[3].y = vs[3].y - dy;
                src[4].x = box1.center.x - dx; src[4].y = box1.center.y - dy;
                Point2f dst[5];
                dst[0].x = box1.center.x - a / 2 - dx; dst[0].y = box1.center.y - a / 2 - dy;
                dst[1].x = box1.center.x + a / 2 - dx; dst[1].y = box1.center.y - a / 2 - dy;
                dst[2].x = box1.center.x + a / 2 - dx; dst[2].y = box1.center.y + a / 2 - dy;
                dst[3].x = box1.center.x - a / 2 - dx; dst[3].y = box1.center.y + a / 2 - dy;
                dst[4].x = box1.center.x - dx; dst[4].y = box1.center.y - dy;
                //得到仿射变换矩阵
                Mat M = getAffineTransform(src, dst);
                if (typeid(M) == typeid(int))
                {
                    continue;
                }
                Mat CCT_img;
                //对cct_roi进行仿射变换
                Point2f cct_roi_size(cct_roi.cols, cct_roi.rows);
                if (cct_roi_size.x > 0 && cct_roi_size.y > 0)
                {
                    warpAffine(cct_roi, CCT_img, M, Size(round(s), round(s)));
                }
                //对仿射变换后的CCT进行缩放
                //Mat CCT_large;
                //resize(CCT_img, CCT_large, Size(0, 0), 50.0 / s, 50.0 / s);

                Mat CCT_large = LinerInter(CCT_img, 4, 4);
                //resize(CCT_large, CCT_large, Size(0, 0), 70.0 / s, 70.0 / s);
                //将放大后的CCT转换为灰度图
                Mat CCT_gray;
                cvtColor(CCT_large, CCT_gray, COLOR_BGR2GRAY);
                //对该灰度图进行自适应二值化
                Mat CCT_bina;
                threshold(CCT_gray, CCT_bina, 0, 1, THRESH_BINARY | THRESH_OTSU);
                Mat CCT_bina_show = CCT_bina * 255;
                //腐蚀
                Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
                Mat CCT_eroded;
                erode(CCT_bina, CCT_eroded, kernel);
                //判断这个区域里是不是CCT
                if (CCT_or_not(CCT_eroded) == 1)
                {
                    //调用解码函数进行解码
                    int code = CCT_Decode(CCT_eroded, N, color);
                    CodeTable.push_back(code);
                    CodeTable.push_back(box1.center.x);
                    CodeTable.push_back(box1.center.y);
                    //将编码在原图像中绘制出来.各参数依次是：图片，添加的文字，左上角坐标，字体，字体大小，颜色，字体粗细
                    Point org(box3.center.x - 0.25 * s, box1.center.y + 0.5 * s);
                    putText(img, to_string(code), org, cv::FONT_HERSHEY_COMPLEX, 0.2, (0, 0, 255), 1);
                    cout << "解码结果为：" << to_string(code) << endl;
                    //绘制拟合出的椭圆
                    ellipse(img, box1, (0, 255, 0), 1);
                    ellipse(img, box2, (0, 255, 0), 1);
                    ellipse(img, box3, (0, 255, 0), 1);
                }
            }
        }
    }
    return img;
}
