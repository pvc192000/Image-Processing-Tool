#include "../iptools/core.h"

#include <string.h>

#include <strings.h>

using namespace std;
using namespace cv;

#define MAXLEN 256

int main(int argc, char ** argv) {
  image src, tgt;
  FILE * fp;
  char str[MAXLEN];
  char infile[MAXLEN];
  char outfile[MAXLEN];
  char * pch;

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Can't open file: %s\n", argv[1]);
    exit(1);
  }

  while (fgets(str, MAXLEN, fp) != NULL) {
    if (strncmp(str, "#", 1) == 0) continue;
    int NumParameters = 0;

    pch = strtok(str, " ");
    strcpy(infile, pch);

    pch = strtok(NULL, " ");
    strcpy(outfile, pch);

    pch = strtok(NULL, " ");
    if (strncmp(pch, "opencv", 6) == 0) {
      cv::Mat I = cv::imread(infile);
      cv::Mat I2;

      if (I.empty()) {
        cout << "Could not open or find the image: " << infile << endl;
        return -1;
      }

      pch = strtok(NULL, " ");
      if (strncasecmp(pch, "gray", MAXLEN) == 0) {

        utility::cv_gray(I, I2);

      } else if (strncasecmp(pch, "blur_avg", MAXLEN) == 0) {
        pch = strtok(NULL, " ");
        utility::cv_avgblur(I, I2, atoi(pch));
      } else if (strncasecmp(pch, "sobel", MAXLEN) == 0) {
      /*Sobel Edge Detection*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        Mat amp, binAmp;

        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        char * buffer2;
        char file2[MAXLEN];
        buffer2 = strtok(NULL, " ");
        strcpy(file2, buffer2);

        cvtColor(I, I2, COLOR_BGR2GRAY);
        I2.copyTo(amp);
        I2.copyTo(binAmp);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_sobel(I, I2, amp, binAmp, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
        imwrite(file1, amp);
        imwrite(file2, binAmp);
      } else if (strncasecmp(pch, "sobel_color", MAXLEN) == 0) {
      /*Sobel Edge Detection on color images*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        Mat amp, binAmp;

        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        char * buffer2;
        char file2[MAXLEN];
        buffer2 = strtok(NULL, " ");
        strcpy(file2, buffer2);

        I.copyTo(I2);
        I.copyTo(amp);
        I.copyTo(binAmp);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_hsi_sobel(I, I2, amp, binAmp, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
        imwrite(file1, amp);
        imwrite(file2, binAmp);
      } else if (strncasecmp(pch, "canny", MAXLEN) == 0) {
      /*Canny Edge Detection*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;

        cvtColor(I, I2, COLOR_BGR2GRAY);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_canny(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }

      } else if (strncasecmp(pch, "canny_color", MAXLEN) == 0) {
      /*Canny Edge Detection on color images*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;

        I.copyTo(I2);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_hsi_canny(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }

      }else if (strncasecmp(pch, "hist_stretching", MAXLEN) == 0) {
      /*Histogram stretching using opencv*/
        char * x;
        char * y;
        char * sx;
        char * sy;

        cvtColor(I, I2, COLOR_BGR2GRAY);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            utility::cv_hist_stretching(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy));
          }
        }

      } else if (strncasecmp(pch, "hist_equalization", MAXLEN) == 0) {
      /*Histogram equalization using opencv*/
        char * x;
        char * y;
        char * sx;
        char * sy;

        cvtColor(I, I2, COLOR_BGR2GRAY);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            utility::cv_hist_equalization(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy));
          }
        }

      } else if (strncasecmp(pch, "V_hist_equalization", MAXLEN) == 0) {
      /*Histogram equalization on V channel using opencv*/
        char * x;
        char * y;
        char * sx;
        char * sy;

        I.copyTo(I2);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            utility::cv_V_hist_equalization(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy));
          }
        }

      }else if (strncasecmp(pch, "HSV_hist_equalization", MAXLEN) == 0) {
      /*Histogram equalization on HSV channels using opencv*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;

        I.copyTo(I2);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_HSV_hist_equalization(I, I2, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }

      }else if (strncasecmp(pch, "hist_eq_sobel", MAXLEN) == 0) {
      /*Histogram equalization followed by sobel edge detection*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        Mat hist_edge;

        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        cvtColor(I, I2, COLOR_BGR2GRAY);
        cvtColor(I, hist_edge, COLOR_BGR2GRAY);

        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_hist_eq_sobel(I, I2, hist_edge, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
        imwrite(file1, hist_edge);
      }else if (strncasecmp(pch, "hist_eq_canny", MAXLEN) == 0) {
      /*Histogram equalization followed by canny edge detection*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        Mat hist_edge;

        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        cvtColor(I, I2, COLOR_BGR2GRAY);
        cvtColor(I, hist_edge, COLOR_BGR2GRAY);


        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::cv_hist_eq_canny(I, I2, hist_edge, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
      imwrite(file1, hist_edge);
      }else if (strncasecmp(pch, "QR_code", MAXLEN) == 0) {
      /*Decodes QR code*/

        cvtColor(I, I2, COLOR_BGR2GRAY);
        pch = strtok(NULL, " ");
        utility::cv_QR_code(I, I2, atoi(pch));

      }else {
        printf("No function: %s\n", pch);
        continue;
      }

      cv::imwrite(outfile, I2);
    } else {
      src.read(infile);
      if (strncasecmp(pch, "add", MAXLEN) == 0) {
        /* Add Intensity */
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            utility::addGrey(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
      } else if (strncasecmp(pch, "binarize", MAXLEN) == 0) {
        /* Thresholding */
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            utility::binarize(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
      } else if (strncasecmp(pch, "scale", MAXLEN) == 0) {
        /* Image scaling */
        pch = strtok(NULL, " ");
        utility::scale(src, tgt, atof(pch));
      } else if (strncasecmp(pch, "thresholded_add", MAXLEN) == 0) {
        /* Image thresholding*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::t_add(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
      } else if (strncasecmp(pch, "double_thresholding", MAXLEN) == 0) {
        /*Image double thresholding*/
        char * pch1;
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            utility::d_thresholding(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1));
          }
        }
      } else if (strncasecmp(pch, "color_modification", MAXLEN) == 0) {
        /*Color Modification*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::color_mod(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
      } else if (strncasecmp(pch, "2D_smoothing", MAXLEN) == 0) {
        /*Unifrom 2D Smoothing*/
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            utility::u2d_smoothing(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
      } else if (strncasecmp(pch, "Inc_1D_smoothing", MAXLEN) == 0) {
        /*Uniform Incremental 1D smoothing*/
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            utility::inc_1d_smoothing(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
      } else if (strncasecmp(pch, "histogram_stretching", MAXLEN) == 0) {
        /*Histogram Stretching*/
        char * pch1;
        char * x;
        char * y;
        char * sx;
        char * sy;
        image preHist;
        image postHist;
        char * buffer;
        char file1[MAXLEN];
        char file2[MAXLEN];
        buffer = strtok(NULL, " ");
        strcpy(file1, buffer);
        buffer = strtok(NULL, " ");
        strcpy(file2, buffer);
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            utility::hist_stretching(src, tgt, preHist, postHist, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1));
          }
        }

        preHist.save(file1);
        postHist.save(file2);

      } else if (strncasecmp(pch, "thresholded_histogram_stretching", MAXLEN) == 0) {
        /*Histogram Stretching with basic thresholding*/
        char * x;
        char * y;
        char * sx;
        char * sy;

        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            utility::thresholded_hist_stretching(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch));
          }
        }
      } else if (strncasecmp(pch, "color_histogram_stretching", MAXLEN) == 0) {
        /*Histogram Stretching with color images*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::color_hist_stretching(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
      } else if (strncasecmp(pch, "hsi_histogram_stretching", MAXLEN) == 0) {
        /*Histogram Stretching with RGB to HSI transformation*/
        char * pch1;
        char * x;
        char * y;
        char * sx;
        char * sy;
        image gray;
        gray.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        gray.copyImage(src);

        char * buffer;
        char file1[MAXLEN];
        buffer = strtok(NULL, " ");
        strcpy(file1, buffer);
        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            utility::hsi_hist_stretching(src, tgt, gray, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1));
          }
        }
        gray.save(file1);
      } else if (strncasecmp(pch, "A_HSI_histogram_stretching", MAXLEN) == 0) {
        /*Histogram Stretching with RGB to HSI transformation*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;

        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::A_HSI_hist_stretching(src, tgt, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }

      } else if (strncasecmp(pch, "sobel_edge_detection", MAXLEN) == 0) {
        /*Edge detection using Sobel operator on grayscale images*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        image amp, binAmp;
        amp.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        binAmp.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        amp.copyImage(src);
        binAmp.copyImage(src);
        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        char * buffer2;
        char file2[MAXLEN];
        buffer2 = strtok(NULL, " ");
        strcpy(file2, buffer2);

        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::sobel_edge_detection(src, tgt, amp, binAmp, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
        amp.save(file1);
        binAmp.save(file2);
      } else if (strncasecmp(pch, "hsi_sobel_edge_detection", MAXLEN) == 0) {
        /*Edge detection using Sobel operator on color images*/
        char * pch1;
        char * pch2;
        char * x;
        char * y;
        char * sx;
        char * sy;
        image amp, binAmp;
        amp.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        binAmp.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        amp.copyImage(src);
        binAmp.copyImage(src);
        char * buffer1;
        char file1[MAXLEN];
        buffer1 = strtok(NULL, " ");
        strcpy(file1, buffer1);
        char * buffer2;
        char file2[MAXLEN];
        buffer2 = strtok(NULL, " ");
        strcpy(file2, buffer2);

        tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
        tgt.copyImage(src);
        for (int i = 0; i < 3; i++) {
          x = strtok(NULL, " ");
          if (x != NULL) {
            y = strtok(NULL, " ");
            sx = strtok(NULL, " ");
            sy = strtok(NULL, " ");
            pch = strtok(NULL, " ");
            pch1 = strtok(NULL, " ");
            pch2 = strtok(NULL, " ");
            utility::hsi_sobel_edge_detection(src, tgt, amp, binAmp, atoi(x), atoi(y), atoi(sx), atoi(sy), atoi(pch), atoi(pch1), atoi(pch2));
          }
        }
        amp.save(file1);
        binAmp.save(file2);
      } else {
        printf("No function: %s\n", pch);
        continue;
      }

      tgt.save(outfile);
    }
  }
  fclose(fp);
  return 0;
}