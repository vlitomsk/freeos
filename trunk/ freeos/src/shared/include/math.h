#ifndef MATH_H
#define MATH_H

extern double acos(double a);
extern double asin(double a);
extern double atan(double a);
extern double ceil(double a);
extern double cos(double a);
extern double cosh(double a);
extern double exp(double x);
extern double fabs(double a);
extern double floor(double a);
extern double fmod(double a, double b);
extern double frexp(double x, int* exp);
extern double ldexp(double x, int exp);
extern double log10(double a);
extern double log(double a);
extern double modf(double a, double *iptr);
extern double pow(double a, double b);
extern double sin(double a);
extern double sinh(double a);
extern double sqrt(double x);
extern double tan(double a);
extern double tanh(double a);

// our own functions

extern double factorial(unsigned int a);  // faktorial
extern double agm(double x, double y, double eps); // arifmetichesko-geometricheskoe srednee. eps - zadannaya tochnost'
extern double log2(float x); // logarifm po osnovaniu 2

#endif
