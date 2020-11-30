/*
* Вариант 24.
*/
#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<omp.h>

using namespace std;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"

void findWinners(vector<int> vect, int &res, int &index) {

    if (vect.size() == 1) {
        res = vect[0];
    } else if (vect.size() == 2) {
        res = (vect[0] > vect[1]) ? vect[0] : vect[1];
        index = (res == vect[1]) ? index + 1 : index;
    } else {
        int k = vect.size() / 2;
        vector<int> leftpart(k);
        for (size_t i = 0; i < leftpart.size(); i++) {
            leftpart[i] = vect[i];
        }

        vector<int> rightpart(vect.size() - k);
        for (size_t i = 0; i < rightpart.size(); i++) {
            rightpart[i] = vect[k + i];
        }

        int a, b;
        int ind_a = index, ind_b = index + k;
# pragma omp task shared(leftpart, a, ind_a)
        {
            findWinners(leftpart, a, ind_a); // Левая половина.

#pragma omp critical
            {
                cout << "Thread: " << omp_get_thread_num() << " From : ";
                for (int i = 0; i < leftpart.size(); ++i) {
                    cout << leftpart[i] << " ";
                }
                cout << "\nResult: " << a << endl;

            }
        }

# pragma omp task shared(rightpart, b, ind_b)
        {
            findWinners(rightpart, b, ind_b); // Правая половина.

#pragma omp critical
            {
                cout << "Thread: " << omp_get_thread_num() << " From : ";
                for (int i = 0; i < rightpart.size(); ++i) {
                    cout << rightpart[i] << " ";
                }
                cout << "\nResult: " << b << endl;

            }
        }
#pragma omp taskwait
        // Здесь можно вывести кто из выигравших выиграл
        res = max(a, b);
        index = (res == a) ? ind_a : ind_b;
    }
}

#pragma clang diagnostic pop
#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-msc51-cpp"

int main(int argc, char *argv[]) {

    int n;
    cout << "Введите число участников сореванования " << endl;
    cin >> n;

    if (n < 1)
    {
        cout << "Число участников не может быть неположительным" << endl;
        return 1;
    }

    srand(time(0));

    vector<int> monks(n);
    for (int i = 0; i < n; ++i) {
        monks[i] = rand() % (2*n);
    }

    cout << "\nУровень ци участников соревнования  :" << endl;
    for (int i = 0; i < n; ++i) {
        cout << monks[i] << " ";
    }
    cout << "\n" << endl;

    int res, ind = 0;

# pragma omp parallel shared(monks, res, ind)
    {
# pragma omp single nowait
        {
            findWinners(monks, res, ind);
        }
    }

    cout << "\nНомер победителя: " << ind << " Уровень ци: " << res << endl;

    return 0;
}

#pragma clang diagnostic pop