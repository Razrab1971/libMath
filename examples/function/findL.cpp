
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
   
    const value_t lhs = 0,
          rhs = 1;

    auto value = findNullL_func(
        lhs, //Левая точка отрезка
        rhs, //Правая точка отрезка
        [](value_t x){ return std::atan(x*x) - (x + 0.5) * (x - 0.1);}, // Функция
        eps, //Точность
        maxCountIteration //Количество итераций
    );

    std::cout << "Корень на интервале "
    << '(' << lhs << ';' << rhs << ')'
    << " от atan(x*x) - (x + 0.5) * (x - 0.1): "
    << (value.has_value() ? std::to_string(*value) : "std::nullopt") 
    << std::endl;

    std::vector<value_t> vec{
        -2, -1, 1, 4, 6, 10, 11 // Интервалы
    };

    auto res1 = findsNullLsort_func(
            vec, // Интервалы
            [](value_t x){ return x*x*x - 4.0*x*x - 1.6*x + 3.8;}, // Функция
            eps, //Точность
            maxCountIteration //Количество итераций на один интервал
            );

    std::cout << "Поиск на интервалах: " << std::endl;
    write_console(res1, std::cout);

    return 0;
}
