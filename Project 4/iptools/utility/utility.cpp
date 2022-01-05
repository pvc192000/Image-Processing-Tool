#include "utility.h"

#include <vector>

#include <algorithm>

#include <math.h>

#define MAXRGB 255
#define MINRGB 0
#define PI 3.141592653589

using namespace cv;
using namespace std;

std::string utility::intToString(int number) {
  std::stringstream ss; //create a stringstream
  ss << number; //add number to the stream
  return ss.str(); //return a string with the contents of the stream
}

int utility::checkValue(int value) {
  if (value > MAXRGB)
    return MAXRGB;
  if (value < MINRGB)
    return MINRGB;
  return value;
}

int utility::normalize(int value, int max) {
  int a = (value) * (255);
  int b = max;
  return a / b;
}

bool utility::isInROI(int x, int y, int sx, int sy, int row, int col) {
  if ((row >= x) && (col >= y) && (row < x + sx) && (col < y + sy))
    return true;
  else
    return false;
}
/*-----------------------------------------------------------------------**/
void utility::addGrey(image & src, image & tgt, int x, int y, int sx, int sy, int value) {
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (tgt.isInbounds(i, j)) {
        tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value));
      }

    }
  }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image & src, image & tgt, int x, int y, int sx, int sy, int threshold) {
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (tgt.isInbounds(i, j)) {
        if (src.getPixel(i, j) < threshold)
          tgt.setPixel(i, j, MINRGB);
        else
          tgt.setPixel(i, j, MAXRGB);
      }
    }
  }
}

/*-----------------------------------------------------------------------**/
void utility::scale(image & src, image & tgt, float ratio) {
  int rows = (int)((float) src.getNumberOfRows() * ratio);
  int cols = (int)((float) src.getNumberOfColumns() * ratio);
  tgt.resize(rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      /* Map the pixel of new image back to original image */
      int i2 = (int) floor((float) i / ratio);
      int j2 = (int) floor((float) j / ratio);
      if (ratio == 2) {
        /* Directly copy the value */
        tgt.setPixel(i, j, checkValue(src.getPixel(i2, j2)));
      }
      if (ratio == 0.5) {
        /* Average the values of four pixels */
        int value = src.getPixel(i2, j2) + src.getPixel(i2, j2 + 1) + src.getPixel(i2 + 1, j2) + src.getPixel(i2 + 1, j2 + 1);
        tgt.setPixel(i, j, checkValue(value / 4));
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::t_add(image & src, image & tgt, int x, int y, int sx, int sy, int threshold, int value1, int value2) {
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (tgt.isInbounds(i, j)) {
        if (tgt.getPixel(i, j) > threshold) {
          /*If the intensity is greater than the threshold increase the intensity by value v1*/
          tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value1));
        } else if (tgt.getPixel(i, j) < threshold) {
          /*If the intensity is less than the threshold decrease the intensity by value v2*/
          tgt.setPixel(i, j, checkValue(src.getPixel(i, j) - value2));
        }
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::d_thresholding(image & src, image & tgt, int x, int y, int sx, int sy, int T1, int T2) {
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {
        if (src.getPixel(i, j) > T1 && src.getPixel(i, j) < T2) {
          /*If the pixel is between the given intensities, make it red*/
          tgt.setPixel(i, j, RED, 255);
          tgt.setPixel(i, j, GREEN, 0);
          tgt.setPixel(i, j, BLUE, 0);
        }
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::color_mod(image & src, image & tgt, int x, int y, int sx, int sy, int dR, int dG, int dB) {
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {
        tgt.setPixel(i, j, RED, checkValue(tgt.getPixel(i, j, RED) + dR));
        tgt.setPixel(i, j, GREEN, checkValue(tgt.getPixel(i, j, GREEN) + dG));
        tgt.setPixel(i, j, BLUE, checkValue(tgt.getPixel(i, j, BLUE) + dB));
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::u2d_smoothing(image & src, image & tgt, int x, int y, int sx, int sy, int ws) {
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      int sum = 0;
      int count = 0;
      for (int k = i - (ws / 2); k <= i + (ws / 2); k++) {
        for (int l = j - (ws / 2); l <= j + (ws / 2); l++) {
          if (tgt.isInbounds(k, l)) {
            sum += tgt.getPixel(k, l);
            count++;
          }
        }
      }
      /*Here using a running sum and dividing by loop iterations (count) helps us also deal with edges and corners.
      If a pixel has less number of neigbouring pixels, the mean calculation takes less terms.*/
      if (count != 0) {
        tgt.setPixel(i, j, checkValue(sum / count));
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::inc_1d_smoothing(image & src, image & tgt, int x, int y, int sx, int sy, int ws) {
  for (int i = x; i < sx + x; i++) {
    int total = 0;
    /*We will ignore floor(window size/2) columns from beginning and the end of the ROI */
    for (int j = y + (ws / 2); j < sy + y - (ws / 2); j++) {
      if (j == y + (ws / 2)) {
        /*Calculates the mean with a 1d window for the first element of each column*/
        int count = 0;
        for (int k = y; k <= y + (2 * (ws / 2)); k++) {
          total += src.getPixel(i, k);
          count++;
        }
        tgt.setPixel(i, j, checkValue(total / count));
      } else {
        /*For every other value, previous mean is used to achieve speed up in performance. In this implementation we use the previous 
        total to overcome the limitations of Integer division*/
        tgt.setPixel(i, j, checkValue((total + src.getPixel(i, j + (ws / 2)) - src.getPixel(i, j - (ws / 2) - 1)) / ws));
        total = total + src.getPixel(i, j + (ws / 2)) - src.getPixel(i, j - (ws / 2) - 1);
      }
    }
  }

  for (int j = y; j < sy + y; j++) {
    int total = 0;
    /*We will ignore floor(window size/2) rows from beginning and the end of the ROI */
    for (int i = x + (ws / 2); i < sx + x - (ws / 2); i++) {
      if (i == x + (ws / 2)) {
        /*Calculates the mean with a 1d window for the first element of each row*/
        int count = 0;
        for (int k = x; k <= x + (2 * (ws / 2)); k++) {
          total += tgt.getPixel(k, j);
          count++;
        }
        tgt.setPixel(i, j, checkValue(total / count));
      } else {
        /*For every other value, previous mean is used to achieve speed up in performance. In this implementation we use the previous 
        total to overcome the limitations of Integer division*/
        tgt.setPixel(i, j, checkValue((total + tgt.getPixel(i + (ws / 2), j) - tgt.getPixel(i - (ws / 2) - 1, j)) / ws));
        total = total + tgt.getPixel(i + (ws / 2), j) - tgt.getPixel(i - (ws / 2) - 1, j);
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::hist_stretching(image & src, image & tgt, image & preHist, image & postHist, int x, int y, int sx, int sy, int a, int b) {

  vector < int > weights(256, 0);

  preHist.resize(256, 256);
  /*Get the weights for each gray scale value in the image*/
  for (int i = 0; i < src.getNumberOfRows(); i++) {
    for (int j = 0; j < src.getNumberOfColumns(); j++) {
      if (src.isInbounds(i, j)) {
        weights[src.getPixel(i, j)] = weights[src.getPixel(i, j)] + 1;
      }
    }
  }

  int max = weights[0];
  for (int i = 1; i < 256; i++) {
    if (weights[i] > max)
      max = weights[i];
  }
  /*Create histogram prior to the transformation*/
  for (int j = 0; j < 256; j++) {
    for (int i = 255; i >= 255 - normalize(weights[j], max); i--) {
      if (normalize(weights[j], max) == 0) {
        continue;
      }
      preHist.setPixel(i, j, 255);

    }
  }
  /*Histogram Stretching with contrast clipping*/
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {
        if (src.getPixel(i, j) > a && src.getPixel(i, j) < b) {
          int val = ((src.getPixel(i, j) - a) * (255 / (b - a)));
          tgt.setPixel(i, j, val);
        } else if (src.getPixel(i, j) <= a) {
          tgt.setPixel(i, j, 0);
        } else if (src.getPixel(i, j) >= b) {
          tgt.setPixel(i, j, 255);
        }
      }
    }

  }

  vector < int > weights2(256, 0);
  /*Create histogram after the transformation*/
  postHist.resize(256, 256);
  for (int i = 0; i < tgt.getNumberOfRows(); i++) {
    for (int j = 0; j < tgt.getNumberOfColumns(); j++) {
      if (tgt.isInbounds(i, j)) {
        weights2[tgt.getPixel(i, j)] = weights2[tgt.getPixel(i, j)] + 1;
      }
    }
  }

  int max2 = weights2[0];
  for (int i = 1; i < 256; i++) {
    if (weights2[i] > max2)
      max2 = weights2[i];
  }

  for (int j = 0; j < 256; j++) {
    for (int i = 255; i >= 255 - normalize(weights2[j], max2); i--) {
      if (normalize(weights2[j], max2) == 0) {
        continue;
      }
      postHist.setPixel(i, j, 255);

    }
  }

}

/*------------------------------------------------------------------------------**/
void utility::thresholded_hist_stretching(image & src, image & tgt, int x, int y, int sx, int sy, int threshold) {

  vector < vector < char > > label(src.getNumberOfRows(), vector < char > (src.getNumberOfColumns(), ' '));
  /*separate image into foregrounf and background with the threshold*/
  int fmax = 0, fmin = 255, bmax = 0, bmin = 255;
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {
        if (src.getPixel(i, j) > threshold) {
          label[i][j] = 'f';
          if (src.getPixel(i, j) > fmax) {
            fmax = src.getPixel(i, j);
          }
          if (src.getPixel(i, j) < fmin) {
            fmin = src.getPixel(i, j);
          }
        } else if (src.getPixel(i, j) < threshold) {
          label[i][j] = 'b';
          if (src.getPixel(i, j) > bmax) {
            bmax = src.getPixel(i, j);
          }
          if (src.getPixel(i, j) < bmin) {
            bmin = src.getPixel(i, j);
          }
        }

      }
    }
  }

  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {
        if (label[i][j] == 'f') {
          /*histogram stretching on the foregorund*/
          if (src.getPixel(i, j) > fmin && src.getPixel(i, j) < fmax) {
            int val = ((src.getPixel(i, j) - fmin) * (255 / (fmax - fmin)));
            tgt.setPixel(i, j, val);
          } else if (src.getPixel(i, j) <= fmin) {
            tgt.setPixel(i, j, 0);
          } else if (src.getPixel(i, j) >= fmax) {
            tgt.setPixel(i, j, 255);
          }
        } else if (label[i][j] == 'b') {
          /*histogram stretching on the background*/
          if (src.getPixel(i, j) > bmin && src.getPixel(i, j) < bmax) {
            int val = ((src.getPixel(i, j) - bmin) * (255 / (bmax - bmin)));
            tgt.setPixel(i, j, val);
          } else if (src.getPixel(i, j) <= bmin) {
            tgt.setPixel(i, j, 0);
          } else if (src.getPixel(i, j) >= bmax) {
            tgt.setPixel(i, j, 255);
          }
        }
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::color_hist_stretching(image & src, image & tgt, int x, int y, int sx, int sy, int a, int b, int code) {
  if (code == 1 || code == 4) {
    /*histogram stretching on the red channel*/
    for (int i = x; i < sx + x; i++) {
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {
          if (src.getPixel(i, j, RED) > a && src.getPixel(i, j, RED) < b) {
            int val = ((src.getPixel(i, j, RED) - a) * (255 / (b - a)));
            tgt.setPixel(i, j, RED, val);
          } else if (src.getPixel(i, j) <= a) {
            tgt.setPixel(i, j, RED, 0);
          } else if (src.getPixel(i, j) >= b) {
            tgt.setPixel(i, j, RED, 255);
          }
        }
      }

    }
  }
  if (code == 2 || code == 4) {
    /*histogram stretching on the green channel*/
    for (int i = x; i < sx + x; i++) {
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {
          if (src.getPixel(i, j, GREEN) > a && src.getPixel(i, j, GREEN) < b) {
            int val = ((src.getPixel(i, j, GREEN) - a) * (255 / (b - a)));
            tgt.setPixel(i, j, GREEN, val);
          } else if (src.getPixel(i, j) <= a) {
            tgt.setPixel(i, j, GREEN, 0);
          } else if (src.getPixel(i, j) >= b) {
            tgt.setPixel(i, j, GREEN, 255);
          }
        }
      }

    }
  }
  if (code == 3 || code == 4) {
    /*histogram stretching on the blue channel*/
    for (int i = x; i < sx + x; i++) {
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {
          if (src.getPixel(i, j, BLUE) > a && src.getPixel(i, j, BLUE) < b) {
            int val = ((src.getPixel(i, j, BLUE) - a) * (255 / (b - a)));
            tgt.setPixel(i, j, BLUE, val);
          } else if (src.getPixel(i, j) <= a) {
            tgt.setPixel(i, j, BLUE, 0);
          } else if (src.getPixel(i, j) >= b) {
            tgt.setPixel(i, j, BLUE, 255);
          }
        }
      }

    }
  }
  /*If the code was 4, histogram stretching is done in all the 3 (RGB) channels*/
}
/*------------------------------------------------------------------------------**/
void utility::hsi_hist_stretching(image & src, image & tgt, image & gray, int x, int y, int sx, int sy, int a, int b) {
  vector < vector < int > > h_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < int > > s_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < int > > i_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  /*convert RGB -> HSI*/
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {

        h_value[i][j] = rgb_to_h((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE));
        s_value[i][j] = rgb_to_s((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE));
        i_value[i][j] = rgb_to_i((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE));

      }
    }
  }

  for (int i = x; i < sx + x; i++) {
    /*histogram stretching on the I channel*/
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {

        if (i_value[i][j] > a && i_value[i][j] < b) {
          int val = ((i_value[i][j] - a) * (255 / (b - a)));

          gray.setPixel(i, j, val);
          int H, S;
          H = h_value[i][j];
          S = s_value[i][j];

          vector < int > rgb = hsi_to_rgb(H, S, val);
          tgt.setPixel(i, j, RED, rgb[0]);
          tgt.setPixel(i, j, GREEN, rgb[1]);
          tgt.setPixel(i, j, BLUE, rgb[2]);

        } else if (i_value[i][j] <= a) {

          gray.setPixel(i, j, 0);
          int H, S;
          H = h_value[i][j];
          S = s_value[i][j];
          vector < int > rgb = hsi_to_rgb(H, S, 0);
          tgt.setPixel(i, j, RED, rgb[0]);
          tgt.setPixel(i, j, GREEN, rgb[1]);
          tgt.setPixel(i, j, BLUE, rgb[2]);

        } else if (i_value[i][j] >= b) {

          gray.setPixel(i, j, 255);
          int H, S;
          H = h_value[i][j];
          S = s_value[i][j];
          vector < int > rgb = hsi_to_rgb(H, S, 255);
          tgt.setPixel(i, j, RED, rgb[0]);
          tgt.setPixel(i, j, GREEN, rgb[1]);
          tgt.setPixel(i, j, BLUE, rgb[2]);
        }
      }
    }

  }

}

int utility::rgb_to_h(double R, double G, double B) {

  double r, g, b;
  r = R / (R + G + B);
  g = G / (R + G + B);
  b = B / (R + G + B);
  double temp;

  if (r == b && b == g) {
    /*H is not defined if R=G=B, i.e. S = 0*/
    temp = 0;
  } else if (b <= g) {
    temp = acos((0.5 * ((r - g) + (r - b))) / (sqrt((pow((r - g), 2)) + ((r - b) * (g - b)))));

  } else if (b > g) {
    temp = (2 * PI) - acos((0.5 * ((r - g) + (r - b))) / (sqrt((pow((r - g), 2)) + ((r - b) * (g - b)))));

  }

  return (int)((180 * temp) / PI);
}
int utility::rgb_to_s(double R, double G, double B) {
  double r, g, b;
  r = R / (R + G + B);
  g = G / (R + G + B);
  b = B / (R + G + B);
  double min_rgb = min(min(r, g), b);

  return (int)(100 * (1 - (3 * min_rgb)));

}
int utility::rgb_to_i(double R, double G, double B) {
  return (int)(((R + G + B) / (3 * 255)) * 255);
}

vector < int > utility::hsi_to_rgb(int H, int S, int I) {

  vector < int > rgb(3, 0);
  double h = (((double) H) * PI) / 180;
  double s = ((double) S) / 100;
  double i = ((double) I) / 255;

  double r, g, b;
  if (h >= 0 && h < 2 * PI / 3) {
    b = i * (1 - s);
    r = (i * (1 + ((s * cos(h)) / (cos((PI / 3) - h)))));
    g = 3 * i - (r + b);

  } else if (h >= 2 * (PI / 3) && h < 4 * (PI / 3)) {

    h = h - (2 * PI / 3);
    r = i * (1 - s);
    g = (i * (1 + ((s * cos(h)) / (cos((PI / 3) - h)))));
    b = 3 * i - (r + g);

  } else if (h >= 4 * (PI / 3) && h < 2 * PI) {
    h = h - (4 * PI / 3);
    g = i * (1 - s);
    b = (i * (1 + ((s * cos(h)) / (cos((PI / 3) - h)))));
    r = 3 * i - (g + b);

  }

  rgb[0] = (int)(r * 255);
  rgb[1] = (int)(g * 255);
  rgb[2] = (int)(b * 255);

  return rgb;

}
/*------------------------------------------------------------------------------**/
void utility::A_HSI_hist_stretching(image & src, image & tgt, int x, int y, int sx, int sy, int a, int b, int code) {
  vector < vector < int > > h_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < int > > s_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < int > > i_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  /*convert RGB -> HSI*/
  for (int i = x; i < sx + x; i++) {
    for (int j = y; j < sy + y; j++) {
      if (src.isInbounds(i, j)) {

        h_value[i][j] = normalize((rgb_to_h((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE))), 360);
        s_value[i][j] = rgb_to_s((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE));
        i_value[i][j] = normalize((rgb_to_i((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE))), 100);

      }
    }
  }

  if (code == 1 || code == 2) {
    /*histogram stretching on the I channel*/
    for (int i = x; i < sx + x; i++) {
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {

          if (i_value[i][j] > a && i_value[i][j] < b) {
            int val = ((i_value[i][j] - a) * (255 / (b - a)));

            int H, S;
            H = (h_value[i][j] * 360) / 255;
            S = (s_value[i][j] * 100) / 255;

            vector < int > rgb = hsi_to_rgb(H, S, val);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (i_value[i][j] <= a) {

            int H, S;
            H = (h_value[i][j] * 360) / 255;
            S = (s_value[i][j] * 100) / 255;
            vector < int > rgb = hsi_to_rgb(H, S, 0);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (i_value[i][j] >= b) {
            int H, S;
            H = (h_value[i][j] * 360) / 255;
            S = (s_value[i][j] * 100) / 255;
            vector < int > rgb = hsi_to_rgb(H, S, 255);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);
          }
        }
      }

    }
  }
  if (code == 1 || code == 2) {
    /*histogram stretching on the S channel*/
    for (int i = x; i < sx + x; i++) {
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {

          if (s_value[i][j] > a && s_value[i][j] < b) {
            int val = ((s_value[i][j] - a) * (255 / (b - a)));

            int H, I;
            H = (h_value[i][j] * 360) / 255;
            I = i_value[i][j];

            vector < int > rgb = hsi_to_rgb(H, val, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (s_value[i][j] <= a) {
            int H, I;
            H = (h_value[i][j] * 360) / 255;
            I = i_value[i][j];
            vector < int > rgb = hsi_to_rgb(H, 0, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (s_value[i][j] >= b) {
            int H, I;
            H = (h_value[i][j] * 360) / 255;
            I = i_value[i][j];
            vector < int > rgb = hsi_to_rgb(H, 255, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);
          }
        }
      }

    }
  }
  if (code == 2) {
    for (int i = x; i < sx + x; i++) {
      /*histogram stretching on the H channel*/
      for (int j = y; j < sy + y; j++) {
        if (src.isInbounds(i, j)) {

          if (h_value[i][j] > a && h_value[i][j] < b) {
            int val = ((h_value[i][j] - a) * (255 / (b - a)));

            int S, I;
            S = (s_value[i][j] * 100) / 255;
            I = i_value[i][j];

            vector < int > rgb = hsi_to_rgb(val, S, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (h_value[i][j] <= a) {
            int S, I;
            S = (s_value[i][j] * 100) / 255;
            I = i_value[i][j];
            vector < int > rgb = hsi_to_rgb(0, S, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);

          } else if (h_value[i][j] >= b) {
            int S, I;
            S = (s_value[i][j] * 100) / 255;
            I = i_value[i][j];
            vector < int > rgb = hsi_to_rgb(255, S, I);
            tgt.setPixel(i, j, RED, rgb[0]);
            tgt.setPixel(i, j, GREEN, rgb[1]);
            tgt.setPixel(i, j, BLUE, rgb[2]);
          }
        }
      }

    }
  }
  /* Code 1: stretching is performed on S and I channels
     Code 2: stretching is performed on H, S, and I channels*/
}
/*------------------------------------------------------------------------------**/
void utility::sobel_edge_detection(image & src, image & tgt, image & amp, image & binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2) {
  int t3;
  /*Adjustment made to also map circularly oppsotie angles, and also to map to c++'s atan2 functions range*/
  if (t2 >= 0 && t2 <= 90) {
    t2 += 90;
    t3 = t2 - 180;
  } else {
    t2 -= 270;
    t3 = t2 + 180;
  }
  vector < vector < int > > amplitude(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < double > > direction(src.getNumberOfRows(), vector < double > (src.getNumberOfColumns(), 0));
  /*Sobel's masks*/
  int x3[9] = {-1,   0,   1,   -2,    0,    2,    -1,    0,    1  };
  int y3[9] = {    1,   2,    1,    0,    0,    0,    -1,    -2,    -1  };
  int x5[25] = {    -4,    -5,    0,    5,    4,    -8,    -10,    0,    10,    8,    -10,    -20,    0,    20,    10,    -8,    -10,    0,    10,    8,    -4,    -5,    0,    5,    4  };
  int y5[25] = {    4,    8,    10,    8,    4,    5,    10,    20,    10,    5,    0,    0,    0,    0,    0,    -5,    -10,    -20,    -10,    -5,    -4,    -8,    -10,    -8,    -4  };
  /*Gradient calculation*/
  for (int i = 0; i < src.getNumberOfRows(); i++) {
    for (int j = 0; j < src.getNumberOfColumns(); j++) {
      if (tgt.isInbounds(i, j)) {

        int knum = 0;
        if (ws == 3) {
          if (i == 0 || j == 0 || i == src.getNumberOfRows()- 1 || j == src.getNumberOfColumns() - 1) {
            amplitude[i][j] = 0;
            if (isInROI(x, y, sx, sy, i , j)){
          amp.setPixel(i, j, amplitude[i][j]);}
            continue;
          }
          int Gx = 0, Gy = 0;
          for (int k = i - 1; k <= i + 1; k++) {
            for (int l = j - 1; l <= j + 1; l++) {
                Gx += src.getPixel(k, l) * x3[knum];
                Gy += src.getPixel(k, l) * y3[knum];

              knum++;
            }
          }
          amplitude[i][j] = normalize(sqrt((Gx * Gx) + (Gy * Gy)), 1443);
          if (isInROI(x, y, sx, sy, i , j)){
          amp.setPixel(i, j, amplitude[i][j]);}
          direction[i][j] = atan2(Gy, Gx);
        } else if (ws == 5) {
          if (i == 0 || j == 0 || i == 1 || j ==  1 || i == src.getNumberOfRows() - 2 || j == src.getNumberOfColumns() - 2 || i == src.getNumberOfRows() - 1 || j == src.getNumberOfColumns() - 1) {
            amplitude[i][j] =  0;
            if (isInROI(x, y, sx, sy, i , j)){
          amp.setPixel(i, j, amplitude[i][j]);}
            continue;
          }
          int Gx = 0, Gy = 0;
          for (int k = i - 2; k <= i + 2; k++) {
            for (int l = j - 2; l <= j + 2; l++) {
              
                Gx += src.getPixel(k, l) * x5[knum];
                Gy += src.getPixel(k, l) * y5[knum];
              knum++;
            }
          }
          amplitude[i][j] = normalize(sqrt((Gx * Gx) + (Gy * Gy)), 30293);
          if (isInROI(x, y, sx, sy, i , j)){
          amp.setPixel(i, j, amplitude[i][j]);}
          direction[i][j] = atan2(Gy, Gx);
        }
      }
    }
  }
/*Binarization to produce better edges*/
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (binAmp.isInbounds(i, j)) {
        if (amp.getPixel(i, j) < t1)
          binAmp.setPixel(i, j, MINRGB);
        else
          binAmp.setPixel(i, j, MAXRGB);
      }
    }
  }
/*Binarization based on angle*/
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (tgt.isInbounds(i, j)) {
        if (amp.getPixel(i, j) > t1 && ((direction[i][j] < ((t2 + 10) * PI) / 180 && direction[i][j] > ((t2 - 10) * PI) / 180) || (direction[i][j] < ((t3 + 10) * PI) / 180 && direction[i][j] > ((t3 - 10) * PI) / 180)))  
          tgt.setPixel(i, j, MAXRGB);
        else
          tgt.setPixel(i, j, MINRGB);
      }
    }
  }
}
/*------------------------------------------------------------------------------**/
void utility::hsi_sobel_edge_detection(image & src, image & tgt, image & amp, image & binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2) {
  int t3;
  if (t2 >= 0 && t2 <= 90) { /*Adjustment made to also map circularly oppsotie angles, and also to map to c++'s atan2 functions range*/
    t2 += 90;
    t3 = t2 - 180;
  } else {
    t2 -= 270;
    t3 = t2 + 180;
  }
  image temp;
  temp.resize(src.getNumberOfRows(), src.getNumberOfColumns());
  temp.copyImage(src);
  vector < vector < int > > i_value(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  /*convert RGB -> HSI*/
  for (int i = 0; i < src.getNumberOfRows(); i++) {
    for (int j = 0; j < src.getNumberOfColumns(); j++) {
      if (src.isInbounds(i, j)) {

        i_value[i][j] = normalize((rgb_to_i((double) src.getPixel(i, j, RED), (double) src.getPixel(i, j, GREEN), (double) src.getPixel(i, j, BLUE))), 100);
        temp.setPixel(i, j, RED, i_value[i][j]);
        temp.setPixel(i, j, GREEN, i_value[i][j]);
        temp.setPixel(i, j, BLUE, i_value[i][j]);

      }
    }
  }
  vector < vector < int > > amplitude(src.getNumberOfRows(), vector < int > (src.getNumberOfColumns(), 0));
  vector < vector < double > > direction(src.getNumberOfRows(), vector < double > (src.getNumberOfColumns(), 0));
  /*Sobel's masks*/
  int x3[9] = {    -1,    0,    1,    -2,    0,    2,    -1,    0,    1  };
  int y3[9] = {    1,    2,    1,    0,    0,    0,    -1,    -2,    -1  };
  int x5[25] = {    -4,    -5,    0,    5,    4,    -8,    -10,    0,    10,    8,    -10,    -20,    0,    20,    10,    -8,    -10,    0,    10,    8,    -4,    -5,    0,    5,    4  };
  int y5[25] = {    4,    8,    10,    8,    4,    5,    10,    20,    10,    5,    0,    0,    0,    0,    0,    -5,    -10,    -20,    -10,    -5,    -4,    -8,    -10,    -8,    -4  };
  /*Gradient calculation*/
  for (int i = 0; i < src.getNumberOfRows(); i++) {
    for (int j = 0; j < src.getNumberOfColumns(); j++) {
      if (tgt.isInbounds(i, j)) {

        int knum = 0;
        if (ws == 3) {
          if (i == 0 || j == 0 || i == src.getNumberOfRows()- 1 || j == src.getNumberOfColumns() - 1)  {
            amplitude[i][j] = 0;
            if (isInROI(x,y,sx,sy,i,j)){
            amp.setPixel(i, j, RED, 0);
            amp.setPixel(i, j, GREEN, 0);
            amp.setPixel(i, j, BLUE, 0);}
            continue;
          }
          int Gx = 0, Gy = 0;
          for (int k = i - 1; k <= i + 1; k++) {
            for (int l = j - 1; l <= j + 1; l++) {
                Gx += temp.getPixel(k, l) * x3[knum];
                Gy += temp.getPixel(k, l) * y3[knum];
              knum++;
            }
          }
          amplitude[i][j] = normalize(sqrt((Gx * Gx) + (Gy * Gy)), 1443);
          if (isInROI(x,y,sx,sy,i,j)){
          amp.setPixel(i, j, RED, amplitude[i][j]);
          amp.setPixel(i, j, GREEN, amplitude[i][j]);
          amp.setPixel(i, j, BLUE, amplitude[i][j]);}
          direction[i][j] = atan2(Gy, Gx);
        } else if (ws == 5) {
          if (i == 0 || j == 0 || i == 1 || j ==  1 || i == src.getNumberOfRows() - 2 || j == src.getNumberOfColumns() - 2 || i == src.getNumberOfRows() - 1 || j == src.getNumberOfColumns() - 1){
            amplitude[i][j] = 0;
            if (isInROI(x,y,sx,sy,i,j)){
            amp.setPixel(i, j, RED, 0);
            amp.setPixel(i, j, GREEN, 0);
            amp.setPixel(i, j, BLUE, 0);}
            continue;
          }
          int Gx = 0, Gy = 0;
          for (int k = i - 2; k <= i + 2; k++) {
            for (int l = j - 2; l <= j + 2; l++) {
              if (isInROI(x, y, sx, sy, k, l)) {
                Gx += src.getPixel(k, l) * x5[knum];
                Gy += src.getPixel(k, l) * y5[knum];

              }
              knum++;
            }
          }
          amplitude[i][j] = normalize(sqrt((Gx * Gx) + (Gy * Gy)), 30293);
          if (isInROI(x,y,sx,sy,i,j)){
          amp.setPixel(i, j, RED, amplitude[i][j]);
          amp.setPixel(i, j, GREEN, amplitude[i][j]);
          amp.setPixel(i, j, BLUE, amplitude[i][j]);}
          direction[i][j] = atan2(Gy, Gx);
        }
      }
    }
  }
/*Binarization to produce better edges*/
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (binAmp.isInbounds(i, j)) {
        if (amp.getPixel(i, j) < t1) {
          binAmp.setPixel(i, j, RED, MINRGB);
          binAmp.setPixel(i, j, GREEN, MINRGB);
          binAmp.setPixel(i, j, BLUE, MINRGB);
        } else {
          binAmp.setPixel(i, j, RED, MAXRGB);
          binAmp.setPixel(i, j, GREEN, MAXRGB);
          binAmp.setPixel(i, j, BLUE, MAXRGB);
        }
      }
    }
  }
/*Binarization based on angle*/
  for (int i = x; i < x + sx; i++) {
    for (int j = y; j < y + sy; j++) {
      if (tgt.isInbounds(i, j)) {
        if (amp.getPixel(i, j) > t1 && ((direction[i][j] < ((t2 + 10) * PI) / 180 && direction[i][j] > ((t2 - 10) * PI) / 180) || (direction[i][j] < ((t3 + 10) * PI) / 180 && direction[i][j] > ((t3 - 10) * PI) / 180))) {  
          tgt.setPixel(i, j, RED, MAXRGB);
          tgt.setPixel(i, j, GREEN, MAXRGB);
          tgt.setPixel(i, j, BLUE, MAXRGB);
        } else {
          tgt.setPixel(i, j, RED, MINRGB);
          tgt.setPixel(i, j, GREEN, MINRGB);
          tgt.setPixel(i, j, BLUE, MINRGB);
        }
      }
    }
  }

}

/*------------------------------------------------------------------------------**/
void utility::cv_gray(Mat & src, Mat & tgt) {
  cvtColor(src, tgt, COLOR_BGR2GRAY);

}

/*------------------------------------------------------------------------------**/
void utility::cv_avgblur(Mat & src, Mat & tgt, int WindowSize) {
  blur(src, tgt, Size(WindowSize, WindowSize));
}
/*------------------------------------------------------------------------------**/
void utility::cv_hsi_sobel(cv::Mat & src, cv::Mat & tgt, cv::Mat & amp, cv::Mat & binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2) {

  Rect roi(x, y, sx, sy);
  Mat grad_x, grad_y;
  Mat temp;
  tgt(roi).copyTo(temp);
  cvtColor(temp, temp, COLOR_BGR2HSV); //color scale conversion
  Mat channels[3]; // splitting three channels
  split(temp, channels);
  /*Sobel in x and y direction*/
  Sobel(channels[2], grad_x, -1, 1, 0, ws); 
  Sobel(channels[2], grad_y, -1, 0, 1, ws);
  Mat grad;
  grad.create(channels[2].size(), channels[2].type());
  Mat dir;
  dir.create(grad_x.size(), grad_x.type());
  int dirn;
  /*Gradient and direction calculation*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {
      grad.at < uchar > (i, j) = (uchar) sqrt(((int) grad_x.at < uchar > (i, j) * (int) grad_x.at < uchar > (i, j)) + (int)(grad_y.at < uchar > (i, j) * (int) grad_y.at < uchar > (i, j)));
      if ((int) grad_x.at < uchar > (i, j) == 0) {
        dirn = 101;
      } else {
        dirn = 90 - (int)(atan((int) grad_y.at < uchar > (i, j) / (int) grad_x.at < uchar > (i, j)) * 180 / PI);
      }

      dir.at < uchar > (i, j) = (uchar) dirn;
    }
  }

  grad.copyTo(channels[0]);
  grad.copyTo(channels[1]);
  grad.copyTo(channels[2]);
  merge(channels, 3, temp);
  temp.copyTo(binAmp(roi));
  temp.copyTo(amp(roi));
/*Binarization to improve edges*/
  threshold(binAmp(roi), binAmp(roi), t1, 255, THRESH_BINARY);
  Mat tgtbin;
  tgtbin.create(temp.size(), temp.type());
  Mat ch[3];
  split(tgtbin, ch);
/*angle based binarization*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {

      if ((int) grad.at < uchar > (i, j) > t1 && ((int) dir.at < uchar > (i, j) < (t2 + 10) && (int) dir.at < uchar > (i, j) > (t2 - 10))) {
        ch[0].at < uchar > (i, j) = MAXRGB;
        ch[1].at < uchar > (i, j) = MAXRGB;
        ch[2].at < uchar > (i, j) = MAXRGB;
      } else {
        ch[0].at < uchar > (i, j) = MINRGB;
        ch[1].at < uchar > (i, j) = MINRGB;
        ch[2].at < uchar > (i, j) = MINRGB;
      }

    }
  }
  merge(ch, 3, tgtbin);
  tgtbin.copyTo(tgt(roi));

}
/*------------------------------------------------------------------------------**/
void utility::cv_sobel(cv::Mat & src, cv::Mat & tgt, cv::Mat & amp, cv::Mat & binAmp, int x, int y, int sx, int sy, int ws, int t1, int t2) {
  Rect roi(x, y, sx, sy);
  Mat grad_x, grad_y;
  Mat temp = tgt(roi);
  /*Sobel in x and y direction*/
  Sobel(temp, grad_x, CV_8U, 1, 0, ws);
  Sobel(temp, grad_y, CV_8U, 0, 1, ws);
  Mat grad;
  grad.create(grad_x.size(), grad_x.type());
  Mat dir;
  dir.create(grad_x.size(), grad_x.type());
  int dirn;
  /*Gradient and direction calculation*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {
      grad.at < uchar > (i, j) = (uchar) sqrt(((int) grad_x.at < uchar > (i, j) * (int) grad_x.at < uchar > (i, j)) + (int)(grad_y.at < uchar > (i, j) * (int) grad_y.at < uchar > (i, j)));
      if ((int) grad_x.at < uchar > (i, j) == 0) {
        dirn = 101;
      } else {
        dirn = 90 - (int)(atan((int) grad_y.at < uchar > (i, j) / (int) grad_x.at < uchar > (i, j)) * 180 / PI);
      }

      dir.at < uchar > (i, j) = (uchar) dirn;
    }
  }

  grad.copyTo(amp(roi));
  grad.copyTo(binAmp(roi));
  /*Binarization to improve edges*/
  threshold(binAmp(roi), binAmp(roi), t1, 255, THRESH_BINARY);
  Mat tgtbin;
  tgtbin.create(temp.size(), temp.type());
/*angle based binarization*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {

      if ((int) grad.at < uchar > (i, j) > t1 && ((int) dir.at < uchar > (i, j) < (t2 + 10) && (int) dir.at < uchar > (i, j) > (t2 - 10)))
        tgtbin.at < uchar > (i, j) = MAXRGB;
      else
        tgtbin.at < uchar > (i, j) = MINRGB;

    }
  }
  tgtbin.copyTo(tgt(roi));
}
/*------------------------------------------------------------------------------**/

void utility::cv_canny(cv::Mat & src, cv::Mat & tgt, int x, int y, int sx, int sy, int ws, int t1, int t2) {
  Rect roi(x, y, sx, sy);
  Canny(src(roi), tgt(roi), t1, t2, ws, true);

}
/*------------------------------------------------------------------------------**/
void utility::cv_hsi_canny(cv::Mat & src, cv::Mat & tgt, int x, int y, int sx, int sy, int ws, int t1, int t2) {
  Rect roi(x, y, sx, sy);
  Mat temp;
  tgt(roi).copyTo(temp);
  cvtColor(temp, temp, COLOR_BGR2HSV);//color scale conversion
  Mat channels[3];
  split(temp, channels); // split image channels
  Canny(channels[2], channels[2], t1, t2, ws, true);// apply canny to V channel
  channels[2].copyTo(channels[0]);
  channels[2].copyTo(channels[1]);
  merge(channels, 3, temp);
  temp.copyTo(tgt(roi));

}
/*------------------------------------------------------------------------------**/
void utility::cv_hist_stretching(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy)
{
  Rect roi (x,y,sx,sy);
  double min, max;
  Mat temp = tgt(roi);
  minMaxLoc(src(roi), &min, &max);
  for (int i = 0; i < temp.rows; i++) {
    for (int j = 0; j < temp.cols; j++) {

      temp.at<uchar>(i,j) = (uchar)(255*((temp.at<uchar>(i,j) - min)/(max-min)));

    }
  }
  temp.copyTo(tgt(roi));
}
/*------------------------------------------------------------------------------**/
void utility::cv_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy)
{
  Rect roi(x,y,sx,sy);
  equalizeHist(tgt(roi), tgt(roi));
}
/*------------------------------------------------------------------------------**/
void utility::cv_V_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy)
{
  Rect roi(x, y, sx, sy);
  Mat temp;
  tgt(roi).copyTo(temp);
  cvtColor(temp, temp, COLOR_BGR2HSV);//color scale conversion
  Mat channels[3];
  split(temp, channels); // split image channels
  equalizeHist(channels[2], channels[2]);// apply canny to V channel
  merge(channels, 3, temp);
  cvtColor(temp, temp, COLOR_HSV2BGR);
  temp.copyTo(tgt(roi));
}
/*------------------------------------------------------------------------------**/   
void utility::cv_HSV_hist_equalization(cv::Mat &src, cv::Mat &tgt,int x, int y, int sx, int sy, int h, int s, int v)
{
  Rect roi(x, y, sx, sy);
  Mat temp;
  tgt(roi).copyTo(temp);
  cvtColor(temp, temp, COLOR_BGR2HSV);//color scale conversion
  Mat channels[3];
  split(temp, channels); // split image channels
  if (h ==1)
  {equalizeHist(channels[0], channels[0]);}
  if (s == 1)
  {equalizeHist(channels[1], channels[1]);}
  if (v == 1)
  {equalizeHist(channels[2], channels[2]);}
  merge(channels, 3, temp);
  cvtColor(temp, temp, COLOR_HSV2BGR);
  temp.copyTo(tgt(roi));
}
/*------------------------------------------------------------------------------**/
void utility::cv_hist_eq_sobel(cv::Mat &src, cv::Mat &tgt,cv::Mat &he, int x, int y, int sx, int sy, int ws)
{
  Rect roi(x, y, sx, sy);
  Mat grad_x, grad_y;
  Mat no_he;
  tgt(roi).copyTo(no_he);
  /*Sobel in x and y direction*/
  Sobel(no_he, grad_x, CV_8U, 1, 0, ws);
  Sobel(no_he, grad_y, CV_8U, 0, 1, ws);
  Mat grad;
  grad.create(grad_x.size(), grad_x.type());
  /*Gradient calculation*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {
      grad.at < uchar > (i, j) = (uchar) sqrt(((int) grad_x.at < uchar > (i, j) * (int) grad_x.at < uchar > (i, j)) + (int)(grad_y.at < uchar > (i, j) * (int) grad_y.at < uchar > (i, j)));      
    }
  }
  grad.copyTo(no_he);
  Mat w_he;
  tgt(roi).copyTo(w_he);
  equalizeHist(w_he, w_he);
  Sobel(w_he, grad_x, CV_8U, 1, 0, ws);
  Sobel(w_he, grad_y, CV_8U, 0, 1, ws);
  /*Gradient calculation*/
  for (int i = 0; i < grad.rows; i++) {
    for (int j = 0; j < grad.cols; j++) {
      grad.at < uchar > (i, j) = (uchar) sqrt(((int) grad_x.at < uchar > (i, j) * (int) grad_x.at < uchar > (i, j)) + (int)(grad_y.at < uchar > (i, j) * (int) grad_y.at < uchar > (i, j)));      
    }
  }
  grad.copyTo(he(roi));
  tgt(roi) = he(roi) - no_he;

  
}
/*------------------------------------------------------------------------------**/
void utility::cv_hist_eq_canny(cv::Mat &src, cv::Mat &tgt, cv::Mat &he, int x, int y, int sx, int sy, int ws, int t1, int t2)
{
  Rect roi(x, y, sx, sy);
  Mat no_he;
  tgt(roi).copyTo(no_he);
  Canny(no_he, no_he, t1, t2, ws, true);
  
  Mat w_he;
  tgt(roi).copyTo(w_he);
  equalizeHist(w_he, w_he);
  Canny(w_he, w_he, t1, t2, ws, true);
  w_he.copyTo(he(roi));
  tgt(roi) = w_he - no_he;
}
/*------------------------------------------------------------------------------**/
void utility::cv_QR_code(cv::Mat &src, cv::Mat &tgt, int HE)
{
  QRCodeDetector qrd = QRCodeDetector();
  if (HE ==1)
    equalizeHist(tgt, tgt);
  string link = qrd.detectAndDecode(tgt);
  if (link.length() > 0)
  {
    cout<< "Decoded data: "<< link<<endl;
  }else
  {
    cout<<"QR code not detected"<<endl;
  }
}
/*------------------------------------------------------------------------------**/