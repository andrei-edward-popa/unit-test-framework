/**
 * Unit Test Framework for compile time checking in VPL
 * @Copyright (C) 2020 Andrei-Edward Popa
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>
 */


CLASS(Test);

CHECK_CLASS_ATTRIBUTE(tmp);
CHECK_CLASS_ATTRIBUTE(ftmp);

CHECK_CLASS_METHOD(print);
CHECK_CLASS_METHOD(sum);

CHECK_FUNCTION(multiply);

CHECK_CLASS_ATTRIBUTE_SIGNATURE(tmp, int T::*, tmp_int);
CHECK_CLASS_ATTRIBUTE_SIGNATURE(ftmp, float T::*, ftmp_float);

CHECK_CLASS_METHOD_SIGNATURE(print, void (T::*)(), rv_print_);
CHECK_CLASS_METHOD_SIGNATURE(sum, int (T::*)(int, int), ri_sum_ii);

CHECK_FUNCTION_SIGNATURE(multiply, int (T::*)(int, int), int_multiply_int_int);


TEST(Function, Name_multiply) {
	
    ASSERT_FUNCTION(multiply, "Function 'multiply' DOES NOT exist!");
	
	ASSERT_FUNCTION_SIGNATURE(multiply, int_multiply_int_int, "Signature for function 'multiply' IS NOT correct!") BEGIN {
		
		ASSERT_EQUAL(multiply(3, 2), 6, "Function 'multiply' DOES NOT return the required result!");
		
	} END
    
}


TEST(Class_Test, Class_Existence) {

	ASSERT_CLASS(Test, "Class 'Test' HAS NOT BEEN defined!");
	
}


TEST(Class_Test, Attributes_tmp_ftmp_Existence) {
	
	ASSERT_CLASS_ATTRIBUTE(Test, tmp, "Class 'Test' HAS NOT 'tmp' attribute!");	
	ASSERT_CLASS_ATTRIBUTE(Test, ftmp, "Class 'Test' HAS NOT 'ftmp' attribute!");
	
}

TEST(Class_Test, Attributes_tmp_ftmp_Signatures) {

	ASSERT_CLASS_ATTRIBUTE_SIGNATURE(Test, tmp, tmp_int, "Class 'Test' HAS NOT an integer 'tmp' attribute!");
	ASSERT_CLASS_ATTRIBUTE_SIGNATURE(Test, ftmp, ftmp_float, "Class 'Test' HAS NOT a float 'ftmp' attribute!");
	
}


TEST(Class_Test, Methods_print_sum_Existence) {
	
	ASSERT_CLASS_METHOD(Test, print, "Class 'Test' HAS NOT 'print' method!");
	ASSERT_CLASS_METHOD(Test, sum, "Class 'Test' HAS NOT 'sum' method!");
	
}


TEST(Class_Test, Methods_print_sum_Signatures) {
		
	ASSERT_CLASS_METHOD_SIGNATURE(Test, sum, ri_sum_ii, "Signature for method 'sum' in class 'Test' IS NOT correct!");
	ASSERT_CLASS_METHOD_SIGNATURE(Test, print, rv_print_, "Signature for method 'print' in class 'Test' IS NOT correct!");
		
}


TEST(Class_Test, Default_Constructor) {	

	ASSERT_CLASS_CONSTRUCTOR("Default constructor DOES NOT exist!", Test) BEGIN {
		Test test_default;
		auto tmp = __check_attribute_tmp__(&test_default);
		auto ftmp = __check_attribute_ftmp__(&test_default);
		auto sum = __check_method_sum__(&test_default, 2, 3);
		ASSERT_EQUAL(tmp, 10, "Default constructor DOES NOT initialize the attribute 'tmp' correctly!");
		ASSERT_EQUAL(ftmp, 12.45f, "Default constructor DOES NOT initialize the attribute 'ftmp' correctly!");
		ASSERT_EQUAL(sum, 15, "Method 'sum' from class 'Test' DOES NOT return the required result!");
	} END
	
}


TEST(Class_Test, Two_Arguments_Constructor) {
		
	ASSERT_CLASS_CONSTRUCTOR("Constructor with args (int, float) DOES NOT exist!", Test, int, float) BEGIN {
		Test test_int_float(1, 1.5f);
		auto tmp = __check_attribute_tmp__(&test_int_float);
		auto ftmp = __check_attribute_ftmp__(&test_int_float);
		auto sum = __check_method_sum__(&test_int_float, 2, 3);
		ASSERT_EQUAL(tmp, 1, "Constructor with args (int, float) DOES NOT initialize the attribute 'tmp' correctly!");
		ASSERT_EQUAL(ftmp, 1.5f, "Constructor with args (int, float) DOES NOT initialize the attribute 'ftmp' correctly!");	
	} END
	
}

