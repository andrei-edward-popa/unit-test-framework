/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */

#pragma once

#include "structures.h"

#define TEST(suite_name, test_name)																																					\
	void __##suite_name##__##test_name(utf::AssertionHolder &holder);																												\
	utf::Insertion ut__##suite_name##__##test_name{ #suite_name, #test_name, &__##suite_name##__##test_name };																		\
	void __##suite_name##__##test_name(utf::AssertionHolder &holder)

#define ASSERT_EQUAL(first, second, error)																																			\
	holder.insert(utf::Assertion{ first == second, error })

#define ASSERT_NOT_EQUAL(first, second, error)																																		\
	holder.insert(utf::Assertion{ first != second, error })

#define ASSERT_GREATER(first, second, error)																																		\
	holder.insert(utf::Assertion{ first > second, error })

#define ASSERT_LESS(first, second, error)																																			\
	holder.insert(utf::Assertion{ first < second, error })

#define ASSERT_GREATER_OR_EQUAL(first, second, error)																																\
	holder.insert(utf::Assertion{ first >= second, error })

#define ASSERT_LESS_OR_EQUAL(first, second, error)																																	\
	holder.insert(utf::Assertion{ first <= second, error })

#define ASSERT_INHERITANCE(base, derived, error)																																	\
	ASSERT_CALL_CLASS(base)																																							\
	ASSERT_CALL_CLASS(derived)																																						\
	holder.insert(utf::Assertion{ std::is_base_of<base, derived>::value, error });																									\
	END_ASSERT_CALL_CLASS																																							\
	END_ASSERT_CALL_CLASS

#define ASSERT_INHERITANCE_STL_BASE_CLASS(base, derived, error)																														\
	ASSERT_CALL_CLASS(derived)																																						\
	holder.insert(utf::Assertion{ std::is_base_of<base, derived>::value, error });																									\
	END_ASSERT_CALL_CLASS

#define ASSERT_ABSTRACT(class_name, error)																																			\
	ASSERT_CALL_CLASS(class_name)																																					\
	holder.insert(utf::Assertion{ std::is_abstract<class_name>::value, error });																									\
	END_ASSERT_CALL_CLASS

#define ASSERT_CONSTRUCTOR(class_name, ...)																																			\
	ASSERT_CALL_CLASS(class_name)																																					\
	holder.insert(utf::Assertion{ std::is_constructible<class_name POP_LAST_COMMMA(__VA_ARGS__)>::value, LAST(__VA_ARGS__) });														\
	END_ASSERT_CALL_CLASS																																							\
	using class_name = struct class_name;																																			\
	if constexpr (utf::is_complete_type_v<class_name>)

#define ASSERT_DESTRUCTOR(class_name, error)																																		\
	ASSERT_CALL_CLASS(class_name)																																					\
	holder.insert(utf::Assertion{ !std::is_trivially_destructible<class_name>::value, error });																						\
	END_ASSERT_CALL_CLASS

#define ASSERT_ATTRIBUTE(class_name, attribute_name, attribute_template, error)																										\
	ASSERT_CLASS(class_name, error);																																				\
	ASSERT_CALL_CLASS(class_name)																																					\
	const bool g_class_##class_name##_has_attribute_##attribute_name =																												\
		class_name##Wrapper::namecheck_attribute_##attribute_name##_##attribute_template<class_name>::value &&																		\
		class_name##Wrapper::sigcheck_attribute_##attribute_name##_##attribute_template<class_name>::value;																			\
	holder.insert(utf::Assertion{ g_class_##class_name##_has_attribute_##attribute_name, error });																					\
	END_ASSERT_CALL_CLASS

#define ASSERT_METHOD(class_name, method_name, method_template, error)																												\
	ASSERT_CLASS(class_name, error);																																				\
	ASSERT_CALL_CLASS(class_name)																																					\
	const bool g_class_##class_name##_has_method_##method_name =																													\
		class_name##Wrapper::namecheck_method_##method_name##_##method_template<class_name>::value &&																				\
		class_name##Wrapper::sigcheck_method_##method_name##_##method_template<class_name>::value;																					\
	holder.insert(utf::Assertion{ g_class_##class_name##_has_method_##method_name, error });																						\
	END_ASSERT_CALL_CLASS

#define ASSERT_OPERATOR(class_name, operator_name, operator_template, error)																										\
	ASSERT_CLASS(class_name, error);																																				\
	ASSERT_CALL_CLASS(class_name)																																					\
	const bool g_class_##class_name##_has_operator_##operator_name =																												\
		class_name##Wrapper::namecheck_operator_##operator_name##_##operator_template<class_name>::value &&																			\
		class_name##Wrapper::sigcheck_operator_##operator_name##_##operator_template<class_name>::value;																			\
	holder.insert(utf::Assertion{ g_class_##class_name##_has_operator_##operator_name, error });																					\
	END_ASSERT_CALL_CLASS

#define ASSERT_FUNCTION(function_name, function_template, error)																													\
	const bool g_function_##function_name =																																			\
		namecheck_function_##function_name##_##function_template &&																													\
		sigcheck_function_##function_name##_##function_template;																													\
	holder.insert(utf::Assertion{ g_function_##function_name, error });																												\
	if (g_function_##function_name)

#define ASSERT_CLASS(class_name, error)																																				\
	ASSERT_CALL_CLASS(class_name)																																					\
	END_ASSERT_CALL_CLASS																																							\
	holder.insert(utf::Assertion{ g_class_##class_name##_exists, error })

#define ASSERT_CALL_CLASS(class_name)																																				\
	utf::call_if_class_defined<struct class_name>(holder, [&](utf::AssertionHolder &holder, auto *ptr) constexpr -> void {															\
		using class_name = std::decay_t<decltype(*ptr)>;																															\
		g_class_##class_name##_exists = true;

#define REGISTER_ATTRIBUTE(attribute_name, attribute_type, attribute_template)																										\
	REGISTER_SAME_ATTRIBUTE(attribute_name, attribute_type, attribute_template);																									\
																																													\
	template<class T>																																								\
	static auto attribute_name(T* obj) -> decltype(obj->attribute_name) {																											\
		return obj->attribute_name;																																					\
	}																																												\
																																													\
	static auto attribute_name(...) -> std::string {																																\
		return std::string("Attribute ") + std::string(#attribute_name) + std::string(" is not defined!");																			\
	}

#define REGISTER_SAME_ATTRIBUTE(attribute_name, attribute_type, attribute_template)																									\
	template<typename T, typename = std::true_type>																																	\
	struct Alias_##attribute_name##_##attribute_template;																															\
																																													\
	template<typename T>																																							\
	struct Alias_##attribute_name##_##attribute_template<T, std::integral_constant<bool, utf::GotType<decltype(&T::attribute_name)>::value>> {										\
		static constexpr decltype(&T::attribute_name) value{};																														\
	};																																												\
																																													\
	struct AmbiguitySeed_##attribute_name##_##attribute_template {																													\
		char attribute_name;																																						\
	};																																												\
																																													\
	template<typename T>																																							\
	struct namecheck_attribute_##attribute_name##_##attribute_template {																											\
		static constexpr bool value = utf::HasMember<Alias_##attribute_name##_##attribute_template<utf::Ambiguate<T, AmbiguitySeed_##attribute_name##_##attribute_template>>,		\
			Alias_##attribute_name##_##attribute_template<AmbiguitySeed_##attribute_name##_##attribute_template>>::value;					\
	};																																												\
																																													\
	template<typename Class, typename = std::true_type>																																\
	struct sigcheck_attribute_##attribute_name##_##attribute_template : std::false_type {};																							\
																																													\
	template<typename Class>																																						\
	struct sigcheck_attribute_##attribute_name##_##attribute_template<Class, std::integral_constant<bool,																			\
		utf::SignatureCheck<attribute_type Class::*, &Class::attribute_name>::value>> : std::true_type {}

#define REGISTER_METHOD(method_name, method_signature, method_template)																												\
	REGISTER_SAME_METHOD(method_name, method_signature, method_template);																											\
																																													\
	template<typename T, typename... Ts>																																			\
	static auto method_name(T* obj, Ts... args) -> decltype(obj->method_name(args...)) {																							\
		return obj->method_name(args...);																																			\
	}																																												\
																																													\
	static auto method_name(...) -> std::string {																																	\
		return std::string("Method ") + std::string(#method_name) + std::string(" is not defined!");																				\
	}

#define REGISTER_SAME_METHOD(method_name, method_signature, method_template)																										\
	template<typename T, typename = std::true_type>																																	\
	struct Alias_method_##method_name##_##method_template;																															\
																																													\
	template<typename T>																																							\
	struct Alias_method_##method_name##_##method_template<T, std::integral_constant<bool, utf::GotType<decltype(&T::method_name)>::value>> {										\
		static constexpr decltype(&T::method_name) value{};																															\
	};																																												\
																																													\
	struct AmbiguitySeed_##method_name##_##method_template {																														\
		char method_name;																																							\
	};																																												\
																																													\
	template<typename T>																																							\
	struct namecheck_method_##method_name##_##method_template {																														\
		static constexpr bool value = utf::HasMember<Alias_method_##method_name##_##method_template<utf::Ambiguate<T, AmbiguitySeed_##method_name##_##method_template>>,			\
			Alias_method_##method_name##_##method_template<AmbiguitySeed_##method_name##_##method_template>>::value;																\
	};																																												\
																																													\
	template<typename Class, typename = std::true_type>																																\
	struct sigcheck_method_##method_name##_##method_template : std::false_type {};																									\
																																													\
	template<typename Class>																																						\
	struct sigcheck_method_##method_name##_##method_template<Class, std::integral_constant<bool,																					\
									utf::SignatureCheck<method_signature, &Class::method_name>::value>> : std::true_type {}

#define REGISTER_OPERATOR(operator_name, operator_identifier_name, operator_signature, operator_template)																			\
	REGISTER_SAME_OPERATOR(operator_name, operator_identifier_name, operator_signature, operator_template);																			\
																																													\
	template<typename T, typename... Ts>																																			\
	static auto operator_identifier_name(T* obj, Ts... args) -> decltype(obj->operator_name(args...)) {																				\
		return obj->operator_name(args...);																																			\
	}																																												\
																																													\
	static auto operator_identifier_name(...) -> std::string {																														\
		return std::string("Operator ") + std::string(#operator_name) + std::string(" is not defined!");																			\
	}

#define REGISTER_SAME_OPERATOR(operator_name, operator_identifier_name, operator_signature, operator_template)																		\
	template<typename T, typename = std::true_type>																																	\
	struct Alias_operator_##operator_identifier_name##_##operator_template;																											\
																																													\
	template<typename T>																																							\
	struct Alias_operator_##operator_identifier_name##_##operator_template<T, std::integral_constant<bool, utf::GotType<decltype(&T::operator_name)>::value>> {						\
		static constexpr decltype(&T::operator_name) value{};																														\
	};																																												\
																																													\
	struct AmbiguitySeed_##operator_identifier_name##_##operator_template {																											\
		NO_ARG(+) NO_ARG(-) NO_ARG(*) NO_ARG(&) NO_ARG(~) NO_ARG(!) NO_ARG(++) NO_ARG(--) NO_ARG(()) NO_ARG(->)																		\
		ONE_ARG(/)  ONE_ARG(%)  ONE_ARG(^)  ONE_ARG(|) ONE_ARG(<)  ONE_ARG(>)  ONE_ARG(<=) ONE_ARG(>=) ONE_ARG(<<) ONE_ARG(>>) ONE_ARG(<<=) ONE_ARG(>>=) ONE_ARG(||)				\
		ONE_ARG(==) ONE_ARG(!=) ONE_ARG(&&) ONE_ARG(+=) ONE_ARG(-=) ONE_ARG(/=) ONE_ARG(%=) ONE_ARG(^=) ONE_ARG(&=) ONE_ARG(|=) ONE_ARG(*=) ONE_ARG([])  ONE_ARG(->*)				\
		void operator,(size_t param) {}																																				\
		void* operator new(size_t param) { int* r = new int; return r; }																											\
		void operator delete(void* param) {}																																		\
		void* operator new[](size_t param) { int* r = new int; return r; }																											\
		void operator delete[](void* param) {}																																		\
	};																																												\
																																													\
	template<typename T>																																							\
	struct namecheck_operator_##operator_identifier_name##_##operator_template {																									\
		static constexpr bool value =																																				\
			utf::HasMember<Alias_operator_##operator_identifier_name##_##operator_template<utf::Ambiguate<T, AmbiguitySeed_##operator_identifier_name##_##operator_template>>,		\
			Alias_operator_##operator_identifier_name##_##operator_template<AmbiguitySeed_##operator_identifier_name##_##operator_template>>::value;								\
	};																																												\
																																													\
	template<typename Class, typename = std::true_type>																																\
	struct sigcheck_operator_##operator_identifier_name##_##operator_template : std::false_type {};																					\
																																													\
	template<typename Class>																																						\
	struct sigcheck_operator_##operator_identifier_name##_##operator_template<Class, std::integral_constant<bool,																	\
		utf::SignatureCheck<operator_signature, &Class::operator_name>::value>> : std::true_type {}

#define REGISTER_FUNCTION(retval, function_name, ...)																																\
	retval __attribute((weak)) function_name(POP_LAST(__VA_ARGS__));																												\
	retval (*CONCAT(g_func_ptr_, LAST(__VA_ARGS__)))(POP_LAST(__VA_ARGS__)) = &function_name;																						\
	const bool CONCAT(namecheck_function_##function_name##_, LAST(__VA_ARGS__)) =																									\
		CONCAT(g_func_ptr_, LAST(__VA_ARGS__)) != NULL;																																\
	const bool CONCAT(sigcheck_function_##function_name##_, LAST(__VA_ARGS__)) =																									\
		CONCAT(namecheck_function_##function_name##_, LAST(__VA_ARGS__)) ?																											\
		std::is_same_v<decltype(CONCAT(g_func_ptr_, LAST(__VA_ARGS__))), retval(*)(POP_LAST(__VA_ARGS__))> : false;

#define REGISTER_CLASS(class_name)																																					\
	class class_name;																																								\
	static bool g_class_##class_name##_exists = false;																																\
	struct class_name##Wrapper

#define REGISTER_SEGMENTATION_FAULT(test_case, message)																																\
	holder.insert(utf::Assertion{ true, "" });																																		\
	utf::SuiteHolder::Instance().segfault_messages().insert({ test_case, message })

#define COMMA ,
#define PARAM void*
#define RETURN void
#define END_ASSERT_CALL_CLASS });
#define NO_ARG(sign) RETURN operator sign() {}
#define ONE_ARG(sign) RETURN operator sign(PARAM param) {}

#define NARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define NARGS(...) NARGS_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define PRIMITIVE_CONCAT(first, second) first ## second
#define CONCAT(first, second) PRIMITIVE_CONCAT(first, second)

#define POP_LAST(...) CONCAT(POP_LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define POP_LAST_1(_1)
#define POP_LAST_2(_1, _2) _1
#define POP_LAST_3(_1, _2, _3) _1, _2
#define POP_LAST_4(_1, _2, _3, _4) _1, _2, _3
#define POP_LAST_5(_1, _2, _3, _4, _5) _1, _2, _3, _4
#define POP_LAST_6(_1, _2, _3, _4, _5, _6) _1, _2, _3, _4, _5
#define POP_LAST_7(_1, _2, _3, _4, _5, _6, _7) _1, _2, _3, _4, _5, _6
#define POP_LAST_8(_1, _2, _3, _4, _5, _6, _7, _8) _1, _2, _3, _4, _5, _6, _7
#define POP_LAST_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _1, _2, _3, _4, _5, _6, _7, _8
#define POP_LAST_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _1, _2, _3, _4, _5, _6, _7, _8, _9

#define POP_LAST_COMMMA(...) CONCAT(POP_LAST_COMMMA_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define POP_LAST_COMMMA_1(_1)
#define POP_LAST_COMMMA_2(_1, _2) ,_1
#define POP_LAST_COMMMA_3(_1, _2, _3) ,_1, _2
#define POP_LAST_COMMMA_4(_1, _2, _3, _4) ,_1, _2, _3
#define POP_LAST_COMMMA_5(_1, _2, _3, _4, _5) ,_1, _2, _3, _4
#define POP_LAST_COMMMA_6(_1, _2, _3, _4, _5, _6) ,_1, _2, _3, _4, _5
#define POP_LAST_COMMMA_7(_1, _2, _3, _4, _5, _6, _7) ,_1, _2, _3, _4, _5, _6
#define POP_LAST_COMMMA_8(_1, _2, _3, _4, _5, _6, _7, _8) ,_1, _2, _3, _4, _5, _6, _7
#define POP_LAST_COMMMA_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) ,_1, _2, _3, _4, _5, _6, _7, _8
#define POP_LAST_COMMMA_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) ,_1, _2, _3, _4, _5, _6, _7, _8, _9

#define LAST(...) CONCAT(LAST_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define LAST_1(_1) _1
#define LAST_2(_1, _2) _2
#define LAST_3(_1, _2, _3) _3
#define LAST_4(_1, _2, _3, _4) _4
#define LAST_5(_1, _2, _3, _4, _5) _5
#define LAST_6(_1, _2, _3, _4, _5, _6) _6
#define LAST_7(_1, _2, _3, _4, _5, _6, _7) _7
#define LAST_8(_1, _2, _3, _4, _5, _6, _7, _8) _8
#define LAST_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _9
#define LAST_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) _10

#define PRIMITIVE_HELPER_NARGS(_1, N, ...) N
#define PRIMITIVE_NARGS(...) PRIMITIVE_HELPER_NARGS(__VA_OPT__(,) Const, __VA_ARGS__)
#define EVEN_MORE_INDIRECT(...) LAST(__VA_ARGS__) __VA_OPT__(,)
#define CONST_NAME(...) PRIMITIVE_NARGS(EVEN_MORE_INDIRECT(LAST(__VA_ARGS__)))
#define CONST_CONCAT(item, ...) CONCAT(item,  CONST_NAME(__VA_ARGS__))
