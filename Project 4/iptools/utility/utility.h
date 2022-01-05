#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>
#include <vector>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
    static void cv_sobel(cv::Mat &src,cv::Mat &tgt, cv::Mat&amp, cv::Mat &binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2);
    static void cv_hsi_sobel(cv::Mat &src, cv::Mat &tgt, cv::Mat &amp, cv::Mat &binAmp,  int x, int y, int sx, int sy, int ws, int t1, int t2);
    static void cv_canny(cv::Mat &src, cv::Mat &tgt, int x, int y, int sx, int sy, int ws, int t1, int t2);
    static void cv_hsi_canny(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy, int ws, int t1, int t2);  
    static void cv_hist_stretching(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy);
    static void cv_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy);
    static void cv_V_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy);   
    static void cv_HSV_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy, int h, int s, int v); 
    static void cv_hist_eq_sobel(cv::Mat &src, cv::Mat &tgt, cv::Mat &he, int x, int y, int sx, int sy, int ws);
    static void cv_hist_eq_canny(cv::Mat &src, cv::Mat &tgt,cv::Mat &he, int x, int y, int sx, int sy, int ws, int t1, int t2);
    static void cv_QR_code(cv::Mat &src, cv::Mat &tgt, int HE);
    static int normalize(int value, int max);
    static int rgb_to_h(double R, double G,double B );
    static int rgb_to_s(double R, double G,double B );
    static int rgb_to_i(double R, double G,double B);
    static std::vector<int> hsi_to_rgb(int H, int S, int I);
    static bool isInROI (int x, int y, int sx, int sy, int row, int col); 
		static void addGrey(image &src, image &tgt, int x, int y, int sx, int sy, int value);
		static void binarize(image &src, image &tgt, int x, int y, int sx, int sy, int threshold);
		static void scale(image &src, image &tgt, float ratio);
    static void t_add(image &src, image &tgt, int x, int y, int sx, int sy, int threshold, int value1, int value2);
    static void d_thresholding(image &src, image &tgt, int x, int y, int sx, int sy, int T1, int T2);
    static void color_mod(image &src, image &tgt, int x, int y, int sx, int sy, int dR, int dG, int dB);
    static void u2d_smoothing(image &src, image &tgt, int x, int y, int sx, int sy, int ws);
    static void inc_1d_smoothing(image &src, image &tgt, int x, int y, int sx, int sy, int ws);
    static void hist_stretching(image &src, image &tgt, image &preHist, image &postHist, int x, int y, int sx, int sy, int a, int b);
    static void thresholded_hist_stretching(image &src, image &tgt, int x, int y, int sx, int sy, int threshold);
    static void color_hist_stretching(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b, int code);
    static void hsi_hist_stretching(image &src, image &tgt, image &gray, int x, int y, int sx, int sy, int a, int b);
    static void A_HSI_hist_stretching(image &src, image &tgt, int x, int y, int sx, int sy, int a, int b, int code);
    static void sobel_edge_detection(image &src, image &tgt, image &amp, image &binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2);
    static void hsi_sobel_edge_detection(image &src, image &tgt, image &amp, image &binAmp,  int x, int y, int sx, int sy, int ws, int t1, int t2);
};

#endif

