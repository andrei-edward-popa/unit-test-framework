/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */

#pragma once

#include "structures.h"

static std::unordered_map<std::string, std::vector<utf::Test<utf::any>>> suites;
static int segmentation_fault_case_index = 0;                           
static char segmentation_fault_case[64][2][1024];

#define TEST(suite_name, test_name)                                                                                                                                             \
    template<typename T>                                                                                                                                                        \
    constexpr void __##suite_name##__##test_name(utf::Holder<T> *holder);                                                                                                       \
    utf::Insertion<utf::any> ut__##suite_name##__##test_name{  #suite_name, #test_name, __##suite_name##__##test_name, suites };                                                \
    template<typename T>                                                                                                                                                        \
    constexpr void __##suite_name##__##test_name(utf::Holder<T> *holder)

#define ASSERT_EQUAL(first_operand, second_operand, message)                                                                                                                    \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "==", T(first_operand), T(second_operand), utf::to_string(first_operand) == utf::to_string(second_operand) })

#define ASSERT_NOT_EQUAL(first_operand, second_operand, message)                                                                                                                \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "!=", T(first_operand), T(second_operand), utf::to_string(first_operand) != utf::to_string(second_operand) })

#define ASSERT_GREATER(first_operand, second_operand, message)                                                                                                                  \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", ">", T(first_operand), T(second_operand), utf::to_string(first_operand) > utf::to_string(second_operand) })

#define ASSERT_LESS(first_operand, second_operand, message)                                                                                                                     \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "<", T(first_operand), T(second_operand), utf::to_string(first_operand) < utf::to_string(second_operand) })

#define ASSERT_GREATER_OR_EQUAL(first_operand, second_operand, message)                                                                                                         \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", ">=", T(first_operand), T(second_operand), utf::to_string(first_operand) >= utf::to_string(second_operand) })

#define ASSERT_LESS_OR_EQUAL(first_operand, second_operand, message)                                                                                                            \
    holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "<=", T(first_operand), T(second_operand), utf::to_string(first_operand) <= utf::to_string(second_operand) })

#define ASSERT_INHERITANCE(base_class, derived_class, message)                                                                                                                  \
	ASSERT_CALL_CLASS(base_class)                                                                                                                                               \
		ASSERT_CALL_CLASS(derived_class)                                                                                                                                        \
		holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                 \
									   std::is_base_of<base_class, derived_class>::value == true });                                                                            \
		END                                                                                                                                                                     \
	END

#define ASSERT_ABSTRACT(class_name, message)                                                                                                                                    \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								   std::is_abstract<class_name>::value == true });                                                                                              \
	END
             
#define ASSERT_CONSTRUCTOR(message, class_name, ...)                                                                                                                            \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	constexpr bool has_constructor = (((std::string_view(#__VA_ARGS__) == std::string_view("const "#class_name"&"))                                                             \
								        || (std::string_view(#__VA_ARGS__) == std::string_view("const "#class_name" &")))                                                       \
								        && ((!std::is_trivially_copy_constructible<class_name>::value == 1)                                                                     \
								        && (std::is_copy_constructible<class_name>::value == 1))) ||                                                                            \
	                                 (((std::string_view(#__VA_ARGS__) == std::string_view(#class_name"&&"))                                                                    \
								        || (std::string_view(#__VA_ARGS__) == std::string_view(#class_name" &&")))                                                              \
								        && ((!std::is_trivially_move_constructible<class_name>::value == 1)                                                                     \
								        && (std::is_move_constructible<class_name>::value == 1))) ||                                                                            \
	                                 ((std::string_view(#__VA_ARGS__) == std::string_view(""))                                                                                  \
	                                    && (!std::is_trivially_default_constructible<class_name>::value == 1)                                                                   \
	                                    && (std::is_default_constructible<class_name>::value == 1)) ||                                                                          \
	                                 ((std::string_view(#__VA_ARGS__) != std::string_view(""))                                                                                  \
	                                    && (std::string_view(#__VA_ARGS__) != std::string_view("const "#class_name"&"))                                                         \
	                                    && (std::string_view(#__VA_ARGS__) != std::string_view("const "#class_name" &"))                                                        \
	                                    && (std::string_view(#__VA_ARGS__) != std::string_view(#class_name"&&"))                                                                \
	                                    && (std::string_view(#__VA_ARGS__) != std::string_view(#class_name" &&"))                                                               \
	                                    && (std::is_constructible<class_name, ##__VA_ARGS__>::value == 1));                                                                     \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0, has_constructor });                                                                 \
	if constexpr (has_constructor)
	
#define ASSERT_DESTRUCTOR(message, class_name)                                                                                                                                  \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								   !std::is_trivially_destructible<class_name>::value == 1 });                                                                                  \
	END
	
#define ASSERT_ATTRIBUTE(class_name, attribute_name, message)                                                                                                                   \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_attribute_##attribute_name##__ = class_name##Wrapper::__has_attribute_##attribute_name##__<class_name>::value;                        \
	const bool __class_##class_name##_has_attribute_with_##attribute_name##__ = class_name##Wrapper::__has_attribute_with_sig_##attribute_name##__<class_name>::value;          \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								   __class_##class_name##_has_attribute_##attribute_name##__ == true &&                                                                         \
								   __class_##class_name##_has_attribute_with_##attribute_name##__ == true });                                                                   \
	END

#define ASSERT_METHOD(class_name, method_name, message)                                                                                                                         \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_method_##method_name##__ = class_name##Wrapper::__has_method_##method_name##__<class_name>::value;                                    \
	const bool __class_##class_name##_has_method_with_##method_name##__ = class_name##Wrapper::__has_method_with_sig_##method_name##__<class_name>::value;                      \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								   __class_##class_name##_has_method_##method_name##__ == true &&                                                                               \
								   __class_##class_name##_has_method_with_##method_name##__ == true });                                                                         \
	END
	
#define ASSERT_OPERATOR(class_name, operator_template, message)                                                                                                                 \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_operator_##operator_template##__ = class_name##Wrapper::__has_operator_##operator_template##__<class_name>::value;                    \
	const bool __class_##class_name##_has_operator_with_##operator_template##__ = class_name##Wrapper::__has_operator_with_sig_##operator_template##__<class_name>::value;      \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								  __class_##class_name##_has_operator_##operator_template##__ == true &&                                                                        \
								  __class_##class_name##_has_operator_with_##operator_template##__ == true });                                                                  \
	END
	
#define ASSERT_FUNCTION(function_name, message)                                                                                                                                 \
	ASSERT_CALL_FUNCTION(function_name)                                                                                                                                         \
	const bool __class_GlobalFunctionsWrapper_has_function_##function_name##__ = __has_function_##function_name##__<GlobalFunctionsWrapper>::value;                             \
	const bool __class_GlobalFunctionsWrapper_has_function_with_##function_name##__ = __has_function_with_sig_##function_name##__<GlobalFunctionsWrapper>::value;               \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
								   __class_GlobalFunctionsWrapper_has_function_##function_name##__ == true &&                                                                   \
								   __class_GlobalFunctionsWrapper_has_function_with_##function_name##__ == true });                                                             \
	if constexpr (__class_GlobalFunctionsWrapper_has_function_##function_name##__ == true &&                                                                                    \
			      __class_GlobalFunctionsWrapper_has_function_with_##function_name##__ == true)
	
#define ASSERT_CLASS(class_name, message)                                                                                                                                       \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	holder->mAssertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                     \
									   __class_##class_name##_exists__ == true });                                                                                              \
	END
    
#define ASSERT_CALL_CLASS(class_name)                                                                                                                                           \
	utf::call_if_class_defined<struct class_name>(holder, [&](utf::Holder<utf::any> *holder, auto* ptr_##class_name) constexpr -> void {                                        \
		using class_name = std::decay_t<decltype(*ptr_##class_name)>;                                                                                                           \
		__class_##class_name##_exists__ = true;
		
#define ASSERT_CALL_FUNCTION(function_name)                                                                                                                                     \
	utf::call_if_class_defined<struct GlobalFunctionsWrapper>(holder, [&](utf::Holder<utf::any> *holder, auto* ptr_##function_name) constexpr -> void {                         \
		using GlobalFunctionsWrapper = std::decay_t<decltype(*ptr_##function_name)>;
		
#define CHECK_ATTRIBUTE(attribute_name, attribute_type)                                                                                                                         \
	template<class T>                                                                                                                                                           \
	static auto attribute_name(T* obj) -> decltype(obj->attribute_name) {                                                                                                       \
		return obj->attribute_name;                                                                                                                                             \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	static auto attribute_name(...) -> std::string {                                                                                                                            \
		return std::string("Attribute ") + std::string(#attribute_name) + std::string(" is not defined!");                                                                      \
	}                                                                                                                                                                           \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct Alias_##attribute_name;                                                                                                                                              \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct Alias_##attribute_name<T, std::integral_constant<bool, utf::got_type<decltype(&T::attribute_name)>::value>> {                                                        \
		static const decltype(&T::attribute_name) value;                                                                                                                        \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	struct AmbiguitySeed_##attribute_name {                                                                                                                                     \
		char attribute_name;                                                                                                                                                    \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct __has_attribute_##attribute_name##__ {                                                                                                                               \
		static const bool value = utf::has_member<Alias_##attribute_name<utf::ambiguate<T, AmbiguitySeed_##attribute_name>>,                                                    \
		                                     Alias_##attribute_name<AmbiguitySeed_##attribute_name>>::value;                                                                    \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_attribute_with_sig_##attribute_name##__ : std::false_type {};                                                                                                  \
			                                                                                                                                                                    \
	template<typename T>                                                                                                                                                        \
	struct __has_attribute_with_sig_##attribute_name##__<T, std::integral_constant<bool,                                                                                        \
		                                   utf::sig_check<attribute_type T::*, &T::attribute_name>::value>> : std::true_type {}
	
#define CHECK_METHOD(method_name, return_type, ...)                                                                                                                             \
	template<typename T, typename... Ts>                                                                                                                                        \
	static auto method_name(T* obj, Ts... args) -> decltype(obj->method_name(args...)) {                                                                                        \
		return obj->method_name(args...);                                                                                                                                       \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	static auto method_name(...) -> std::string {                                                                                                                               \
		return std::string("Method ") + std::string(#method_name) + std::string(" is not defined!");                                                                            \
	}                                                                                                                                                                           \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct Alias_method_##method_name;                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct Alias_method_##method_name<T, std::integral_constant<bool, utf::got_type<decltype(&T::method_name)>::value>> {                                                       \
		static const decltype(&T::method_name) value;                                                                                                                           \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	struct AmbiguitySeed_##method_name {                                                                                                                                        \
		char method_name;                                                                                                                                                       \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct __has_method_##method_name##__ {                                                                                                                                     \
		static const bool value = utf::has_member<Alias_method_##method_name<utf::ambiguate<T, AmbiguitySeed_##method_name>>,                                                   \
		                                     Alias_method_##method_name<AmbiguitySeed_##method_name>>::value;                                                                   \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_method_with_sig_##method_name##__ : std::false_type {};                                                                                                        \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_method_with_sig_##method_name##__<T, std::integral_constant<bool,                                                                                              \
	                                       utf::sig_check<return_type(T::*)(POP_LAST(__VA_ARGS__)) LAST(__VA_ARGS__), &T::method_name>::value>> : std::true_type {};
	                                       
#define CHECK_OPERATOR(operator_name, operator_template, return_type, ...)                                                                                                      \
	template<typename T, typename... Ts>                                                                                                                                        \
	static auto operator_template(T* obj, Ts... args) -> decltype(obj->operator_name(args...)) {                                                                                \
		return obj->operator_name(args...);                                                                                                                                     \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	static auto operator_template(...) -> std::string {                                                                                                                         \
		return std::string("Method ") + std::string(#operator_template) + std::string(" is not defined!");                                                                      \
	}                                                                                                                                                                           \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct Alias_operator_##operator_template;                                                                                                                                  \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct Alias_operator_##operator_template<T, std::integral_constant<bool, utf::got_type<decltype(&T::operator_name)>::value>> {                                             \
		static const decltype(&T::operator_name) value;                                                                                                                         \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	struct AmbiguitySeed_##operator_template {                                                                                                                                  \
		NO_ARG(+) NO_ARG(-) NO_ARG(*) NO_ARG(&) NO_ARG(~) NO_ARG(!) NO_ARG(++) NO_ARG(--) NO_ARG(()) NO_ARG(->)                                                                 \
		ONE_ARG(/)  ONE_ARG(%)  ONE_ARG(^)  ONE_ARG(|) ONE_ARG(<)  ONE_ARG(>)  ONE_ARG(<=) ONE_ARG(>=) ONE_ARG(<<) ONE_ARG(>>) ONE_ARG(<<=) ONE_ARG(>>=) ONE_ARG(||)            \
		ONE_ARG(==) ONE_ARG(!=) ONE_ARG(&&) ONE_ARG(+=) ONE_ARG(-=) ONE_ARG(/=) ONE_ARG(%=) ONE_ARG(^=) ONE_ARG(&=) ONE_ARG(|=) ONE_ARG(*=) ONE_ARG([])  ONE_ARG(->*)           \
		void operator,(size_t param) {}                                                                                                                                         \
		void* operator new(size_t param) { int* r = new int; return r; }                                                                                                        \
		void operator delete(void* param) {}                                                                                                                                    \
		void* operator new[](size_t param) { int* r = new int; return r; }                                                                                                      \
		void operator delete[](void* param) {}                                                                                                                                  \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct __has_operator_##operator_template##__ {                                                                                                                             \
		static const bool value = utf::has_member<Alias_operator_##operator_template<utf::ambiguate<T, AmbiguitySeed_##operator_template>>,                                     \
		                                     Alias_operator_##operator_template<AmbiguitySeed_##operator_template>>::value;                                                     \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
    template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_operator_with_sig_##operator_template##__ : std::false_type {};                                                                                                \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_operator_with_sig_##operator_template##__<T, std::integral_constant<bool,                                                                                      \
	                                       utf::sig_check<return_type(T::*)(POP_LAST(__VA_ARGS__)) LAST(__VA_ARGS__), &T::operator_name>::value>> : std::true_type {};
	
#define CHECK_FUNCTION(function_name, return_type, ...)                                                                                                                         \
	template<typename T, typename... Ts>                                                                                                                                        \
	static auto function_name(T* obj, Ts... args) -> decltype(obj->function_name(args...)) {                                                                                    \
		return obj->function_name(args...);                                                                                                                                     \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	static auto function_name(...) -> std::string {                                                                                                                             \
		return std::string("Function ") + std::string(#function_name) + std::string(" is not defined!");                                                                        \
	}                                                                                                                                                                           \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct Alias_function_##function_name;                                                                                                                                      \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct Alias_function_##function_name<T, std::integral_constant<bool, utf::got_type<decltype(&T::function_name)>::value>> {                                                 \
		static const decltype(&T::function_name) value;                                                                                                                         \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	struct AmbiguitySeed_##function_name {                                                                                                                                      \
		char function_name;                                                                                                                                                     \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T>                                                                                                                                                        \
	struct __has_function_##function_name##__ {                                                                                                                                 \
		static const bool value = utf::has_member<Alias_function_##function_name<utf::ambiguate<T, AmbiguitySeed_##function_name>>,                                             \
		                                     Alias_function_##function_name<AmbiguitySeed_##function_name>>::value;                                                             \
	};                                                                                                                                                                          \
	                                                                                                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_function_with_sig_##function_name##__ : std::false_type {};                                                                                                    \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_function_with_sig_##function_name##__<T, std::integral_constant<bool,                                                                                          \
	                                       utf::sig_check<return_type(T::*)(__VA_ARGS__), &T::function_name>::value>> : std::true_type {}
		                                   
#define CHECK_CONTAINER(container_name, template_postfix, ...)                                                                                                                  \
	template <typename T, typename = void>                                                                                                                                      \
	struct __is_##container_name##_of_##template_postfix##__ : std::false_type {};                                                                                              \
		                                                                                                                                                                        \
	template <typename T>                                                                                                                                                       \
	struct __is_##container_name##_of_##template_postfix##__<T, typename std::enable_if<std::is_same<T, std::container_name<__VA_ARGS__>>::value>::type> : std::true_type {};   \
	                                                                                                                                                                            \
	auto __check_##container_name##_of_##template_postfix##__(auto cont, uint64_t index) -> std::string {                                                                       \
		if constexpr (__is_##container_name##_of_##template_postfix##__<decltype(cont)>::value == 1) {                                                                          \
			auto it = std::next(cont.begin(), index);                                                                                                                           \
			if (std::distance(cont.begin(), it) >= 0 && std::distance(it, cont.end()) > 0) {                                                                                    \
				if constexpr (!(std::string_view(#container_name) == "map" || std::string_view(#container_name) == "unordered_map" ||                                           \
								std::string_view(#container_name) == "unordered_multimap" || std::string_view(#container_name) == "multimap")) {                                \
					return utf::to_string(*it);                                                                                                                                 \
				} else {                                                                                                                                                        \
					return utf::to_string((*it).first) + " " + utf::to_string((*it).second);                                                                                    \
				}                                                                                                                                                               \
			} else {                                                                                                                                                            \
				return "Index of out bound!";                                                                                                                                   \
			}                                                                                                                                                                   \
		} else {                                                                                                                                                                \
			return "Container type is incorrect!";                                                                                                                              \
		}                                                                                                                                                                       \
	}
	
#define REFLECT_SEGMENTATION_FAULT(test_case, message)                                                                                                                          \
	strcpy(segmentation_fault_case[segmentation_fault_case_index][0], test_case);                                                                                               \
	strcpy(segmentation_fault_case[segmentation_fault_case_index][1], message);                                                                                                 \
	segmentation_fault_case_index++;
		                                   
#define BEGIN
#define END });
#define COMMA ,
#define PARAM void*
#define RETURN void
#define NO_ARG(sign) RETURN operator sign() {}
#define ONE_ARG(sign) RETURN operator sign(PARAM param) {}

#define NARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define NARGS(...) NARGS_HELPER(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define PRIMITIVE_CONCAT(x, y) x ## y
#define CONCAT(x, y) PRIMITIVE_CONCAT(x, y)

#define POP_LAST(...) CONCAT(POP_LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define POP_LAST_1(x1)
#define POP_LAST_2(x1, x2) x1
#define POP_LAST_3(x1, x2, x3) x1, x2
#define POP_LAST_4(x1, x2, x3, x4) x1, x2, x3
#define POP_LAST_5(x1, x2, x3, x4, x5) x1, x2, x3, x4
#define POP_LAST_6(x1, x2, x3, x4, x5, x6) x1, x2, x3, x4, x5
#define POP_LAST_7(x1, x2, x3, x4, x5, x6, x7) x1, x2, x3, x4, x5, x6
#define POP_LAST_8(x1, x2, x3, x4, x5, x6, x7, x8) x1, x2, x3, x4, x5, x6, x7
#define POP_LAST_9(x1, x2, x3, x4, x5, x6, x7, x8, x9) x1, x2, x3, x4, x5, x6, x7, x8
#define POP_LAST_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x1, x2, x3, x4, x5, x6, x7, x8, x9

#define LAST(...) CONCAT(LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define LAST_1(x1) x1
#define LAST_2(x1, x2) x2
#define LAST_3(x1, x2, x3) x3
#define LAST_4(x1, x2, x3, x4) x4
#define LAST_5(x1, x2, x3, x4, x5) x5
#define LAST_6(x1, x2, x3, x4, x5, x6) x6
#define LAST_7(x1, x2, x3, x4, x5, x6, x7) x7
#define LAST_8(x1, x2, x3, x4, x5, x6, x7, x8) x8
#define LAST_9(x1, x2, x3, x4, x5, x6, x7, x8, x9) x9
#define LAST_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10) x10

#define REFLECT_CLASS(class_name)                                                                                                                                               \
	class class_name;                                                                                                                                                           \
	static bool __class_##class_name##_exists__ = false;                                                                                                                        \
	struct class_name##Wrapper
	
#define REFLECT_ATTRIBUTE(attribute_name, attribute_type)                                                                                                                       \
	CHECK_ATTRIBUTE(attribute_name, attribute_type);
	
#define REFLECT_METHOD(method_name, return_type, ...)                                                                                                                           \
	CHECK_METHOD(method_name, return_type, __VA_ARGS__);
	
#define REFLECT_OPERATOR(operator_name, operator_template, return_type, ...)                                                                                                    \
	CHECK_OPERATOR(operator_name, operator_template, return_type, __VA_ARGS__);
	
#define REFLECT_FUNCTION(function_name, return_type, ...)                                                                                                                       \
	CHECK_FUNCTION(function_name, return_type, __VA_ARGS__);
	                                       
template<typename T>
constexpr void RUN(const std::unordered_map<std::string, std::vector<utf::Test<T>>>& suites, std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>>& requirements) {

    for (const std::pair<std::string, std::vector<utf::Test<T>>>& suite : suites) {

        const std::vector<utf::Test<T>>& tests = suite.second;
        const std::string& suite_name = suite.first;

        for (size_t test_index = 0; test_index < tests.size(); test_index++) {

		    const utf::Test<T> test = tests[test_index];
		    utf::Holder<T> holder;
		    (test.mFunctionTester)(&holder);
		    requirements[test.mSuiteName + "::" + test.mTestName].first = true;

		    for (size_t assert_index = 0; assert_index < holder.mAssertions.size(); assert_index++) {
		    	
		        const utf::Assertion<T> assertion = holder.mAssertions[assert_index];
		        requirements[test.mSuiteName + "::" + test.mTestName].first &= assertion.mCheck;
		        if (!assertion.mCheck) {
		        	requirements[test.mSuiteName + "::" + test.mTestName].second.push_back(assertion.mErrorMessage);
		        }
		        
		    }

	    }
    }
}

template<typename T>
constexpr void RUN_ONE(std::unordered_map<std::string, std::vector<utf::Test<T>>>& suites, const std::string& full_name, std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>>& requirements) {

	int64_t colon_index = full_name.find("::");
	
	const std::string& suite_name = full_name.substr(0, colon_index);
	const std::string& test_name = full_name.substr(colon_index + 2, full_name.size());
	
	if (suite_name == std::string("Segmentation") && suites.count(suite_name) == 0) {
		return;
	}
	
	if (colon_index == std::string::npos || suites.count(suite_name) == 0) {
		printf("Error while creating tests. Your test cases file contains at least one requirement that don't respect the convention name SUITE::TEST!\nABORT!\n");
		return;
	}
    
    const utf::Test<T> current_test = *std::find_if(suites[suite_name].begin(), suites[suite_name].end(), [&](const utf::Test<T> &test) {
		return test.mTestName == test_name;
	});

    utf::Holder<T> holder;
    (current_test.mFunctionTester)(&holder);
    requirements[full_name].first = true;

    for (size_t assert_index = 0; assert_index < holder.mAssertions.size(); assert_index++) {
    	
        const utf::Assertion<T> assertion = holder.mAssertions[assert_index];
        requirements[full_name].first &= assertion.mCheck;
        if (!assertion.mCheck) {
        	requirements[full_name].second.push_back(assertion.mErrorMessage);
        }
        
    }

}

