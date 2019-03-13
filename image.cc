#include "image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#define PI 3.14159265

using namespace std;

Image::Image(int width, int height, int channel){
  this->width = width;
  this->height = height;
  this->channel = channel;
  this->pixels = new unsigned char[width*height*channel];
}

Image::~Image(){
  delete [] pixels;
}

Image* Image::newGrayImage(int width, int height){
  return new Image(width, height, 1);
}

Image* Image::newRgbImage(int width, int height){
  return new Image(width, height, 3);
}

bool Image::writePGM(const string& filename){
  if(channel != 1){
    cout<<"You can use writePgm function for only gray scale image"<<endl;
    return false;
  }

  ofstream ofs;
  string fullFilename = filename + ".pgm";

  ofs.open(fullFilename.c_str(), ios::out | ios::binary | ios::trunc);
  ofs<<"P5"<<"\n";
  ofs<<width<<" "<<height<<" "<<"255"<<"\n";

  for(int i = 0; i < height ; i++){
    ofs.write(reinterpret_cast<char*>(getPixels(i)), channel*width*sizeof(unsigned char));
  }

  ofs.close();
  return true;
}

bool Image::writePPM(const string& filename){
  if(channel != 3){
    cout<<"You can use writePgm function for only gray scale image"<<endl;
    return false;
  }

  ofstream ofs;
  string fullFilename = filename + ".ppm";

  ofs.open(fullFilename.c_str(), ios::out | ios::binary | ios::trunc);
  ofs<<"P6"<<"\n";
  ofs<<width<<" "<<height<<" "<<"255"<<"\n";

  for(int i = 0; i < height ; i++){
    ofs.write(reinterpret_cast<char*>(getPixels(i)), channel*width*sizeof(unsigned char));
  }

  ofs.close();
  return true;
}

Image* Image::readPNM(const std::string& filename){

  ifstream ifs;
  ifs.open(filename, ios::in | ios::binary);

  stringstream ss;

  string magicNumber ="";
  getline(ifs,magicNumber);
  cout<<magicNumber<<endl;

  int channel;

  if(magicNumber.compare("P5") == 0)
    channel = 1;
  else if(magicNumber.compare("P6") == 0)
    channel = 3;
  else{
    cout<<"Magic Number should be P5 or P6"<<endl;
    exit(EXIT_FAILURE);
  }

  cout<<channel<<endl;

  int width;
  int height;

  string comment = "";
  getline(ifs,comment);

  size_t n;
  n = comment.find(" ");
  width = atoi(comment.substr(0,n).c_str());
  cout<<width<<endl;
  height = atoi(comment.substr(n).c_str());
  cout<<height<<endl;

  int maxValue;
  string max = "";
  getline(ifs,max);
  maxValue = atoi(max.c_str());
  cout<<maxValue<<endl;

  Image* img = new Image(width, height, channel);

  unsigned char *pixelArray = (unsigned char *) new unsigned char [width*height*channel];
  ifs.read( reinterpret_cast<char *>(pixelArray), (width*height*channel)*sizeof(unsigned char));

  img->setPixels(pixelArray);

  return img;
}

void Image::calculateHistogramGray(){
  int count_arry[256] = {};
  int count = 0;

  for(int i = 0; i < 255; i++){
    for(int j = 0; j < width*height*channel; j++){
      if(pixels[j] == i)
        count++;
    }
    count_arry[i] = count;
    count = 0;
  }

  for(int k = 0; k < 255; k++)
    cout<<count_arry[k]<<" ";
}

void Image::calculateHistogramRGB(){
  int count_arry_R[256] = {};
  int count_arry_G[256] = {};
  int count_arry_B[256] = {};

  int countR = 0;
  int countG = 0;
  int countB = 0;

  for(int i = 0; i < 255; i++){
    for(int j = 0; j < width*height*channel; j+=3){
      if(pixels[j] == i)
        countR++;
      if(pixels[j+1] == i)
        countG++;
      if(pixels[j+2] == i)
        countB++;
    }
    count_arry_R[i] = countR;
    count_arry_G[i] = countG;
    count_arry_B[i] = countB;
    countR = 0;
    countG = 0;
    countB = 0;
  }

  cout<<"R"<<endl;

  for(int k = 0; k < 255; k++)
    cout<<count_arry_R[k]<<" ";

  cout<<endl;
  cout<<"G"<<endl;

  for(int k = 0; k < 255; k++)
    cout<<count_arry_G[k]<<" ";

  cout<<endl;
  cout<<"B"<<endl;

  for(int k = 0; k < 255; k++)
    cout<<count_arry_B[k]<<" ";
}

unsigned char Image::truncate(unsigned char value){
    if(value < 0)
      value = 0;
    else if(value > 255)
      value = 255;
    return value;
}

Image* Image::changeBrightnessGray(unsigned char value){

  Image* newImage = Image::newGrayImage(width, height);

  for(int i = 0; i < width*height*channel; i++)
    newImage->getPixels()[i] = truncate( pixels[i] + value );

  return newImage;
}

Image* Image::changeBrightnessRGB(unsigned char value){

  Image* newImage = Image::newRgbImage(width, height);

  for(int i = 0; i < width*height*channel; i += 3){
    newImage->getPixels()[i] = truncate( pixels[i] + value);
    newImage->getPixels()[i+1] = truncate( pixels[i+1] + value);
    newImage->getPixels()[i+2] = truncate( pixels[i+2] + value);
  }

  return newImage;
}

Image* Image::changeContrastGray(unsigned char value){

  float factor = (259*(value + 255)) / (255 * (259 - value));

  Image* newImage = Image::newGrayImage(width, height);

  for(int i = 0; i < width*height*channel; i++)
    newImage->getPixels()[i] = truncate(factor * (pixels[i] - 128) + 128);

  return newImage;
}

Image* Image::changeContrastRGB(unsigned char value){

  float factor = (259*(value + 255)) / (255 * (259 - value));

  Image* newImage = Image::newRgbImage(width, height);

  for(int i = 0; i < width*height*channel; i += 3){
    newImage->getPixels()[i] = truncate(factor * (pixels[i] - 128) + 128);
    newImage->getPixels()[i+1] = truncate(factor * (pixels[i+1] - 128) + 128);
    newImage->getPixels()[i+2] = truncate(factor * (pixels[i+2] - 128) + 128);
  }

  return newImage;
}

Image* Image::convertRGBtoGray(){

  Image* newImage = Image::newGrayImage(width, height);

  int j = 0;

  for(int i = 0; i < width*height*channel; i += 3){

    newImage->getPixels()[j] = truncate(0.299 * pixels[i] + 0.587 * pixels[i+1] + 0.114 * pixels[i+2] );
    j += 1;

  }

  return newImage;
}

Image* Image::thresholding(unsigned char value){

  Image* newImage;

  if(channel == 1)
    newImage = Image::newGrayImage(width, height);
  else
    newImage = convertRGBtoGray();

  for(int i = 0; i < width*height*channel; i++){

    if(pixels[i] < value)
      newImage->getPixels()[i] = 0;
    else
      newImage->getPixels()[i] = 255;
  }

  return newImage;
}

Image* Image::colourInversionGray(){

  Image* newImage = Image::newGrayImage(width, height);

  for(int i = 0; i < width*height*channel; i++)
    newImage->getPixels()[i] = 255 - pixels[i];

  return newImage;
}

Image* Image::colourInversionRGB(){

  Image* newImage = Image::newRgbImage(width, height);

  for(int i = 0; i < width*height*channel; i += 3){
    newImage->getPixels()[i] = 255 - pixels[i];
    newImage->getPixels()[i+1] = 255 - pixels[i+1];
    newImage->getPixels()[i+2] = 255 - pixels[i+2];
  }

  return newImage;
}

Image* Image::solarizationGray(unsigned char thresholdValue){

  Image* newImage = Image::newGrayImage(width, height);

  for(int i = 0; i < width*height*channel; i++){

    if(pixels[i] < thresholdValue)
      newImage->getPixels()[i] = 255 - pixels[i];
    else
      newImage->getPixels()[i] = pixels[i];
  }

  return newImage;
}

Image* Image::solarizationRGB(unsigned char thresholdValue){

  Image* newImage = Image::newRgbImage(width, height);

  for(int i = 0; i < width*height*channel; i += 3){

    if(getPixels()[i] < thresholdValue)
      newImage->getPixels()[i] = 255 - pixels[i];
    else
      newImage->getPixels()[i] = pixels[i];

    if(getPixels()[i+1] < thresholdValue)
      newImage->getPixels()[i+1] = 255 - pixels[i+1];
    else
      newImage->getPixels()[i+1] = pixels[i+1];

    if(getPixels()[i+2] < thresholdValue)
      newImage->getPixels()[i+2] = 255 - pixels[i+2];
    else
      newImage->getPixels()[i+2] = pixels[i+2];
  }
  return newImage;
}

Image* Image::nearestNeighborInterpolationGray(double x_rate, double y_rate){

  double newWidth = floor(x_rate*width);
  double newHeight = floor(y_rate*height);

  double x_ratio = width / newWidth;
  double y_ratio = height / newHeight;

  Image* newImage = Image::newGrayImage(newWidth, newHeight);

  double x, y;

  for(int i = 0; i < newHeight; i++){
    for(int j = 0; j < newWidth; j++){
      x = floor(j * x_ratio);
      y = floor(i * y_ratio);
      newImage->getPixels()[(int)(i*newWidth)+j] = pixels[(int)((y*width) + x)];
    }
  }

  return newImage;
}

Image* Image::nearestNeighborInterpolationRGB(double x_rate, double y_rate){

  double newWidth = floor(x_rate*width);
  double newHeight = floor(y_rate*height);

  double x_ratio = width / newWidth;
  double y_ratio = height / newHeight;

  Image* newImage = Image::newRgbImage(newWidth, newHeight);

  double x,y;

  for(int i = 0; i < newHeight; i++){
    for(int j = 0; j < newWidth*channel; j+=3){
      x = floor(j/3 * x_ratio);
      y = floor(i * y_ratio);
      newImage->getPixels()[(int)((i*newWidth*channel) + j)] = pixels[(int)floor((y*width*channel) + 3*x)];
      newImage->getPixels()[(int)((i*newWidth*channel) + j + 1)] = pixels[(int)floor((y*width*channel) + 3*x + 1)];
      newImage->getPixels()[(int)((i*newWidth*channel) + j + 2)] = pixels[(int)floor((y*width*channel) + 3*x + 2)];
    }
  }

  return newImage;
}

Image* Image::bilinearInterpolationGray(double x_rate, double y_rate){

  double newWidth = floor(x_rate*width);
  double newHeight = floor(y_rate*height);

  double x_ratio = width / newWidth;
  double y_ratio = height / newHeight;

  Image* newImage = Image::newGrayImage(newWidth, newHeight);

  double x,y;
  double r1,r2;
  int x1,x2,y1,y2;
  int p;
  unsigned char q11,q12,q21,q22;

  for(int i = 0; i < newHeight; i++){
    for(int j = 0; j < newWidth; j++){
      x = j * x_ratio;
      y = i * y_ratio;

      newImage->getPixels()[(int)(i*newWidth)+j] = calculateBilinear(x, y, 0);
    }
  }

  return newImage;
}

Image* Image::bilinearInterpolationRGB(double x_rate, double y_rate){

  double newWidth = floor(x_rate*width);
  double newHeight = floor(y_rate*height);

  double x_ratio = width / newWidth;
  double y_ratio = height / newHeight;

  Image* newImage = Image::newRgbImage(newWidth, newHeight);

  double x,y;

  for(int i = 0; i < newHeight; i++){
    for(int j = 0; j < newWidth*channel; j+=3){
      x = j/3 * x_ratio;
      y = i * y_ratio;

      newImage->getPixels()[(int)(i*newWidth*channel)+j] = calculateBilinear(x, y, 0);
      newImage->getPixels()[(int)(i*newWidth*channel)+j+1] = calculateBilinear(x, y, 1);
      newImage->getPixels()[(int)(i*newWidth*channel)+j+2] = calculateBilinear(x, y, 2);
    }
  }

  return newImage;
}

int Image::calculateBilinear(int x, int y, int pixel){

  unsigned char q11,q12,q21,q22;
  int x1,x2,y1,y2;
  double r1,r2;
  int p;

  if(channel == 3){
    x1 = floor(x) + pixel;
    x2 = x1 + 3;
    y2 = floor(y) + pixel;
    y1 = y2 + 3;

    q12 = pixels[(int)((y2*width*channel) + 3*x1 + pixel)];
    q22 = pixels[(int)((y2*width*channel) + 3*x1 + 3 + pixel)];
    q11 = pixels[(int)((y2*width*channel) + 3*x1 + width*channel + pixel)];
    q21 = pixels[(int)((y2*width*channel) + 3*x1 + width*channel + 3 + pixel)];
  }
  else if(channel == 1){
    x1 = floor(x);
    x2 = x1+1;
    y2 = floor(y);
    y1 = y2+1;

    q12 = pixels[(int)((y2*width) + x1)];
    q22 = pixels[(int)((y2*width) + x1 + 1)];
    q11 = pixels[(int)((y2*width) + x1 + width)];
    q21 = pixels[(int)((y2*width) + x1 + width + 1)];
  }
  else{
    cout<<"Channel is not compatible"<<endl;
    exit(EXIT_FAILURE);
  }

  r1 = ((x2-x)/(x2-x1))*q11 + ((x-x1)/(x2-x1))*q21;
  r2 = ((x2-x)/(x2-x1))*q12 + ((x-x1)/(x2-x1))*q22;
  p =  ((y2-y)/(y2-y1))*r1 + ((y-y1)/(y2-y1))*r2;

  return p;
}
