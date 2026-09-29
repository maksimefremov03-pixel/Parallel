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
#pragma omp parallel for collapse(1)
	for (int k = 0; k < N; k++)
	{
		double local_real = 0;
		double local_imag = 0;

		for (int n = 0; n < N; n++)
		{
			double angle = (2 * PI * k * n) / N;
			local_real += x_real[n] * cos(angle) + x_imag[n] * sin(angle);
			local_imag += -x_real[n] * sin(angle) + x_imag[n] * cos(angle);
		}

#pragma omp critical
		{
			X_real[k] += local_real;
			X_imag[k] += local_imag;
		}
	}
}

void open_mp(const int threads)
{
	int N;
	for (int i = 1; i <= threads; i++)
	{
		omp_set_num_threads(i);
		cout << "Число потоков: " << i << endl;
		N = 10;
		while (N != 100000)
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
			printf("Время выполнения ДПФ (OpenMP) для N = %d: %lf секунд\n", N, diff.count());

			delete[] x_real;
			delete[] x_imag;
			delete[] X_real;
			delete[] X_imag;
			N *= 10;
		}
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	srand(time(NULL));

	int threads = omp_get_max_threads();

	cout << "Максимально доступно потоков: " << omp_get_max_threads() << endl;

	open_mp(threads);
	
	return 0;
}