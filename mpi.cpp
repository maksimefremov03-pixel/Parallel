//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//#include <cmath>
//#include <mpi.h>
//
//using namespace std;
//
//double PI = acos(-1);
//
//void fill(double* x_real, double* x_imag, double* X_real, double* X_imag, const int N)
//{
//    for (int n = 0; n < N; n++)
//    {
//        x_real[n] = rand() % 100;
//        x_imag[n] = rand() % 100;
//    }
//
//    memset(X_real, 0, N * sizeof(double));
//    memset(X_imag, 0, N * sizeof(double));
//}
//
//void dpf(double* x_real, double* x_imag, double* local_X_real, double* local_X_imag, int start_idx, int local_k_count, int N)
//{
//    for (int local_k = 0; local_k < local_k_count; local_k++)
//    {
//        int k = start_idx + local_k;
//        for (int n = 0; n < N; n++)
//        {
//            double angle = (2 * PI * k * n) / N;
//            local_X_real[local_k] += x_real[n] * cos(angle) + x_imag[n] * sin(angle);
//            local_X_imag[local_k] += -x_real[n] * sin(angle) + x_imag[n] * cos(angle);
//        }
//    }
//}
//
//double mpi(int rank, int size, int N)
//{
//    // Количество итераций для каждого процесса
//    int local_n = N / size;
//    int remainder = N % size;
//
//    // Начальный и конечный индексы для каждого процесса
//    int start_idx = rank * local_n + min(rank, remainder);
//    int end_idx = start_idx + local_n + (rank < remainder ? 1 : 0);
//    int local_k_count = end_idx - start_idx;
//
//    double* x_real = new double[N];
//    double* x_imag = new double[N];
//    double* local_X_real = new double[local_k_count]();
//    double* local_X_imag = new double[local_k_count]();
//    double* X_real = nullptr;
//    double* X_imag = nullptr;
//
//    if (rank == 0)
//    {
//        X_real = new double[N];
//        X_imag = new double[N];
//        fill(x_real, x_imag, X_real, X_imag, N);
//    }
//
//    double starttime = MPI_Wtime();
//
//    // Рассылаем x_real и x_imag всем процессам
//    MPI_Bcast(x_real, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    MPI_Bcast(x_imag, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//
//    dpf(x_real, x_imag, local_X_real, local_X_imag, start_idx, local_k_count, N);
//
//    int* recvcounts = new int[size]; // количество принимаемых элементов
//    int* displs = new int[size]; // массив смещений
//
//    int offset = 0;
//    for (int i = 0; i < size; i++) 
//    {
//        recvcounts[i] = N / size + (i < remainder ? 1 : 0);
//        displs[i] = offset;
//        offset += recvcounts[i];
//    }
//
//    // Собираем результаты в корневом процессе
//    if (rank == 0) 
//    {
//        MPI_Gatherv(local_X_real, local_k_count, MPI_DOUBLE,
//            X_real, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gatherv(local_X_imag, local_k_count, MPI_DOUBLE,
//            X_imag, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    }
//    else 
//    {
//        MPI_Gatherv(local_X_real, local_k_count, MPI_DOUBLE,
//            nullptr, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//        MPI_Gatherv(local_X_imag, local_k_count, MPI_DOUBLE,
//            nullptr, recvcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    }
//
//    double endtime = MPI_Wtime();
//
//    delete[] x_real;
//    delete[] x_imag;
//    delete[] local_X_real;
//    delete[] local_X_imag;
//    delete[] recvcounts;
//    delete[] displs;
//
//    if (rank == 0) 
//    {
//        delete[] X_real;
//        delete[] X_imag;
//    }
//
//    return endtime - starttime;
//}
//
//int main(int argc, char** argv)
//{
//    MPI_Init(&argc, &argv);
//
//    int rank, size;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    const int RUNS = 30;
//
//    if (rank == 0)
//    {
//        cout << "Number of MPI processes: " << size << endl;
//        cout << "Averaging over " << RUNS << " runs" << endl;
//        cout << "----------------------------------------" << endl;
//    }
//
//    double this_time, max_time, total_time;
//    int N = 10;
//    while (N <= 10000)
//    {
//        total_time = 0.0;
//
//        for (int run = 0; run < RUNS; run++)
//        {
//            srand(time(NULL) + rank + run * 100);
//
//            this_time = mpi(rank, size, N);
//
//            MPI_Reduce(&this_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
//
//            if (rank == 0) {
//                total_time += max_time;
//            }
//        }
//
//        if (rank == 0) 
//        {
//            printf("N = %5d: avg = %8.6lf seconds\n", N, total_time / RUNS);
//        }
//
//        N *= 10;
//    }
//
//    MPI_Finalize();
//    return 0;
//}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

void fill_matrix(int* matrix, int N)
{
    for (int i = 0; i < N * N; i++)
        matrix[i] = rand() % 10;
}

void print_matrix(int* matrix, int N)
{
    cout << "Matrix:" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << matrix[i * N + j] << " ";
        cout << endl;
    }
}

int find_zero_in_rows(int* data, int start_row, int local_rows, int N)
{
    for (int i = 0; i < local_rows; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (data[i * N + j] == 0)
                return start_row + i;
        }
    }
    return N+1;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 10;
    int* matrix = nullptr;
    int* sendcounts = nullptr;
    int* displs = nullptr;

    // Распределение строк
    int rows_per_proc = N / size;
    int rem = N % size;

    int start_row = rank * rows_per_proc + min(rank, rem);
    int local_rows = rows_per_proc + (rank < rem ? 1 : 0);

    // Процесс 0 готовит данные для рассылки
    if (rank == 0)
    {
        matrix = new int[N * N];
        srand(time(NULL));
        fill_matrix(matrix, N);
        print_matrix(matrix, N);

        // Вычисляем sendcounts и displs
        sendcounts = new int[size];
        displs = new int[size];

        int offset = 0;
        for (int i = 0; i < size; i++)
        {
            int rows = rows_per_proc + (i < rem ? 1 : 0);
            sendcounts[i] = rows * N;
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    int* local_data = new int[local_rows * N];

    // Рассылка данных (исправленный вариант)
    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT,
        local_data, local_rows * N, MPI_INT,
        0, MPI_COMM_WORLD);

    // Поиск нуля
    int local_result = find_zero_in_rows(local_data, start_row, local_rows, N);

    // Сбор результата
    int global_result;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        if (global_result != N+1)
            cout << "First row with zero: " << global_result << endl;
        else
            cout << "No zero found" << endl;

        delete[] matrix;
        delete[] sendcounts;
        delete[] displs;
    }

    delete[] local_data;
    MPI_Finalize();
    return 0;
}