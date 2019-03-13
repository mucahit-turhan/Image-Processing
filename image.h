#include <string>

class Image{

public:
        Image(int width, int height, int channel);
        ~Image();

        static Image* newGrayImage(int width, int height);
        static Image* newRgbImage(int width, int height);
        static Image* readPNM(const std::string& filename);

        int getWidth(){return width;}
        int getHeight(){return height;}
        int getChannel(){return channel;}

        unsigned char* getPixels(){return pixels;}
        unsigned char* getPixels(int i){return (pixels + (i*width*channel));}
        void setPixels(unsigned char* newPixels){pixels = newPixels;}

        bool writePGM(const std::string& filename);
        bool writePPM(const std::string& filename);

        void calculateHistogramGray(); //Only print number of pixels for each tonal value
        void calculateHistogramRGB(); //Only print number of pixels for each tonal value

        unsigned char truncate(unsigned char value);
        
        Image* changeBrightnessGray(unsigned char value);
        Image* changeBrightnessRGB(unsigned char value);
        Image* changeContrastGray(unsigned char value);
        Image* changeContrastRGB(unsigned char value);
        Image* convertRGBtoGray();
        Image* thresholding(unsigned char value);

        Image* colourInversionGray();
        Image* colourInversionRGB();
        Image* solarizationGray(unsigned char thresholdValue);
        Image* solarizationRGB(unsigned char thresholdValue);

        Image* nearestNeighborInterpolationGray(double x_rate, double y_rate);
        Image* nearestNeighborInterpolationRGB(double x_rate, double y_rate);
        Image* bilinearInterpolationGray(double x_rate, double y_rate);
        Image* bilinearInterpolationRGB(double x_rate, double y_rate);

        int calculateBilinear(int x, int y, int pixel);

private:
        int width;
        int height;
        int channel;
        unsigned char* pixels;
};
