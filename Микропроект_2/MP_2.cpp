// Вариант 2. 
// Задача о Винни-Пухе и правильных пчелах.
#include <iostream>
#include <pthread>
#include <semaphore.h>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <chrono>

static int current_h = 0; // Сколько глотков меда в горшке на данный момент.
static int max_h = 0; // Размер горшка (в глотках меда).
static int iterations = 0; //Число полных циклов.

// В горшок еще можно добавить мед.
sem_t emp;
// Гошок полон.
sem_t full;

pthread_mutex_t mutexD; // Для пчел.
pthread_mutex_t mutexF; // Для медедя.

// Функция пчелы - добавляет 1 глоток меда в горшок.
void* fill(void* param) {

	// Номер пчелы.
	int bNum = *((int*)param);

	while (iterations) {
		pthread_mutex_lock(&mutexD);

		if (!(iterations)) {
			pthread_mutex_unlock(&mutexD);
			break;
		}

		// Ждем ситуации, когда в горшок физически можно добавить мед.
		sem_wait(&emp);

		current_h++;
		printf("time = %u beer %d add 1 to the pot, now it is %d\n",
			clock(), bNum, current_h);

		// Пчела, последняя добавившая мед будит медведя.
		// В данном случае, она устанавливает соответствующее значение семафора.
		if (current_h == max_h)
			sem_post(&full);
		else
			// Иначе мед еще можно добавить.
			sem_post(&emp);


		std::this_thread::sleep_for(std::chrono::seconds(1));
		pthread_mutex_unlock(&mutexD);

	}

	return nullptr;

}

// Функция медведя - опустошает горшок.
void* empty(void* param) {

	while (1) {
		pthread_mutex_lock(&mutexF);

		if (!(iterations)) {
			pthread_mutex_unlock(&mutexF);
			break;
		}
		// Ждем пока горшок не станет полон.
		sem_wait(&full);
		current_h = 0;
		iterations--;
		// Уведомляем, что горшок пуст.
		sem_post(&emp);
		printf("time = %u bear emptied the pot\n", clock());

		std::this_thread::sleep_for(std::chrono::seconds(3));
		pthread_mutex_unlock(&mutexF);

	}
	return nullptr;
}

using namespace std;
int main()
{
	// Вод данных.

	int n; // Число пчел.

	cout << "Input number of bees" << endl;
	cin >> n;
	if (n < 1)
	{
		cout << "Number of bees cannot be negative!" << endl;
		return -1;
	}

	cout << "Input capacity of the pot" << endl;
	cin >> max_h;
	if (max_h < 1) {
		cout << "Capacity of the pot cannot be negative!" << endl;
		return -1;
	}

	cout << "Input number of cycles" << endl;
	cin >> iterations;
	if (iterations < 1) {
		cout << "Number of cycles cannot be negative!" << endl;
		return -1;
	}


	pthread_mutex_init(&mutexD, nullptr);
	pthread_mutex_init(&mutexF, nullptr);

	sem_init(&emp, 0, 1);
	sem_init(&full, 0, 0);

	// Создать потоки пчел + основной поток - тоже поток пчелы.
	int* beesNames = new int[n - 1];
	pthread_t* bees = new pthread_t[n - 1];
	for (size_t i = 0; i < n - 1; i++)
	{
		beesNames[i] = i + 2;
		pthread_create(&bees[i], NULL, fill, (void*)(beesNames + i));
	}

	// Создать поток медвведя.
	pthread_t bear;
	pthread_create(&bear, NULL, empty, NULL);

	int mNum = 1;
	fill((void*)&mNum);

	// Освобождение памяти.
	delete[] bees;
	delete[] beesNames;
	sem_destroy(&emp);
	sem_destroy(&full);

	return 0;
}

