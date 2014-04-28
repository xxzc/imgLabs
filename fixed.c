#define FP 4

typedef int fixed;

#define INT2FIXED(a) (a)<<FP
#define FIXED2INT(a) (a)>>FP
#define FIXED2DOUBLE(a) (double)(a)/(1<<FP)

#define FADD(a,b) (a)+(b)
#define FSUB(a,b) (a)-(b)
#define FMUL(a,b) (a)*(b)>>FP
#define FDIV(a,b) ((a)<<FP)/(b)












#include <stdio.h>

int main()
{
	fixed a1 = INT2FIXED(100),a2 = INT2FIXED(3);
	printf("%f",FIXED2DOUBLE(FDIV(a1,a2)));
}