#include "../../function/function.hpp"

#include <string>
#include <random> 

#include <iostream>
#include <iomanip>


typedef double value_t;
typedef sqrtValue<value_t> sqrtValue_;
typedef std::vector<sqrtValue_> VectorSrt;

void write_console(const VectorSrt& vec, std::ostream& ost){
    for(auto& x: vec){
        try{
            ost << std::to_string(*x) << std::endl;
        }catch(const ValueNull&){
            ost << "Корень имеет значение: std::nullopt" << std::endl;
        }catch(const ValueNotRange&){
            ost << "Корень вне диапазона: " << x.get_value_not_optional_valid() << std::endl;
        }catch(const ValueMissing&){
            ost << "Корень потерялся " << "nullopt" <<  " диапазон: " << "(" << x.get_range().first << "," << x.get_range().second << ")" << std::endl;
        }catch(...){
            ost << "RER" << std::endl;
        }
    }
}

int main(){
    
    const value_t eps = 0.001; // Точность
    const unsigned maxCountIteration = 2000; // Максимальное количество итераций


    auto value = findNullN_func(
        0.2, //Начальная точка старта
        [](value_t x){ return std::atan(x*x) - (x + 0.5) * (x - 0.1);}, // Функция
        eps, //Точность
        maxCountIteration //Количество итераций
    );

    std::cout << "Корень через приближённую производную: std::atan(x*x) - (x + 0.5) * (x - 0.1): "
    << (value.has_value() ? std::to_string(*value) : "std::nullopt") 
    << std::endl;

    value = findNullN_func(
        0.2, //Начальная точка старта
        [](value_t x){ return std::atan(x*x) - (x + 0.5) * (x - 0.1);}, // Функция
        [](value_t x){ return 2*x/(1 + x*x*x*x) - (2 * x + 0.4);}, // Производная функции
        eps, //Точность
        maxCountIteration //Количество итераций
    );

    std::cout << "Корень через производную: std::atan(x*x) - (x + 0.5) * (x - 0.1): "
    << (value.has_value() ? std::to_string(*value) : "std::nullopt") 
    << std::endl
    << std::endl;



    std::vector<value_t> vec{
        -2, -1, 1, 4, 6, 10, 11 // Интервалы
    };


    constexpr bool flag = true; // Флаг для развилки ветки:
                      // true  - с приближёнными производными,
                      // false - с производными.


    VectorSrt res1, res2;
    if constexpr (flag){
        res1 = findsNullN_func(
                vec, // Интервалы
                [](value_t x){ return x*x*x - 4.0*x*x - 1.6*x + 3.8;}, // Функция
                eps, //Точность
                maxCountIteration //Количество итераций на один интервал
        );
        
        std::shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device{}())); // Перетасовывает вектор
        res2 = findsNullNsort_func(
                vec, // Интервалы
                [](value_t x){ return x*x*x - 4.0*x*x - 1.6*x + 3.8;}, // Функция
                eps, //Точность
                maxCountIteration //Количество итераций на один интервал
        );
    }else{
        res1 = findsNullN_func(
                vec, // Интервалы
                [](value_t x){ return x*x*x - 4.0*x*x - 1.6*x + 3.8;}, // Функция
                [](value_t x){ return 3*x*x - 8.0*x - 1.6;}, // Производная функции
                eps, //Точность
                maxCountIteration //Количество итераций на один интервал
        );
        
        std::shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device{}())); // Перетасовывает вектор
        res2 = findsNullNsort_func(
                vec, // Интервалы
                [](value_t x){ return x*x*x - 4.0*x*x - 1.6*x + 3.8;}, // Функция
                [](value_t x){ return 3*x*x - 8.0*x - 1.6;}, // Производная функции
                eps, //Точность
                maxCountIteration //Количество итераций на один интервал
        );
    }

    std::cout << "Поиск на интервалах: " << std::endl;
    write_console(res1, std::cout);
    std::cout << std::endl;

    std::cout << "Поиск на интервалах с sort: " << std::endl;
    write_console(res2, std::cout);

    return 0;
}
