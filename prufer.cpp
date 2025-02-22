#include <iostream>
#include <vector>
#include <random>
#include <chrono>

template <class T>
using List = std::vector<T>;

// Функция для генерации последовательности Пруфера
List<int> prufer_gen(int n) {
    List<int> prufer_sequence(n - 2);  // Последовательность Пруфера будет иметь длину n-2

    // Используем текущее время как начальное значение для генератора
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dist(1, n);

    for (int i = 0; i < n - 2; ++i) {
        prufer_sequence[i] = dist(gen);
    }

    return prufer_sequence;
}

int main() {
    int n = 10;  // Задайте n
    List<int> sequence = prufer_gen(n);

    std::cout << "Prufer with n = " << n << " edges: ";
    for (int num : sequence) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}