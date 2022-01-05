**Author: Param Vipulkumar Chokshi, U#: U70722122**


*** INSTALATION ***

On Linux

Enter the iptools directory in terminal and run make

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.
Function requires one parameters: Value(v).

2. Binarization: binarize
Binarize the pixels with the threshold.
Function requires one parameters: threshold(T).

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.
Function requires one parameters: Scale_factor(s).

*Assignment 0*
4. Add intensity (thresholded): thresholded_add
Add or decrease intesity of a pixel with the threshold. 
Function requires three parameters: threshold(T), value1(v1), and value2(v2).
Increases the intensity with V1 if the intensity of a pixel is larger than T.
Decreases the intensity with V2 if the intensity of a pixel is smaller than T.

*Assignment 1*
5. Double thresholding: double_thresholding
Change pixels to red if the initial intensity falls between the given thresholds.
Function requires two parameters: threshold1(T1), threshold2(T2).

6. Color Modification: color_modification
Increase the intensities on Red, Green, and Blue three channels with inputs 
dR, dG, and dB, on color image.
Function requires three parameters: Rvalue(dR), Gvalue(dG), and Bvalue(Db).

7. 2D Smoothing: 2D_smoothing
2D uniform smoothing with window size (WS) on gray-level image.
Function requires one parameter: Window size(WS).

8. Incremental 1D Smoothing: Inc_1D_smoothing
1D uniform smoothing with window size (WS) using incremental implementation on gray-level image.
Function requires one parameter: Window size(WS).

*Assignment 2*
9. Histogram Stretching: histogram_stretching
Histogram stretching in the input range [a,b] to [0,255]. The function also does contrast clipping,
i.e maps [0,a] to 0, and [b,255] to 255. The function also generates histograms of the image before and after
stretching.
Function requires two parameters: Value1(a) and Value2(b).
Filenames required before the ROI parameters: pre-histogram filename(preHist), post-histogram filename(postHist),
Here the filnames for histograms should be with the extension .pgm


10. Thresholded Histogram Stretching: thresholded_histogram_stretching
Combination of image thresholding and histogram stretching. Histogram stretching with contrast clipping is
applied to the foreground and background seperately, and the final image is produced by combining those two
images.
Function requires one parameter: threshold(T).

11. Color Image Histogram Stretching: color_histogram_stretching
Histogrm stretching with contrats clipping on color images. The function allows to perform stretching
on each individual channels and aslo on all channels together. Stretching is applied seperately to each 
channels and the final image is produced by combining them.
Function requires three parameters: value1(a), value2(b), RGB code(code).
Here the RGB code is and int value from [1-4] inclusive. Based on the code the function performs stretching on
1 - only red channel;  2 - only green channel
3 - only blue channel; 4 - all the 3 channels

12. Histogram Stretching on I: hsi_histogram_stretching
The function converts the RGB values of the Image to HSI values, and then performs histogram stretching
with contrast clipping to the I component. The function converts the stretched values back to RGB to display the
resulting image. The function also generates a gray-level image of the stretched I component.
Function requires two parameters:  value1(a) and value2(b).
Filenames required before the ROI parameters: graylevel I image filename(gray)
Here the filname for graylevel I image should be with the extension .pgm

*Extra Credit*
13. Histogram stretching on SI and HSI: A_HSI_hist_stretching
The function converts the RGB values of the Image to HSI values, and then performs histogram stretching
with contrast clipping to the S and I components or all HSI components. The function converts the stretched values back to RGB to display the
resulting image.
Function requires three parameters: value1(a), value2(b),  code(code).
Here the code is and int value from [1-2] inclusive. Based on the code the function performs stretching on
1 - only S and I components;  2 - all HSI components

*Assignment 3*
14. Sobel edge Detection: sobel_edge_detection
The function calculates gradient of the image using Sobel's operator. The gradient image is binarized to 
represent edges. Edges angled at a user specified angle is stored in the resulting image
Function requires three parameters:  window size(ws), threshold(t1), angle(t2)(range: 0-180).
Filenames required before the ROI parameters: gradient image name(amp), binarized gradient image name(bin)
Here the filname for gradient image and binarized should be with the extension .pgm

15. Sobel edge Detection on color images: hsi_sobel_edge_detection
Performs Sobel edge detection on the I channel.
Function requires three parameters:  window size(ws), threshold(t1), angle(t2)(range: 0-180).
Filenames required before the ROI parameters: gradient image name(amp), binarized gradient image name(bin)
Here the filname for gradient image and binarized should be with the extension .ppm

*OpenCV filters*
16. Sobel edge Detection: sobel
The function does Sobel edge detection using openCV's Sobel function.
Function requires three parameters:  window size(ws), threshold(t1), angle(t2)(range: 0-90).
Filenames required before the ROI parameters: gradient image name(amp), binarized gradient image name(bin)
Here the filname for gradient image and binarized should be with the extension .jpg

17. Sobel edge Detection on color images: sobel_color
The function does Sobel edge detection on color images (I/V channel) using openCV's Sobel function.
Function requires three parameters:  window size(ws), threshold(t1), angle(t2)(range: 0-90).
Filenames required before the ROI parameters: gradient image name(amp), binarized gradient image name(bin)
Here the filname for gradient image and binarized should be with the extension .jpg

18. Canny Edge detection: canny
The function does Canny edge detection using openCV's Canny function.
Function requires three parameters:  window size(ws), threshold1(t1), threshold2(t2).

19. Canny Edge detection on color images: canny_color
The function does Canny edge detection on color images (I/V channel) using openCV's Canny function.
Function requires three parameters:  window size(ws), threshold1(t1), threshold2(t2).

*Assignment 4*
20. Histogram Stretching on Gray Scale images: hist_stretching
The function performs histogram stretching from image max-min to 0-255.
Function requires no parameters apart from ROIs

21. Histogram Equalization on Gray Scale images: hist_equalization
The function performs Histogram Equalization on gray scale images.
Function requires no parameters apart from ROIs

22. Histogram Equalization on V channel : V_hist_equalization
The function performs Histogram Equalization on the V channel of color images.
Function requires no parameters apart from ROIs

23. Histogram Equalization on multiple HSV channels: HSV_hist_equalization
The function performs Histogram Equalization on any of the HSV channels of the color images.
Function requires three parameters:  perform on Hue(H), perform on Saturation(S), perform of on Value(V).
All the parameters accept 0 or 1. 0 for false, and 1 for true. 

24. HE followed by Sobel Edge Detection: hist_eq_sobel
The function performs Histogram Equalization followed by Sobel Edge detection.
Function requires one parameters:  window size(ws)
Filenames required before the ROI parameters: histogram equalized sobel amplitude image (amp)
Here the filname for gradient image and binarized should be with the extension .jpg

25. HE followed by Canny Edge Detection: hist_eq_canny
The function performs Histogram Equalization followed by Canny Edge detection.
Function requires three parameters:  window size(ws), threshold1(t1), threshold2(t2)
Filenames required before the ROI parameters: histogram equalized canny amplitude image (amp)
Here the filname for gradient image and binarized should be with the extension .jpg


*Extra Credit*
26. QR code decoding: QR_code
The function decodes QR code from inputted images.
Function requires one parameters: perform HE (HE); value should be 0 or 1, 0 for false, 1 for true

*** PARAMETERS FILE ***

There are upto 6 parameters:
1. the input file name;
2. the output file name;
3. (only if using functions using OpenCV) Add "opencv"
4. the name of the filter. Use "add", "binarize", "scale", "thresholded_add", "double_thresholding", "color_modification", 
   "2D_smoothing", "Inc_1D_smoothing", "histogram_stretching", "thresholded_histogram_stretching", "color_histogram_stretching",
   "hsi_histogram_stretching", "A_HSI_hist_stretching", "sobel_edge_detection", "hsi_sobel_edge_detection", "sobel", "sobel_color",
   "canny", "canny_color", "hist_stretching", "hist_equalization", "V_hist_equalization", "HSV_hist_equalization", "hist_eq_sobel", "hist_eq_canny", and "QR_code" for your filters;
5. (only for "histogram_stretching" ,"color_histogram_stretching", "sobel_edge_detection", "hsi_sobel_edge_detection", "sobel", "sobel_color", "hist_eq_sobel", "hist_eq_canny") 
   required filenames (refer "Filenames" section above for the required filenames)
6. upto 3 region of interest (ROI's) of the format: x y Sx Sy; where
   (x,y): the left-top pixel of the ROI
   (Sx,Sy): the size of ROI
   (skip this step for scale filter)
7. the required parameters for each function, (refer "Function" section above for the required parameters)

Example:
with 3 ROIs
baboon.pgm baboon_2Dsmooth.pgm 0 100 100 500 5 100 0 100 100 5 200 200 100 100 5

with 2 ROIs
shore.ppm shore_cm.ppm color_modification 0 100 100 500 50 -50 50 100 0 100 100 50 50 50 

with 1 ROI
lena.pgm lena_dt.ppm double_thresholding 0 100 100 500 100 200 
(Here the input is a pgm file and the output is a ppm file , this is done to view the red color)

with 1 ROI (with filename example)
slope.pgm slope_hist.pgm histogram_stretching slope_preHist.pgm slope_postHist.pgm 0 0 534 800 100 200

with 1 ROI (with filename and opencv example)
wheel.jpg wheel_t.jpg opencv sobel wheel_Samp.jpg wheel_Sbin.jpg 0 0 300 300 3 20 1

*** Run the program: ./iptool parameters.txt
