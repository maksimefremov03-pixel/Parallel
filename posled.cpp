#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <windows.h>
#include <omp.h>

using namespace std;
using namespace std::chrono;

double PI = acos(-1);

void fill(double* x_real, double* x_imag, double* X_real, double* X_imag, const int N)
{
	for (int n = 0; n < N; n++)
	{
		x_real[n] = rand() % 100;
		x_imag[n] = rand() % 100;
	}

	memset(X_real, 0, N * sizeof(double));
	memset(X_imag, 0, N * sizeof(double));
}

void dpf(const double* x_real, const double* x_imag, double* X_real, double* X_imag, const int N)
{
	double angle;
	for (int k = 0; k < N; k++)
	{
		for (int n = 0; n < N; n++)
		{
			angle = (2 * PI * k * n) / N;
			X_real[k] += x_real[n] * cos(angle) + x_imag[n] * sin(angle);
			X_imag[k] += -x_real[n] * sin(angle) + x_imag[n] * cos(angle);
		}
	}
}

void sequence(int N)
{
	const int RUNS = 30;
	double total_time;

	while (N != 100000)
	{
		total_time = 0.0;

		for (int run = 0; run < RUNS; run++)
		{
			double* x_real = new double[N];
			double* x_imag = new double[N];
			double* X_real = new double[N];
			double* X_imag = new double[N];

			fill(x_real, x_imag, X_real, X_imag, N);

			auto start = high_resolution_clock::now();

			dpf(x_real, x_imag, X_real, X_imag, N);

			auto end = high_resolution_clock::now();

			duration<double> diff = end - start;
			total_time += diff.count();

			delete[] x_real;
			delete[] x_imag;
			delete[] X_real;
			delete[] X_imag;
		}

		printf("N = %5d: avg = %8.6lf seconds\n", N, total_time / RUNS);

		N *= 10;
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	srand(time(NULL));

	int N = 10;

	sequence(N);

	return 0;
}