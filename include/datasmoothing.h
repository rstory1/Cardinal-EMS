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

    float maxValueAge;  // If the value doesn't meet the required offset to trigger a display update for this many seconds, snap the display value to the current value
    float window; // How different does a value have to be to trigger a display update
    float filterTimeConstant; // Example Tau values:tau = .25 for parameters that change quickly tau = 1 for parameters that change somewhat quickly or have some noise tau = 2.0 for slow changing or noisy values like fuel qty
};

#endif // DATASMOOTHING_H
