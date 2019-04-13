#include "arduinoFFT.h"
 
#define SAMPLES 8             //Must be a power of 2
#define SAMPLING_FREQUENCY 768 //Hz, must be less than 10000 due to ADC
 
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
    Serial.begin(115200);
 
    sampling_period_us = round(10000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop() {
   
    /*SAMPLING*/
    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();    //Overflows after around 70 minutes!
     
        vReal[i] = analogRead(A0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 
    /*FFT*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
     
    for(int i=0; i<1; i++)
    {
        delay(5000);
        
        Serial.print("Frequency Value:");
        Serial.println(peak);
        //Serial.print((peak*1.0*SAMPLING_FREQUENCY) / SAMPLES);
        Serial.println(" ");
        /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
        //Serial.print("Frequency Value:");
        //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
        //Serial.println(" ");
        //Serial.println(vImag[i], 1);    //View only this line in serial plotter to visualize the bins
    }
 
    delay(5000);  //Repeat the process every second OR:
    //while(1);       //Run code once
}
