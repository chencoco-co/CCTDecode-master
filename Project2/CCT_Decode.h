#include<iostream>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<shlwapi.h>
#include<opencv2/opencv.hpp>
#include <math.h>
#include <numeric>

#define PI acos(-1)

using namespace std;
using namespace cv;

Mat LinerInter(Mat& srcImage, double kx, double ky);
vector<vector<int>> transpose(vector<vector<int>>& A);
vector<int> I2B(int value, int N);
bool CCT_or_not(cv::Mat img);
vector<int> MoveBit(vector<int> lst, int k, int  N);
int B2I(vector<int> array, int N);
vector<int> swap0and1(vector<int> code_list);
int CCT_Decode(Mat cct_img, int N, bool color);
Mat CCT_extract(Mat img, int N, double R, bool color);