#include <cstdlib>
#include <iostream>
#include "image.h"

using namespace std;

int main(int argc, char** argv){

  Image* gray = Image::readPNM("house.pgm");
  gray->writePGM("/home/user/Desktop/gray");

  delete gray;

  Image* rgb = Image::readPNM("house.ppm");
  rgb->writePPM("/home/user/Desktop/rgb");

  delete rgb;

  gray->calculateHistogramGray();
  rgb->calculateHistogramRGB();

  Image* brightnessGray = gray->changeBrightnessGray(100);
  brightnessGray->writePGM("/home/user/Desktop/grayBrightness");

  delete brightnessGray;

  Image* brightnessRGB = rgb->changeBrightnessRGB(100);
  brightnessRGB->writePPM("/home/user/Desktop/rgbBrightness");

  delete brightnessRGB;

  Image* contrastGray = gray->changeContrastGray(128);
  contrastGray->writePGM("/home/user/Desktop/grayContrast");

  delete contrastGray;

  Image* contrastRGB = rgb->changeContrastRGB(128);
  contrastRGB->writePPM("/home/user/Desktop/rgbContrast");

  delete contrastRGB;

  Image* convertRGBtoGray = rgb->convertRGBtoGray();
  convertRGBtoGray->writePGM("/home/user/Desktop/convertRGBtoGray");

  delete convertRGBtoGray;

  Image* thresholdingGray = gray->thresholding(128);
  thresholdingGray->writePGM("/home/user/Desktop/thresholdingGray");

  delete thresholdingGray;

  Image* colourInversionGray = gray->colourInversionGray();
  colourInversionGray->writePGM("/home/user/Desktop/colourInversionGray");

  delete colourInversionGray;

  Image* colourInversionRGB = rgb->colourInversionRGB();
  colourInversionRGB->writePPM("/home/user/Desktop/colourInversionRGB");

  delete colourInversionRGB;

  Image* solarizationGray = gray->solarizationGray(128);
  solarizationGray->writePGM("/home/user/Desktop/solarizationGray");

  delete solarizationGray;

  Image* solarizationRGB = rgb->solarizationRGB(128);
  solarizationRGB->writePPM("/home/user/Desktop/solarizationRGB");

  delete solarizationRGB;

  Image* nearestNeighborInterpolationGray = gray->nearestNeighborInterpolationGray(2,2);
  nearestNeighborInterpolationGray->writePGM("/home/user/Desktop/nearestNeighborInterpolationGray");

  delete nearestNeighborInterpolationGray;

  Image* nearestNeighborInterpolationRGB = rgb->nearestNeighborInterpolationRGB(2,2);
  nearestNeighborInterpolationRGB->writePPM("/home/user/Desktop/nearestNeighborInterpolationRGB");

  delete nearestNeighborInterpolationRGB;

  Image* bilinearInterpolationGray = gray->bilinearInterpolationGray(2,2);
  bilinearInterpolationGray->writePGM("/home/user/Desktop/bilinearInterpolationGray");

  delete bilinearInterpolationGray;

  Image* bilinearInterpolationRGB = rgb->bilinearInterpolationRGB(2,2);
  bilinearInterpolationRGB->writePPM("/home/user/Desktop/bilinearInterpolationRGB");

  delete bilinearInterpolationRGB;

  return EXIT_SUCCESS;
}
