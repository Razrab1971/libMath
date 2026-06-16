#include "../../function/function.hpp"

#include <iostream>
#include <iomanip>

int main(){
    
    auto vec = linspace(
        0.0, // Начальная точка
        1.0, // Конечная  точка
        10 // число разбиений
    );

    std::cout << "Начальные | Изменённые <- значения:" << std::endl;
    transform_this(
            vec, // Контейнер для пробразования
            [](double x){
                double y = x * x; // Операция 

                std::cout 
                << std::right << std::setw(9) << x
                << " | " 
                << std::left << y 
                << std::endl;

                return y; 
            } //Преобразование
    );


    del_dublicate(
        vec, // Контейнер для пробразования
        0.1, // Точность
        [](double x){
            return x * 2;
        } // Последовательное преобразование(необязательный параметр) и потом удаление
    );

    // del_dublicate(
    //     vec, // Контейнер для пробразования
    //     0.1 // Точность 
    // );

    std::cout << "Вектор после преобразования и удаления дубликатов:" << std::endl;
    for(const auto& val: vec){
        std::cout << std::right << std::setw(9) << val << std::endl;
    }

    return 0;
}
