#ifndef DATASMOOTHING_H
#define DATASMOOTHING_H


class DataSmoothing
{
public:
    explicit DataSmoothing();
    void setSampleSize(int sampleSize);
    double dsp_ema_double(double in);

private:
    int sampleSize;
    double average = 0;
    double alpha;

    void calcAlpha();
};

#endif // DATASMOOTHING_H
