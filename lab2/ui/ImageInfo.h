#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <string>

enum ImageClass
{
    CLASS_SIMPLE,
    CLASS_MEDIUM,
};

struct ImageFilterConfig
{
    int medianKernelSize;
    int gaussianKernelSize;
    double gaussianKernelSigma;
    int openingKernelSize;
    int closingKernelSize;
};

struct ImageInfo
{
    std::string pathToImage;
    ImageClass imgClass;
    ImageFilterConfig filterConfig;
    double groupingThreshold;
    double noiseMaxSize;
    int binaryThreshold = 60;
    bool greyWorld = true;
    bool contrastStretch = true;
};

#endif // IMAGEINFO_H
