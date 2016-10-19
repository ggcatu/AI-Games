#include <math.h>
#include <stdio.h>


double factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}


int main(){
	double a = pow(23,18);
	double b = log(factorial(24))/log(23);
	printf("%f \n %f \n", a, b);

	return 0;
}