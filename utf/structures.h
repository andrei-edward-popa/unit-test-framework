/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */
 
#pragma once

#include <any>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cstring>

#define RUN_ALL_TESTS() RUN(suites, TestCase::requirements)
#define RUN_ONE_TEST(test_name) RUN_ONE(suites, test_name, TestCase::requirements)

namespace utf {

template<typename T>
struct Assertion {
	std::string mErrorMessage;
    std::string mOperand;
    T mFirstOperand;
    T mSecondOperand;
    bool mCheck;
};

template<typename T>
struct Holder {
    std::vector<Assertion<T>> mAssertions;
};

template<typename T>
struct Test {
    std::string mSuiteName;
    std::string mTestName;
    void (*mFunctionTester)(Holder<T> *);
};

template<typename T>
struct Insertion {
    Insertion(const std::string& suite_name, const std::string& test_name, void(*function_to_test)(Holder<T> *), std::unordered_map<std::string, std::vector<Test<T>>> &suites) {
        suites[suite_name].push_back(Test<T>{ suite_name, test_name, function_to_test });
    }
    virtual ~Insertion() = default;
};

template <typename, typename = void>
struct has_ostream_operator : std::false_type {};

template <typename T>
struct has_ostream_operator<T, decltype(void(std::declval<std::ostream&>() << std::declval<const T&>()))>: std::true_type {};

struct any : public std::any {
    std::function<void(std::ostream&, const std::any&, char delim)> print;

    template <typename T>
    any(const T& t) : std::any(t) {
    	if constexpr (has_ostream_operator<T>::value) {
		    this->print = [](std::ostream& os, const std::any& a, char delim) { 
				os << std::any_cast<const T&>(a) << delim;
	 	    };
	 	} else {
	 		this->print = [](std::ostream& os, const std::any& a, char delim) { 
				os << "Class doesn't have an ostream operator\n";
	 	    };
	 	}
    }
};

template<typename, typename = void>
constexpr bool is_complete_type_v = false;

template<typename T>
constexpr bool is_complete_type_v<T, std::void_t<decltype(sizeof(T))>> = true;
    
template<typename T, typename Callable>
constexpr void call_if_class_defined(utf::Holder<utf::any> *holder, Callable&& callable) {
  if constexpr (utf::is_complete_type_v<T>) {
    callable(holder, static_cast<T*>(nullptr));
  }
}

template <typename... Args>
struct ambiguate : public Args... {};

template<typename A, typename = void>
struct got_type : std::false_type {};

template<typename A>
struct got_type<A> : std::true_type {
    typedef A type;
};

template<typename T, T>
struct sig_check : std::true_type {};

template<typename Alias, typename AmbiguitySeed>
struct has_member {
    template<typename C> static char ((&f(decltype(&C::value))))[1];
    template<typename C> static char ((&f(...)))[2];

    static_assert((sizeof(f<AmbiguitySeed>(0)) == 1), "Check member names, it should be identical!");

    static bool const value = sizeof(f<Alias>(0)) == 2;
};

std::string to_string(std::string str) {
    return std::move(str);
}

template <typename T>
constexpr std::enable_if_t<!std::is_convertible<T, std::string>::value, std::string> to_string(T&& value) {
    using std::to_string;
    std::string converted;
    if constexpr (std::is_floating_point<typename std::remove_reference<decltype(value)>::type>::value) {
    	std::stringstream ss;
		ss << std::fixed << std::setprecision(0) << value * 1000;
		converted = ss.str();
    } else if constexpr (has_ostream_operator<T>::value) {
    	converted = std::to_string(std::forward<T>(value));
    } else {
    	converted = "Class doesn't have an ostream operator!";
    }
    return converted;
}

}

