#pragma once

#include "link.h"
#include "concept.hpp"

//Класс для обработки корней
template<numberRConcept T>
class sqrtValue{
public:
    typedef T value_t;

    explicit sqrtValue(): value{std::nullopt}, var_err{ValueNull()}{};
    sqrtValue(const std::nullopt_t& val): value{std::nullopt}, var_err{ValueNull()}{};
    sqrtValue(const value_t& val): value{val}, var_err{Succ()}{};
    sqrtValue(const std::nullopt_t& val, const ValueMissing& t): value{val}, var_err{t}{};
    sqrtValue(const value_t& val, const ValueNotRange& t): value{val}, var_err{t}{};

    bool has_value() const noexcept { return value.has_value(); }
    
    bool operator==(const sqrtValue& rhs) const noexcept {return this->value == rhs.value;}
    bool operator!=(const sqrtValue& rhs) const noexcept {return this->value != rhs.value;}

    bool operator<(const sqrtValue& rhs) const {
        if (!this->has_value() || !rhs.has_value()) throw ValueNull();
        return *(this->value) < *(rhs.value);
    }

    bool operator>(const sqrtValue& rhs) const {
        if (!this->has_value() || !rhs.has_value()) throw ValueNull();
        return *(this->value) > *(rhs.value);
    }

    bool operator<=(const sqrtValue& rhs) const {
        if (!this->has_value() || !rhs.has_value()) throw ValueNull();
        return *(this->value) <= *(rhs.value);
    }

    bool operator>=(const sqrtValue& rhs) const {
        if (!this->has_value() || !rhs.has_value()) throw ValueNull();
        return *(this->value) >= *(rhs.value);
    }


    const value_t& operator*() const{ 
        if(!value.has_value()){
            if(std::holds_alternative<Succ>(var_err)) var_err = ValueNull();
        }
        get_try_exception();
        return *value;
    }



    value_t& operator*(){ 
        if(!value.has_value()){
            if(std::holds_alternative<Succ>(var_err)) var_err = ValueNull();
        }
        get_try_exception();
        return *value;
    }

    explicit operator bool() const noexcept{ return has_value(); }

    char get_code_error() const noexcept{
        if(std::holds_alternative<Succ>(var_err)){ return 0; }
        else if(std::holds_alternative<ValueNull>(var_err)){ return 1; } 
        else if(std::holds_alternative<ValueMissing>(var_err)){ return 2; } 
        else if(std::holds_alternative<ValueNotRange>(var_err)){ return 3; } 
        exit(-1);
    }

    void get_try_exception() const{
        if(std::holds_alternative<Succ>(var_err)){ return; }
        else if(std::holds_alternative<ValueNull>(var_err)){ throw ValueNull();}
        else if(std::holds_alternative<ValueMissing>(var_err)){ throw ValueMissing();}
        else if(std::holds_alternative<ValueNotRange>(var_err)){ throw ValueNotRange();}
        exit(-1);
    }

    const std::pair<value_t, value_t> get_range() const{ return range;}
    void set_range(const std::pair<value_t, value_t>& val){ range = val; }

    const std::optional<value_t>& get_value_not_valid() const{ return value; }
    const value_t& get_value_not_optional_valid() const noexcept{ return *value; }

    void set_exception(const ValueNotRange& t) const { var_err = t;}
    void set_exception(const ValueMissing& t) const { var_err = t;}
    void set_exception(const ValueNull& t) const { var_err = t;}
    void set_exception(const Succ& t) const { var_err = t;}
    void set_exception() const { var_err = Succ();}

private:

    std::optional<value_t> value;
    std::pair<value_t, value_t> range;
    mutable std::variant<Succ, ValueNull, ValueNotRange, ValueMissing> var_err = Succ();

};
