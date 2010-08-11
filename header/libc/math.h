#ifndef MATH_H
#define MATH_H

extern double acos(double);
extern double asin(double);
extern double atan(double);
extern double ceil(double);
extern double cos(double);
extern double cosh(double);
extern double exp(double);
extern double fabs(double);
extern double floor(double);
extern double fmod(double, double);
extern double frexp(double, int*);
extern double ldexp(double, int);
extern double log10(double);
extern double log(double);
extern double modf(double, double*);
extern double pow(double, double);
extern double sin(double);
extern double sinh(double);
extern double sqrt(double);
extern double tan(double);
extern double tanh(double);

// our own functions

extern double factorial(unsigned int);  // faktorial
extern double agm(double, double, double); // arifmetichesko-geometricheskoe srednee. eps - zadannaya tochnost'
extern double log2(float); // binary log

#endif
