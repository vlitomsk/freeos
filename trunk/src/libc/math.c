#include <libc/math.h>

#define E 	2.7182818284590452
#define PI 	3.1415926535897932

double ceil(double a) {	
	return (((long long)a) * 1.0 + 1);
}

double cos(double a) {
	double result;	
    asm ("fcos" : "=t" (result) : "0" (a)); // turututu :) nikto ne zametit etoi strochki:)
    return result;
}

double exp(double x) {
	return pow(E, x);
}

double fabs(double a) {
	return a < 0 ? -a : a;
}

double floor(double a) {
	return (((long long)a) * 1.0);
}

double fmod(double a, double b) {
	return (a - floor(a/b)*b);
}

double ldexp(double x, int exp) {	
	return exp == 0 ? x : x * pow(2, exp);
}

#define LN_2 	0.693147181
#define LG_E 	0.434294482

// ln x = ln (osnovanie) * 

double log(double a) { // ln
	return log2((float)(a)) * LN_2;
}

double log10(double a) { // lg
	return log(a) * LG_E;
}

double pow(double a, double b) {
	return exp(b * log(a));
}

double sin(double a) {
	double result;	
    asm ("fsin" : "=t" (result) : "0" (a));
    return result;
}

double sqrt(double x) {
   double y, z, tempf;
   unsigned long *tfptr = ((unsigned long *)&tempf) + 1;

   tempf = y;
   *tfptr = (0xbfcdd90a - *tfptr)>>1; /* estimate of 1/sqrt(y) */
   x =  tempf;
   z =  y*0.5;                        /* hoist out the “/2”    */
   int i;
   for (i = 0; i < 6; ++i)
	   x = (1.5*x) - (x*x)*(x*z);         /* iteration formula     */
   
   return x*y;
}

double tan(double a) {
	return (sin(a)/cos(a));
}

#define ATAN1 0.785398163

double acos(double a) {
	return atan(-a / (-a * a + 1) * (-a * a + 1)) + ATAN1 * 2;
}

double asin(double a) {
	return atan(a / (-a * a + 1) * (-a * a + 1));	
}

double atan(double a) {	
	float result;
	float st0;
	st0 = 1;
	// nemnogo asma))))
	asm("fld %2\n"
		"fld %1\n"
		"fpatan\n"
		"fstp %0\n"
		: "=g" (result)
		: "g" (st0), "g" ((float)a));
		
	return result;
}

double cosh(double a) {
	return (exp(a) + exp(-a)) / 2;
}

double frexp(double x, int* exp) {
	if (x == 0) {
		exp[0] = 0;
		return 0;
	}

	double power = 1;
	int i;
	for (i = 0; power * 0.5 <= fabs(x); ++i, power = pow(2, i));
	
	*exp = i;
	return x / pow(2, *exp);
}

double modf(double a, double *iptr) {
	*iptr = floor(a);
	return a - *iptr;
}

double sinh(double a) {
	return (exp(a) - exp(-a)) / 2;	
}

double tanh(double a) {
	return sinh(a) / cosh(a);
}

// our ow n functions
double factorial(unsigned int a) {
        double result = a;
        for (; a > 1; --a, result *= a);
        return result;
}

double agm(double x, double y, double eps) {
	while (fabs(x - y) > eps) {
		double temp = x;

		x = (x+y) / 2;
		y = sqrt(temp * y);
	}

	return x;
}

double log2(float x) {
	float result;
	float st1;
	st1 = 1;
	// nemnogo asma))))
	asm("fld %2\n"
        "fld %1\n"
        "fyl2x\n"
        "fstp %0\n"
        : "=g" (result)
        : "g" (x), "g" (st1));
	return result;
}

