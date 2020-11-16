/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "structures.h"

static std::unordered_map<std::string, std::vector<utf::Test<utf::any>>> suites;
static std::unordered_map<std::string, std::string> entityMapper;
static int segmentation_fault_case_index = 0;                           
static char segmentation_fault_case[64][2][1024];

#define TEST(suite_name, test_name)                                                                                                                                             \
    template<typename T>                                                                                                                                                        \
    constexpr void __##suite_name##__##test_name(utf::Holder<T> *holder);                                                                                                       \
    utf::Insertion<utf::any> ut__##suite_name##__##test_name{  #suite_name, #test_name, __##suite_name##__##test_name, suites };                                                \
    template<typename T>                                                                                                                                                        \
    constexpr void __##suite_name##__##test_name(utf::Holder<T> *holder)

#define ASSERT_EQUAL(first_operand, second_operand, message)                                                                                                                    \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "==", T(first_operand), T(second_operand), utf::to_string(first_operand) == utf::to_string(second_operand) })

#define ASSERT_NOT_EQUAL(first_operand, second_operand, message)                                                                                                                \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "!=", T(first_operand), T(second_operand), utf::to_string(first_operand) != utf::to_string(second_operand) })

#define ASSERT_GREATER(first_operand, second_operand, message)                                                                                                                  \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", ">", T(first_operand), T(second_operand), utf::to_string(first_operand) > utf::to_string(second_operand) })

#define ASSERT_LESS(first_operand, second_operand, message)                                                                                                                     \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "<", T(first_operand), T(second_operand), utf::to_string(first_operand) < utf::to_string(second_operand) })

#define ASSERT_GREATER_OR_EQUAL(first_operand, second_operand, message)                                                                                                         \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", ">=", T(first_operand), T(second_operand), utf::to_string(first_operand) >= utf::to_string(second_operand) })

#define ASSERT_LESS_OR_EQUAL(first_operand, second_operand, message)                                                                                                            \
    holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "<=", T(first_operand), T(second_operand), utf::to_string(first_operand) <= utf::to_string(second_operand) })

#define ASSERT_CLASS_INHERITANCE(base_class, derived_class, message)                                                                                                            \
	ASSERT_CALL_CLASS(base_class)                                                                                                                                               \
	ASSERT_CALL_CLASS(derived_class)                                                                                                                                            \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   std::is_base_of<base_class, derived_class>::value == true });                                                                                \
	END                                                                                                                                                                         \
	END

#define ASSERT_CLASS_ABSTRACT(class_name, message)                                                                                                                              \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   std::is_abstract<class_name>::value == true });                                                                                              \
	END
             
#define ASSERT_CLASS_CONSTRUCTOR(message, class_name, ...)                                                                                                                      \
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
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0, has_constructor });                                                                \
	if constexpr (has_constructor)
	
#define ASSERT_CLASS_DESTRUCTOR(message, class_name)                                                                                                                            \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   !std::is_trivially_destructible<class_name>::value == 1 });                                                                                  \
	END

#define ASSERT_CLASS_METHOD(class_name, method_name, message)                                                                                                                   \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_method_##method_name##__ = __has_method_##method_name##__<class_name>::value;                                                         \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   __class_##class_name##_has_method_##method_name##__ == true });                                                                              \
	END

#define ASSERT_CLASS_METHOD_SIGNATURE(class_name, method_name, template_postfix, message)                                                                                       \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_method_##method_name##_with_##template_postfix##__ = __has_method_with_sig_##template_postfix##__<class_name>::value;                 \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
			 __class_##class_name##_has_method_##method_name##_with_##template_postfix##__ == true });                                                                          \
	END
	
#define ASSERT_CLASS_ATTRIBUTE(class_name, attribute_name, message)                                                                                                             \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_attribute_##attribute_name##__ = __has_attribute_##attribute_name##__<class_name>::value;                                             \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   __class_##class_name##_has_attribute_##attribute_name##__ == true });                                                                        \
	END
	
#define ASSERT_CLASS_ATTRIBUTE_SIGNATURE(class_name, attribute_name, template_postfix, message)                                                                                 \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_attribute_##attribute_name##_with_##template_postfix##__ = __has_attribute_with_sig_##template_postfix##__<class_name>::value;        \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
			 __class_##class_name##_has_attribute_##attribute_name##_with_##template_postfix##__ == true });                                                                    \
	END
	
#define ASSERT_FUNCTION(function_name, message)                                                                                                                                 \
	ASSERT_CALL_FUNCTION(function_name)                                                                                                                                         \
	const bool __class___GlobalFunctionWrapper___has_function_##function_name##__ = __has_function_##function_name##__<__GlobalFunctionWrapper__>::value;                                                         \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								   __class___GlobalFunctionWrapper___has_function_##function_name##__ == true });                                                               \
	END
	
#define ASSERT_FUNCTION_SIGNATURE(function_name, template_postfix, message)                                                                                                     \
	ASSERT_CALL_FUNCTION(function_name)                                                                                                                                         \
	const bool __class___GlobalFunctionWrapper___has_function_##function_name##_with_##template_postfix##__ = __has_function_with_sig_##template_postfix##__<__GlobalFunctionWrapper__>::value;                   \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
			 __class___GlobalFunctionWrapper___has_function_##function_name##_with_##template_postfix##__ == true });                                                           \
	if constexpr (__class___GlobalFunctionWrapper___has_function_##function_name##_with_##template_postfix##__)
	
#define ASSERT_CLASS_OPERATOR(class_name, operator_template, message)                                                                                                           \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_operator_##operator_template##__ = __has_operator_##operator_template##__<class_name>::value;                                         \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
								  __class_##class_name##_has_operator_##operator_template##__ == true });                                                                       \
	END
	
#define ASSERT_CLASS_OPERATOR_SIGNATURE(class_name, operator_name, template_postfix, message)                                                                                   \
	ASSERT_CALL_CLASS(class_name)                                                                                                                                               \
	const bool __class_##class_name##_has_operator_with_##template_postfix##__ = __has_operator_with_sig_##template_postfix##__<class_name>::value;                             \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
			 __class_##class_name##_has_operator_with_##template_postfix##__ == true });                                                                                        \
	END

#define IF(class_name, template_postfix, message)                                                                                                                               \
	const bool __##class_name##_has_##template_postfix##__ = __has_entity_with_sig_##template_postfix##__<class_name>::value;                                                   \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
			 __##class_name##_has_##template_postfix##__ == true });                                                                                                            \
	if constexpr (__##class_name##_has_##template_postfix##__)
	
#define ASSERT_CLASS(class_name, message)                                                                                                                                       \
	ASSERT_CALL_CLASS(class_name) BEGIN { } END                                                                                                                                 \
	holder->m_assertions.push_back(utf::Assertion<T>{ std::string(message) + "\n", "", 0, 0,                                                                                    \
									   __class_##class_name##_exists__ == true });
    
#define ASSERT_CALL_CLASS(class_name)                                                                                                                                           \
	utf::call_if_class_defined<struct class_name>(holder, [&](utf::Holder<utf::any> *holder, auto* ptr_##class_name) constexpr -> void {                                        \
		using class_name = std::decay_t<decltype(*ptr_##class_name)>;                                                                                                           \
		__class_##class_name##_exists__ = true;
		
#define ASSERT_CALL_FUNCTION(function_name)                                                                                                                                     \
	utf::call_if_class_defined<struct __GlobalFunctionWrapper__>(holder, [&](utf::Holder<utf::any> *holder, auto* ptr_##function_name) constexpr -> void {                      \
		using __GlobalFunctionWrapper__ = std::decay_t<decltype(*ptr_##function_name)>;
		
#define CLASS(class_name)                                                                                                                                                       \
	class class_name;                                                                                                                                                           \
	static bool __class_##class_name##_exists__ = false
	
#define CHECK_CLASS_METHOD(method_name)                                                                                                                                         \
	template<typename T, typename... Ts>                                                                                                                                        \
	auto __check_method_##method_name##__(T* obj, Ts... args) -> decltype(obj->method_name(args...)) {                                                                          \
		return obj->method_name(args...);                                                                                                                                       \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	auto __check_method_##method_name##__(...) -> std::string {                                                                                                                 \
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
	}
	
#define CHECK_CLASS_METHOD_SIGNATURE(method_name, signature, template_postfix)                                                                                                  \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_method_with_sig_##template_postfix##__ : std::false_type {};                                                                                                   \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_entity_with_sig_##template_postfix##__ : std::false_type {};                                                                                                   \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_method_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                         \
	                                       utf::sig_check<signature, &T::method_name>::value>> : std::true_type {};                                                             \
	template<typename T>                                                                                                                                                        \
	struct __has_entity_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                         \
		                                   utf::sig_check<signature, &T::method_name>::value>> : std::true_type {}
	
#define CHECK_FUNCTION(function_name)                                                                                                                                           \
	template<typename T, typename... Ts>                                                                                                                                        \
	auto __check_function_##function_name##__(T* obj, Ts... args) -> decltype(obj->function_name(args...)) {                                                                    \
		return obj->function_name(args...);                                                                                                                                     \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	auto __check_function_##function_name##__(...) -> std::string {                                                                                                             \
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
	}
	
#define CHECK_FUNCTION_SIGNATURE(function_name, signature, template_postfix)                                                                                                    \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_function_with_sig_##template_postfix##__ : std::false_type {};                                                                                                 \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_function_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                       \
	                                       utf::sig_check<signature, &T::function_name>::value>> : std::true_type {}
	
#define CHECK_CLASS_ATTRIBUTE(attribute_name)                                                                                                                                   \
	template<class T>                                                                                                                                                           \
	auto __check_attribute_##attribute_name##__(T* obj) -> decltype(obj->attribute_name) {                                                                                      \
		return obj->attribute_name;                                                                                                                                             \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	auto __check_attribute_##attribute_name##__(...) -> std::string {                                                                                                           \
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
	}
	
#define CHECK_CLASS_ATTRIBUTE_SIGNATURE(attribute_name, signature, template_postfix)                                                                                            \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_attribute_with_sig_##template_postfix##__ : std::false_type {};                                                                                                \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_entity_with_sig_##template_postfix##__ : std::false_type {};                                                                                                   \
			                                                                                                                                                                    \
	template<typename T>                                                                                                                                                        \
	struct __has_attribute_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                      \
		                                   utf::sig_check<signature T::*, &T::attribute_name>::value>> : std::true_type {};                                                     \
	template<typename T>                                                                                                                                                        \
	struct __has_entity_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                         \
		                                   utf::sig_check<signature T::*, &T::attribute_name>::value>> : std::true_type {}
		                                   
#define CHECK_CLASS_OPERATOR(operator_name, operator_template)                                                                                                                  \
	template<typename T, typename... Ts>                                                                                                                                        \
	auto __check_operator_##operator_template##__(T* obj, Ts... args) -> decltype(obj->operator_name(args...)) {                                                                \
		return obj->operator_name(args...);                                                                                                                                     \
	}                                                                                                                                                                           \
		                                                                                                                                                                        \
	auto __check_operator_##operator_template##__(...) -> std::string {                                                                                                         \
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
	}
	
#define CHECK_CLASS_OPERATOR_SIGNATURE(operator_name, signature, template_postfix)                                                                                              \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_operator_with_sig_##template_postfix##__ : std::false_type {};                                                                                                 \
	template<typename T, typename = std::true_type>                                                                                                                             \
	struct __has_entity_with_sig_##template_postfix##__ : std::false_type {};                                                                                                   \
		                                                                                                                                                                        \
	template<typename T>                                                                                                                                                        \
	struct __has_operator_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                       \
	                                       utf::sig_check<signature, &T::operator_name>::value>> : std::true_type {};                                                           \
	template<typename T>                                                                                                                                                        \
	struct __has_entity_with_sig_##template_postfix##__<T, std::integral_constant<bool,                                                                                         \
		                                   utf::sig_check<signature, &T::operator_name>::value>> : std::true_type {}
		                                   
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
	
#define SEGMENTATION_FAULT_MESSAGE(test_case, message)                                                                                                                          \
	strcpy(segmentation_fault_case[segmentation_fault_case_index][0], test_case);                                                                                               \
	strcpy(segmentation_fault_case[segmentation_fault_case_index][1], message);                                                                                                 \
	segmentation_fault_case_index++;
		                                   
#define BEGIN
#define END });
#define COMMA_HACK ,
#define PARAM void*
#define RETURN void
#define NO_ARG(sign) RETURN operator sign() {}
#define ONE_ARG(sign) RETURN operator sign(PARAM param) {}
	                                       
template<typename T>
constexpr void RUN(const std::unordered_map<std::string, std::vector<utf::Test<T>>>& suites, std::unordered_map<std::string, std::pair<bool, std::vector<std::string>>>& requirements) {

    for (const std::pair<std::string, std::vector<utf::Test<T>>>& suite : suites) {

        const std::vector<utf::Test<T>>& tests = suite.second;
        const std::string& suite_name = suite.first;

        for (size_t test_index = 0; test_index < tests.size(); test_index++) {

		    const utf::Test<T> test = tests[test_index];
		    utf::Holder<T> holder;
		    (test.m_functionTester)(&holder);
		    requirements[test.m_suiteName + "::" + test.m_testName].first = true;

		    for (size_t assert_index = 0; assert_index < holder.m_assertions.size(); assert_index++) {
		    	
		        const utf::Assertion<T> assertion = holder.m_assertions[assert_index];
		        requirements[test.m_suiteName + "::" + test.m_testName].first &= assertion.m_check;
		        if (!assertion.m_check) {
		        	requirements[test.m_suiteName + "::" + test.m_testName].second.push_back(assertion.m_errorMessage);
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
		return test.m_testName == test_name;
	});

    utf::Holder<T> holder;
    (current_test.m_functionTester)(&holder);
    requirements[full_name].first = true;

    for (size_t assert_index = 0; assert_index < holder.m_assertions.size(); assert_index++) {
    	
        const utf::Assertion<T> assertion = holder.m_assertions[assert_index];
        requirements[full_name].first &= assertion.m_check;
        if (!assertion.m_check) {
        	requirements[full_name].second.push_back(assertion.m_errorMessage);
        }
        
    }

}

#endif

