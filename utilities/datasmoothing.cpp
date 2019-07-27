#include "datasmoothing.h"

DataSmoothing::DataSmoothing()
{

}

void DataSmoothing::setSampleSize(int sampleSize) {
    this->sampleSize = sampleSize;
    calcAlpha();
}

double DataSmoothing::dsp_ema_double(double in) {

    if ( alpha >= 1.0 ) {
        return average = in;
    }

    average = in * alpha + average * (1 - alpha);

    return average;

}

void DataSmoothing::calcAlpha() {
    alpha = 2.0 / (sampleSize + 1.0);
}
