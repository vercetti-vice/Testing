#include <iostream>
#include <omp.h>
#include <cstdio>
#include <limits.h>
#include <cmath>


// Первая задача.
void task1()
{
#pragma omp parallel num_threads(8)
    {
        printf("Hello World! %d\n", omp_get_thread_num());
    }
}

// Вторая задача.
void task2()
{
    omp_set_num_threads(3);
#pragma omp parallel if (omp_get_max_threads() > 1)
    {
        printf("1.Thread ID = %d  Number of threads = %d \n", omp_get_thread_num(), omp_get_num_threads());
    }

    omp_set_num_threads(1);
#pragma omp parallel if (omp_get_max_threads() > 1)
    {
        printf("2.Thread ID = %d  Number of threads = %d \n", omp_get_thread_num(), omp_get_num_threads());
    }
}

// Третья задача.
void task3()
{
    int a = 0;
    int b = 0;

    printf("До входа в первую параллельную область: a = %d, b = %d \n", a, b);
#pragma omp parallel private(a) firstprivate(b) num_threads(2)
    {
        a += omp_get_thread_num();
        b += omp_get_thread_num();
        printf("Внутри первой области: a = %d, b = %d \n", a, b);
    }
    printf("После входа в первую параллельную область: a = %d, b = %d \n", a, b);

    printf("\n");

    printf("До входа во вторую параллельную область: a = %d, b = %d \n", a, b);
#pragma omp parallel shared(a) private(b) num_threads(4)
    {
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();
        printf("Внутри второй области: a = %d, b = %d \n", a, b);
    }
    printf("После входа во вторую параллельную область: a = %d, b = %d \n", a, b);
}

// Четвёртая задача.
void task4()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int max = 0;
    int min = 0;

    auto min_element = [](int* arr, int count)
    {
        int min = INT_MAX;
        for(int i = 0; i < count; ++i)
        {
            if(min > arr[i]) min = arr[i];
        }
        return min;
    };
    auto max_element = [](int* arr, int count)
    {
        int max = INT_MIN;
        for(int i = 0; i < count; ++i)
        {
            if(max < arr[i]) max = arr[i];
        }
        return max;
    };

#pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();
        if(thread_id == 1)
        {
            max = max_element(b, 10);
        } else
        {
            min = min_element(a, 10);
        }
    }
    printf("Max = %d , Min= %d \n", max, min);
}

// Пятая задача.
void task5()
{
    int d[6][8];
    int sum;

    srand(time(NULL));

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            d[i][j] = -150 + rand() % ((150 + 1) + 150);
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << d[i][j] << " ";
        }
        std::cout << std::endl;
    }

    printf("\n");

#pragma omp parallel sections
    {
#pragma omp section
        {
            sum = 0;
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    sum += d[i][j];
                }
            }

            double avg = sum/48;
            printf("Section 1: Average = %f Thread ID = %d \n", avg, omp_get_thread_num());
        }
#pragma omp section
        {
            int min = INT_MAX;
            int max = INT_MIN;

            for(int i = 0; i < 6; ++i)
            {
                for (int j = 0; j < 8; ++j)
                {
                    if (min > d[i][j]) min = d[i][j];
                    if (max < d[i][j]) max = d[i][j];
                }
            }

            printf("Section 2: Min = %d, Max = %d, Thread ID = %d \n", min, max, omp_get_thread_num());
        }
#pragma omp section
        {
            int count = 0;
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if(d[i][j] % 3 == 0) count++;
                }
            }

            printf("Section 3: Count = %d, Thread ID = %d \n", count, omp_get_thread_num());
        }
    }
}

// Шестая задача.
void task6() {
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 12};
    int b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int sum_a = 0, sum_b = 0;

#pragma omp parallel for reduction(+: sum_a, sum_b)
    for (int i = 0; i < 10; i++) {
        sum_a += a[i];
        sum_b += b[i];
    }

    double avg_a = (double)sum_a / 10;
    double avg_b = (double)sum_b / 10;

    if (avg_a > avg_b) {
        printf("Среднее значение массива a = %f больше, чем среднее значение массива b = %f", avg_a, avg_b);
    } else if (avg_a == avg_b) {
        printf("Среднее значение массива a = %f равно среднему значению массива b = %f", avg_a, avg_b);
    } else {
        printf("Среднее значение массива b = %f больше, чем среднее значение массива a = %f", avg_b, avg_a);
    }
}

// Седьмая задача.
void task7() {
    int a[12] = {};
    int b[12] = {};
    int c[12] = {};

#pragma omp parallel for schedule(static, 2) num_threads(3)
    for (int i = 0; i < 12; i++)
    {
        a[i] = i;
        b[i] = i;

        printf("Number of threads = %d, Thread ID = %d, a[%d} = %d, b[%d] = %d \n", omp_get_num_threads(), omp_get_thread_num(), i, a[i], i, b[i]);
    }

    printf("\n");

#pragma omp parallel for schedule(dynamic, 2) num_threads(4)
    for(int i = 0; i < 12; i++)
    {
        c[i] = a[i] + b[i];

        printf("Number of threads = %d, Thread ID = %d, c[%d} = %d \n", omp_get_num_threads(), omp_get_thread_num(), i, c[i]);
    }

}

// Восьмая задача.
void task8() {
    int rows = 20000;
    int columns = 20000;
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new int[columns];


    int *vector = new int[columns];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = i + j;
        }
    }

    for (int i = 0; i < columns; i++) {
        vector[i] = i;
    }

    int *out = new int[columns];

    double start = omp_get_wtime();
#pragma omp parallel for num_threads(12)

    for (int i = 0; i < rows; i++) {
        out[i] = 0;
        for (int j = 0; j < columns; j++)
            out[i] += matrix[i][j] * vector[j];
    }

    double end = omp_get_wtime();

    printf("Parallel: start = %.16g\nend = %.16g\ndiff = %.16g\n",
           start, end, end - start);

    printf("\n");

    start = omp_get_wtime();
    for (int i = 0; i < rows; i++) {
        out[i] = 0;
        for (int j = 0; j < columns; j++)
            out[i] += matrix[i][j] * vector[j];
    }
    end = omp_get_wtime();

    printf("Serial: start = %.16g\nend = %.16g\ndiff = %.16g\n",
           start, end, end - start);

    delete[] out;
    delete[] vector;
    for (int count = 0; count < rows; count++)
        delete[] *(matrix + count);
}

// Девятая задача.
void task9()
{
    int d[6][8];
    int i, j, sum;

    srand(time(NULL));

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 8; j++)
        {
            d[i][j] = -150 + rand() % ((150 + 1) + 150);
        }
    }

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 8; j++) {
            std::cout << d[i][j] << " ";
        }
        std::cout << std::endl;
    }

    int min = d[0][0];
    int max = d[0][0];

#pragma omp parallel for num_threads(8)
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 8; ++j) {
#pragma omp critical
                {
                    if (min > d[i][j]) min = d[i][j];
                    if (max < d[i][j]) max = d[i][j];
                }
            }
        }

    printf("\nMin = %d, Max = %d \n", min, max);
}

// Десятая задача.
void task10()
{
    int a[30];
    int i;
    int count = 0;

    srand(time(NULL));

    for (i = 0; i < 30; i++) {
        a[i] = -100 + rand() % ((100 + 1) + 100);
    }

    for (i = 0; i < 30; i++) {
        std::cout << a[i] << " ";
    }

#pragma omp parallel for num_threads(8)
    for(int i = 0; i < 30; i++) {
        if (a[i] % 9 == 0) {
#pragma omp atomic
            count++;
        }
    }

    printf("\nCount: %d", count);
}

// Одиннадцатая задача.
void task11()
{
    int a[30];

    srand(time(NULL));

    for (int i = 0; i < 30; i++) {
        a[i] = -100 + rand() % ((100 + 1) + 100);
    }

    for (int i = 0; i < 30; i++) {
        std::cout << a[i] << " ";
    }

    int max = INT_MIN;
#pragma omp parallel for
    for(int i = 0; i < 30; i++)
    {
#pragma omp critical
        {
            if (max < a[i] && a[i] % 7 == 0) max = a[i];
        }
    }

    if(max != INT_MIN) {
        printf("\nMax: %d", max);
    }
    else{
        printf("Такого числа нет");
    }
}

// Двенадцатая задача. Доделать
void task12()
{
#pragma omp parallel num_threads(8)
    {
        printf("Hello World! %d\n", omp_get_thread_num());
    }
}

// Триннадцатая задача
void task13()
{
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};

    int x = 0;

    int array_count = sizeof(a) / sizeof(*a);

#pragma omp parallel for reduction(+: x)
    for(int i = 0; i < array_count; i++)
    {
        x += a[i]*(pow(2,array_count - i - 1));
    }

    printf("%d", x);
}

// Четырнадцатая задача.
void task14()
{
    int sum = 0;

#pragma omp parallel for reduction(+: sum) num_threads(16)
        for (int i = 1; i <= 210; i++) {
            int iteration = (i * 2) - 1;
            sum += iteration;
        }

    printf("%d", sum);
}

// Пятнадцатая задача.
void task15() {
    int first = 0, last = 0;
    std::cout << "Введите первое число: ";
    std::cin >> first;
    std::cout << "Введите второе число: ";
    std::cin >> last;

    printf("\n");

#pragma omp parallel for firstprivate(first, last) num_threads(8)
    for (int i = first; i <= last; i++)
    {
        bool prime = true;
        for (int j = 2; j*j <= i; j++)
        {
            if (i % j == 0)
            {
                prime = false;
                break;
            }
        }
        if(prime)
        {
            printf("%d ", i);
        }
    }
}


int main() {
    task6();
    return 0;
}

