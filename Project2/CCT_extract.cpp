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
    vector<vector<Point>> contours;//������ÿ��Ԫ�ر�����һ����������Point�㹹�ɵĵ�ļ��ϵ�����
    vector<Vec4i> hierarchy;
    findContours(img_bina, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE, Point());
    //��������
    //for (int i = 0; i < contours.size(); ++i) {
    //    drawContours(img, contours, i, Scalar(0, 0, 255), 2, 8);
    //}
    //hierarchy������ÿһ��Ԫ�ص�4��int�ͱ�������hierarchy[i][0] ~hierarchy[i][3]
    //�ֱ��ʾ��i�������ĺ�һ��������ǰһ������������������Ƕ�������������

    //������ȡ������������ɸѡ���е���Բ����

    if (!contours.empty() && !hierarchy.empty())
    {
        for (int i = 0; i < contours.size(); i++)
        {
            cout << i << endl;
            double area = contourArea((Mat)contours[i]);
            double length = arcLength((Mat)contours[i], true);
            //����������Բ��
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
            box3.points(vs);//���������ĸ�����
            double a = max(box3.size.height, box3.size.width);//������Բ�ĳ���
            double s = a;
            //�ü�CCTͼ��
            Mat cct_roi;
            int row_min = round(box1.center.y - s / 2);
            int row_max = round(box1.center.y + s / 2);
            int col_min = round(box1.center.x - s / 2);
            int col_max = round(box1.center.x + s / 2);
            //�ж�cct_roi�Ƿ񳬳�ԭͼ��߽�
            if (row_min >= 0 && row_max <= img_height && col_min >= 0 && col_max <= img_width)
            {
                //Mat cct_roi = img;
                //��ԭͼ���н�cct_roi��ȡ����
                Mat cct_roi = img(Range(row_min, row_max), Range(col_min, col_max));
                //cct_roi�����ԭʼӰ���ƫ����
                double dx = box1.center.x - s / 2;
                double dy = box1.center.y - s / 2;
                //��CCT��Բ������з���任�����Ϊ��Բ
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
                //�õ�����任����
                Mat M = getAffineTransform(src, dst);
                if (typeid(M) == typeid(int))
                {
                    continue;
                }
                Mat CCT_img;
                //��cct_roi���з���任
                Point2f cct_roi_size(cct_roi.cols, cct_roi.rows);
                if (cct_roi_size.x > 0 && cct_roi_size.y > 0)
                {
                    warpAffine(cct_roi, CCT_img, M, Size(round(s), round(s)));
                }
                //�Է���任���CCT��������
                //Mat CCT_large;
                //resize(CCT_img, CCT_large, Size(0, 0), 50.0 / s, 50.0 / s);

                Mat CCT_large = LinerInter(CCT_img, 4, 4);
                //resize(CCT_large, CCT_large, Size(0, 0), 70.0 / s, 70.0 / s);
                //���Ŵ���CCTת��Ϊ�Ҷ�ͼ
                Mat CCT_gray;
                cvtColor(CCT_large, CCT_gray, COLOR_BGR2GRAY);
                //�ԸûҶ�ͼ��������Ӧ��ֵ��
                Mat CCT_bina;
                threshold(CCT_gray, CCT_bina, 0, 1, THRESH_BINARY | THRESH_OTSU);
                Mat CCT_bina_show = CCT_bina * 255;
                //��ʴ
                Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
                Mat CCT_eroded;
                erode(CCT_bina, CCT_eroded, kernel);
                //�ж�����������ǲ���CCT
                if (CCT_or_not(CCT_eroded) == 1)
                {
                    //���ý��뺯�����н���
                    int code = CCT_Decode(CCT_eroded, N, color);
                    CodeTable.push_back(code);
                    CodeTable.push_back(box1.center.x);
                    CodeTable.push_back(box1.center.y);
                    //��������ԭͼ���л��Ƴ���.�����������ǣ�ͼƬ����ӵ����֣����Ͻ����꣬���壬�����С����ɫ�������ϸ
                    Point org(box3.center.x - 0.25 * s, box1.center.y + 0.5 * s);
                    putText(img, to_string(code), org, cv::FONT_HERSHEY_COMPLEX, 0.2, (0, 0, 255), 1);
                    cout << "������Ϊ��" << to_string(code) << endl;
                    //������ϳ�����Բ
                    ellipse(img, box1, (0, 255, 0), 1);
                    ellipse(img, box2, (0, 255, 0), 1);
                    ellipse(img, box3, (0, 255, 0), 1);
                }
            }
        }
    }
    return img;
}
