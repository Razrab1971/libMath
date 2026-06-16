#pragma once

#include "../util/sqrtValue.hpp"

#include<algorithm>

#include<variant>
#include<vector>



//Шаблон для разбиение отрезка на равноудалённые точки
//Название с matplotplusplus
template<numberRConcept NumberType>
std::vector<NumberType> linspace(NumberType start, NumberType end, size_t n) {
    std::vector<NumberType> ret(n);
    if(n == 0) return ret;

    NumberType step = (end - start) / n;
    for(size_t i = 0; i < n; ++i)
        ret[i] = start + step * static_cast<NumberType>(i);

    return ret;
}

//Шаблон transform_this
template<typename Container, typename Func>
requires numberRConcept<std::ranges::range_value_t<Container>> &&
funcNumberRC<std::ranges::range_value_t<Container>, Func> &&
std::ranges::input_range<Container> && std::ranges::output_range<Container, std::ranges::range_value_t<Container>>
void transform_this(Container& vec, Func f){
    std::transform(vec.begin(), vec.end(), vec.begin(), f);
}



//Шаблон для нахождения нуля в окрестности точки
//Приближённая производная
template<numberRConcept NumberType, typename Func>
requires funcNumberRC<NumberType, Func>
sqrtValue<NumberType> findNullN_func(
        NumberType pointX,
        Func f,
        const NumberType eps,
        unsigned maxCountIter
){
    unsigned i = 0;
    NumberType pointF  = f(pointX); 
    NumberType pointFp = (f(pointX + eps) - pointF) / eps;

    while(std::abs(pointF) > eps){
        if(--maxCountIter == 0) return std::nullopt;
    
        if(pointFp != 0){
            pointX = pointX - pointF / pointFp;   
        }else{
            pointX = pointX + eps;
        }

        pointF  = f(pointX);
        pointFp = (f(pointX + eps) - pointF) / eps;
    }

    return pointX;
}


//Шаблон для нахождения нуля в окрестности точки
//С производной
template<numberRConcept NumberType, typename Func, typename Funcp>
requires funcNumberRC<NumberType, Func> && funcNumberRC<NumberType, Func>
sqrtValue<NumberType> findNullN_func(
        NumberType pointX,
        Func f,
        Funcp fp,
        const NumberType eps,
        unsigned maxCountIter
){
    NumberType pointF  = f(pointX); 
    NumberType pointFp = fp(pointX);

    while(std::abs(pointF) > eps){
        if(--maxCountIter == 0) return std::nullopt;
    
        if(pointFp != 0){
            pointX = pointX - pointF / pointFp;   
        }else{
            pointX = pointX + eps;
        }

        pointF  = f(pointX);
        pointFp = fp(pointX);
    }
    return pointX;
}


//Шаблон для нахождения нуля на интервалах
//Приближённая производная
template<numberRConcept NumberType, vecPointsC VecPoints, typename Func>
requires funcNumberRC<NumberType, Func>
std::vector<sqrtValue<NumberType>> findsNullN_func(
    const VecPoints& vec,
    Func f,
    const NumberType eps,
    const unsigned maxCountIterIter
){
    std::vector<sqrtValue<NumberType>> ret;
    if(vec.empty()) return ret;
    
    auto iterFirst = vec.begin();
    auto iterSecond = iterFirst;
    
    sqrtValue<NumberType> result;

    while(++iterSecond != vec.end()){
        result = findNullN_func(
                (*iterSecond + *iterFirst) / 2,
                f,
                eps,
                maxCountIterIter
                );

        if(!result.has_value()){
            ret.insert(ret.end(),sqrtValue<NumberType>());
        }else{
            if(*result < *iterFirst || *result > *iterSecond){
                ret.insert(ret.end(),sqrtValue<NumberType>(*result, ValueNotRange()));
            }else{
                ret.insert(ret.end(),
                        { *result }
                );
            }
        }
        ++iterFirst;
    }

    return ret;
}

//Шаблон для нахождения нуля на интервалах
//С производной
template<numberRConcept NumberType, vecPointsC VecPoints, typename Func, typename Funcp>
requires funcNumberRC<NumberType, Func> && funcNumberRC<NumberType, Funcp>
std::vector<sqrtValue<NumberType>> findsNullN_func(
    const VecPoints& vec,
    Func f,
    Funcp fp,
    const NumberType eps,
    const unsigned maxCountIterIter
){
    std::vector<sqrtValue<NumberType>> ret;
    if(vec.empty()) return ret;
    
    auto iterFirst = vec.begin();
    auto iterSecond = iterFirst;
    
    sqrtValue<NumberType> result;

    while(++iterSecond != vec.end()){
        result = findNullN_func(
                (*iterSecond + *iterFirst) / 2,
                f,
                fp,
                eps,
                maxCountIterIter
                );

        if(!result.has_value()){
            ret.insert(ret.end(),sqrtValue<NumberType>());
        }else{
            if(*result < *iterFirst || *result > *iterSecond){
                ret.insert(ret.end(),sqrtValue<NumberType>(*result, ValueNotRange()));
            }else{
                ret.insert(ret.end(),
                        { *result }
                );
            }
        }
        ++iterFirst;
    }

    return ret;
}




//Шаблон для удаление дубликатов из контейнера по условию eps
template<std::ranges::forward_range Container, numberRConcept NumberType>
void del_dublicate(Container& cont, NumberType eps){
    Container cont_temp = std::move(cont);
    cont.clear();

    bool add;
    for(auto& value: cont_temp){
        add = true;
        
        if constexpr (requires { std::abs(value - value) < eps;}){
            for(const auto& val: cont){
                if(std::abs(value - val) < eps){ add = false; break; }
            }
        }else if  constexpr (requires { std::abs(*value - *value) < eps;}){
            if constexpr (requires { value.has_value(); value.get_value_not_optional_valid();}){
                if(value.has_value()){
                    for(const auto& val: cont){
                        if(val.has_value() && std::abs(value.get_value_not_optional_valid() - val.get_value_not_optional_valid()) < eps){ add = false; break; }
                    }
                }
            }else{
                for(const auto& val: cont){
                    if(std::abs(*value - *val) < eps){ add = false; break; }
                }
            }
        }


        if(add){
            if constexpr (requires { cont.insert(value); }) {
                cont.insert(std::move(value));
            } else if constexpr (requires { cont.insert(cont.end(), value); }) {
                cont.insert(cont.end(), std::move(value)); 
            }else{
                static_assert(requires { cont.insert(value); }, "Container is not supported!");
            }
        }
    }
}
//Шаблон для удаление дубликатов из контейнера по условию eps
//С модификацией итогового значения
template<std::ranges::forward_range Container, numberRConcept NumberType, typename Clous>
void del_dublicate(Container& cont, NumberType eps, Clous cl){
    Container cont_temp = std::move(cont);
    cont.clear();

    bool add;
    decltype(cl(*cont_temp.begin())) preob;
    for(auto& value: cont_temp){
        add = true;
        preob = cl(value);
        
        if constexpr (requires { std::abs(preob - preob) < eps;}){
            for(const auto& val: cont){
                if(std::abs(preob - val) < eps){ add = false; break; }
            }
        }else if  constexpr (requires { std::abs(*preob - *preob) < eps;}){
            if constexpr (requires { preob.has_value(); preob.get_value_not_optional_valid();}){
                if(preob.has_value()){
                    for(const auto& val: cont){
                        if(val.has_value() && std::abs(preob.get_value_not_optional_valid() - val.get_value_not_optional_valid()) < eps){ add = false; break; }
                    }
                }
            }else{
                for(const auto& val: cont){
                    if(std::abs(*preob - *val) < eps){ add = false; break; }
                }
            }
        }


        if(add){
            if constexpr (requires { cont.insert(value); }) {
                cont.insert(preob);
            } else if constexpr (requires { cont.insert(cont.end(), value); }) {
                cont.insert(cont.end(), preob); 
            }else{
                static_assert(requires { cont.insert(value); }, "Container is not supported!");
            }
        }
    }
}




//Шаблон для нахождения нуля на интервалах c сортировкой 
//Приближённая производная
template<numberRConcept NumberType, vecPointsC VecPoints, typename Func>
requires funcNumberRC<NumberType, Func>
std::vector<sqrtValue<NumberType>> findsNullNsort_func(
    VecPoints& vec,
    Func f,
    const NumberType eps,
    const unsigned maxCountIterIter
){
    std::ranges::sort(vec);

    std::vector<sqrtValue<NumberType>> ret;
    if(vec.empty()) return ret;
    
    auto iterFirst = vec.begin();
    auto iterSecond = iterFirst;
    
    sqrtValue<NumberType> result;

    NumberType F1,F2;
    while(++iterSecond != vec.end()){
        F1 = f(*iterFirst);
        F2 = f(*iterSecond);
        if(F1 * F2 <= 0){
            result = findNullN_func(
                (*iterSecond + *iterFirst) / 2,
                f,
                eps,
                maxCountIterIter
            );

            if(!result.has_value()){
                ret.insert(ret.end(),sqrtValue<NumberType>(std::nullopt, ValueMissing()));
            }else{
                if(*result < *iterFirst || *result > *iterSecond){
                    ret.insert(ret.end(),sqrtValue<NumberType>(std::nullopt, ValueMissing()));
                    ret.back().set_range(std::make_pair(*iterFirst, *iterSecond));
                    // ret.insert(ret.end(),sqrtValue<NumberType>(*result, ValueNotRange()));
                }else{
                    ret.insert(ret.end(),
                        { *result }
                    );
                }
            }
        }
        ++iterFirst;
    }

    del_dublicate(ret, eps);
    // del_dublicate(ret, eps, [&vec](auto& val){ 
    //         if( val.has_value() &&
    //             (val.get_value_not_optional_valid() >= vec.front() && val.get_value_not_optional_valid() <= vec.back()) && 
    //             val.get_code_error() == 3
    //         ) val.set_exception(Succ());
    //         return val;
    // });

    return ret;
}


//Шаблон для нахождения нуля на интервалах c сортировкой 
//С производной
template<numberRConcept NumberType, vecPointsC VecPoints, typename Func, typename Funcp>
requires funcNumberRC<NumberType, Func> && funcNumberRC<NumberType, Funcp>
std::vector<sqrtValue<NumberType>> findsNullNsort_func(
    VecPoints& vec,
    Func f,
    Funcp fp,
    const NumberType eps,
    const unsigned maxCountIterIter
){
    std::ranges::sort(vec);

    std::vector<sqrtValue<NumberType>> ret;
    if(vec.empty()) return ret;
    
    auto iterFirst = vec.begin();
    auto iterSecond = iterFirst;
    
    sqrtValue<NumberType> result;

    NumberType F1,F2;
    while(++iterSecond != vec.end()){
        F1 = f(*iterFirst);
        F2 = f(*iterSecond);
        if(F1 * F2 <= 0){
            result = findNullN_func(
                (*iterSecond + *iterFirst) / 2,
                f,
                fp,
                eps,
                maxCountIterIter
            );

            if(!result.has_value()){
                ret.insert(ret.end(),sqrtValue<NumberType>(std::nullopt, ValueMissing()));
            }else{
                if(*result < *iterFirst || *result > *iterSecond){
                    ret.insert(ret.end(),sqrtValue<NumberType>(std::nullopt, ValueMissing()));
                    ret.back().set_range(std::make_pair(*iterFirst, *iterSecond));
                    // ret.insert(ret.end(),sqrtValue<NumberType>(*result, ValueNotRange()));
                }else{
                    ret.insert(ret.end(),
                        { *result }
                    );
                }
            }
        }
        ++iterFirst;
    }

    del_dublicate(ret, eps);
    // del_dublicate(ret, eps, [&vec](auto& val){ 
    //         if( val.has_value() &&
    //             (val.get_value_not_optional_valid() >= vec.front() && val.get_value_not_optional_valid() <= vec.back()) && 
    //             val.get_code_error() == 3
    //         ) val.set_exception(Succ());
    //         return val;
    // });

    return ret;
}



//Шаблон для нахождения нуля на интервалах метод деления пополам
template<numberRConcept NumberType, typename Func>
requires funcNumberRC<NumberType, Func>
sqrtValue<NumberType> findNullL_func(
        NumberType rhs, NumberType lhs,
        Func f,
        const NumberType eps,
        unsigned maxCountIter
){
    int r_sign{(std::signbit(f(rhs)) << 1) - 1};
    int l_sign;

    sqrtValue<NumberType> values;

    NumberType point;
    NumberType pointF;

    do{
        l_sign = (std::signbit(f(lhs)) << 1) - 1;
        if(r_sign * l_sign < 0){

            point  = (rhs + lhs) / 2;
            pointF = f(point);


            if(std::abs(pointF) < eps){
                return sqrtValue<NumberType>(point);
            }else{
                if(r_sign * ((std::signbit(pointF) << 1) - 1) < 0){
                    lhs = point; continue;
                }else{
                    rhs = point; continue;
                }
            }
        }else{
            values = sqrtValue<NumberType>();
            values.set_range({rhs, lhs});
            return values;
        }
    }while(--maxCountIter > 0);

    values = sqrtValue<NumberType>(std::nullopt, ValueMissing());
    values.set_range({rhs, lhs});

    return values;
}


//Шаблон для нахождения нуля на интервалах метод деления пополам
template<numberRConcept NumberType,vecPointsC VecPoints, typename Func>
requires funcNumberRC<NumberType, Func>
std::vector<sqrtValue<NumberType>> findsNullLsort_func(
        VecPoints& vec,
        Func f,
        const NumberType eps,
        const unsigned maxCountIter
){
    std::ranges::sort(vec);
    std::vector<sqrtValue<NumberType>> ret;
    
    auto iterFisrt = vec.begin();
    auto iterSecond = iterFisrt;

    sqrtValue<NumberType> value;

    while(++iterSecond != vec.end()){
        value = findNullL_func(
            *iterFisrt, *iterSecond,
            f, eps, maxCountIter
        );
        if(value.get_code_error() != 1)
            ret.insert(ret.end(),value);
        ++iterFisrt;
    }
    return ret;
}

