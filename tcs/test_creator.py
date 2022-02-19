# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

import re
import random
import string
from collections import defaultdict

mapper = {
    '<<=': 'ShiftLeftEqual', '>>=': 'ShiftRightEqual',
    '==': 'EqualEqual', '!=': 'NotEqual', '+=': 'PlusEqual', '-=': 'MinusEqual', '*=': 'StarEqual', '/=': 'DivEqual',
    '<=': 'LessEqual', '>=': 'GreaterEqual', '&=': 'AndEqual', '|=': 'OrEqual', '%=': 'ModEqual', '^=': 'HatEqual',
    '++': 'PlusPlus', '--': 'MinusMinus', '&&': 'AndAnd', '||': 'OrOr',
    '<<': "Stdout", '>>': 'Stdin',
	'=': 'Equal', '!': 'Not', '+': 'Plus', '-': 'Minus', '*': 'Star', '/': 'Div',
	'<': 'Less', '>': 'Greater', '&': 'And', '|': "Or", '%': 'Mod', '^': 'Hat',
	'~': 'Tilda', '()': 'Call', '[]': 'Index', '->': 'Arrow', '->*': 'ArrowStar', ',': 'Comma',
	' new[]': 'NewArr', ' delete[]': 'DeleteArr', ' new': 'New', ' delete': 'Delete',
}

def get_random_string(length):
    letters = string.ascii_lowercase
    result_string = "".join(random.choice(letters) for i in range(length))
    return result_string

def test_seg_fault(out_file):
	out_file.write("TEST(private, SegmentationFault) {\n    /* insert code */\n}\n\n")

def test_abstract(out_file, vpl_out_file, entities):
	abstracts = entities["ABSTRACT"]
	vpl_out_file.write("========\nAbstract\n========\n")
	for i in range(len(abstracts)):
		out_file.write("TEST(Abstract, %s) {\n\n" % (abstracts[i].class_name))
		vpl_out_file.write("REQUIRES=Abstract::%s\n" % (abstracts[i].class_name))
		message = '"Clasa \'%s\' nu este abstractă!"' % (abstracts[i].class_name)
		out_file.write("    ASSERT_ABSTRACT(%s, %s);\n\n" % (abstracts[i].class_name, message))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")

def test_attribute(out_file, vpl_out_file, entities):
	attributes = entities["ATTRIBUTE"]
	vpl_out_file.write("=========\nAttribute\n=========\n")
	attribute_dict = defaultdict(int)
	for i in range(len(attributes)):
		dict_key = "%s%s" % (attributes[i].class_name, attributes[i].entity_name)
		if not dict_key in attribute_dict.keys():
			out_file.write("TEST(%sAttribute, %s) {\n\n" % (attributes[i].class_name, attributes[i].entity_name))
			vpl_out_file.write("REQUIRES=%sAttribute::%s\n" % (attributes[i].class_name, attributes[i].entity_name))
			attribute_dict[dict_key] = 2
		else:
			out_file.write("TEST(%sAttribute, %s%d) {\n\n" % (attributes[i].class_name, attributes[i].entity_name, attribute_dict[dict_key]))
			vpl_out_file.write("REQUIRES=%sAttribute::%s%d\n" % (attributes[i].class_name, attributes[i].entity_name, attribute_dict[dict_key]))
			attribute_dict[dict_key] += 1
		message_attribute = '"Clasa \'%s\' nu conține atributul \'%s\' sau nu s-a respectat tipul specificat în cerință!"' % (attributes[i].class_name, attributes[i].entity_name)
		out_file.write("    ASSERT_ATTRIBUTE(%s, %s, %s, %s);\n\n" % (attributes[i].class_name, attributes[i].entity_name, attributes[i].template_postfix, message_attribute))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")


def test_class(out_file, vpl_out_file, entities):
	classes = entities["CLASS"]
	vpl_out_file.write("=========\nExistence\n=========\n")
	for i in range(len(classes)):
		out_file.write("TEST(Existence, %s) {\n\n" % (classes[i].class_name))
		vpl_out_file.write("REQUIRES=Existence::%s\n" % (classes[i].class_name))
		message = '"Clasa \'%s\' nu există!"' % (classes[i].class_name)
		out_file.write("    ASSERT_CLASS(%s, %s);\n\n" % (classes[i].class_name, message))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")

def test_constructor(out_file, vpl_out_file, entities):
	constructors = entities["CONSTRUCTOR"]
	vpl_out_file.write("===========\nConstructor\n===========\n")
	for i in range(len(constructors)):
		nr_params = len(constructors[i].args.split(",")) if constructors[i].args != "" else 0
		out_file.write("TEST(Constructor, %sParam_%s) {\n\n" % (nr_params, constructors[i].class_name))
		vpl_out_file.write("REQUIRES=Constructor::%sParam_%s\n" % (nr_params, constructors[i].class_name))
		message = '"Constructorul cu %s param. pentru clasa \'%s\' nu există, parametrii nu respectă tipul specificat în cerință sau clasa este abstractă!"' % (nr_params, constructors[i].class_name)
		if (len(constructors[i].args.strip()) != 0):
			out_file.write("    ASSERT_CONSTRUCTOR(%s, %s, %s) {\n        /* insert code */\n    }\n\n" % (constructors[i].class_name, constructors[i].args, message))
		else:
			out_file.write("    ASSERT_CONSTRUCTOR(%s, %s) {\n        /* insert code */\n    }\n\n" % (constructors[i].class_name, message))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")
	
def test_destructor(out_file, vpl_out_file, entities):
	destructors = entities["DESTRUCTOR"]
	vpl_out_file.write("==========\nDestructor\n==========\n")
	for i in range(len(destructors)):
		out_file.write("TEST(Destructor, %s) {\n\n" % (destructors[i].class_name))
		vpl_out_file.write("REQUIRES=Destructor::%s\n" % (destructors[i].class_name))
		message = '"Clasa \'%s\' nu conține un destructor!"' % (destructors[i].class_name)
		out_file.write("    ASSERT_DESTRUCTOR(%s, %s);\n\n" % (destructors[i].class_name, message))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")
	
def test_function(out_file, vpl_out_file, entities):
	functions = entities["FUNCTION"]
	vpl_out_file.write("========\nFunction\n========\n")
	for i in range(len(functions)):
		out_file.write("TEST(Function, %s) {\n\n" % (functions[i].entity_name))
		vpl_out_file.write("REQUIRES=Function::%s\n" % (functions[i].entity_name))
		message_function = '"Funcția \'%s\' nu există sau nu s-a respectat signatura specificată în cerință!"' % (functions[i].entity_name)
		out_file.write("    ASSERT_FUNCTION(%s, %s, %s) {\n        /* insert code */\n    }\n\n" % (functions[i].entity_name, functions[i].template_postfix, message_function))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")

	
def test_inheritance(out_file, vpl_out_file, entities):
	inheritances = entities["INHERITANCE"]
	vpl_out_file.write("===========\nInheritance\n===========\n")
	for i in range(len(inheritances)):
		out_file.write("TEST(Inheritance, %s) {\n\n" % (inheritances[i].derived_class))
		vpl_out_file.write("REQUIRES=Inheritance::%s\n" % (inheritances[i].derived_class))
		message = '"Clasa \'%s\' nu este moștenită din clasa \'%s\'!"' % (inheritances[i].derived_class, inheritances[i].class_name)
		out_file.write("    ASSERT_INHERITANCE(%s, %s, %s);\n\n" % (inheritances[i].class_name, inheritances[i].derived_class, message))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")
	
def test_method(out_file, vpl_out_file, entities):
	methods = entities["METHOD"]
	vpl_out_file.write("======\nMethod\n======\n")
	method_dict = defaultdict(int)
	for i in range(len(methods)):
		dict_key = "%s%s" % (methods[i].class_name, methods[i].entity_name)
		if not dict_key in method_dict.keys():
			out_file.write("TEST(%sMethod, %s) {\n\n" % (methods[i].class_name, methods[i].entity_name))
			vpl_out_file.write("REQUIRES=%sMethod::%s\n" % (methods[i].class_name, methods[i].entity_name))
			method_dict[dict_key] = 2
		else:
			out_file.write("TEST(%sMethod, %s%d) {\n\n" % (methods[i].class_name, methods[i].entity_name, method_dict[dict_key]))
			vpl_out_file.write("REQUIRES=%sMethod::%s%d\n" % (methods[i].class_name, methods[i].entity_name, method_dict[dict_key]))
			method_dict[dict_key] += 1
		message_method = '"Clasa \'%s\' nu conține metoda \'%s\' sau nu s-a respectat signatura specificată în cerință!"' % (methods[i].class_name, methods[i].entity_name)
		out_file.write("    ASSERT_METHOD(%s, %s, %s, %s);\n\n" % (methods[i].class_name, methods[i].entity_name, methods[i].template_postfix, message_method))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")
	
def test_operator(out_file, vpl_out_file, entities):
	operators = entities["OPERATOR"]
	vpl_out_file.write("========\nOperator\n========\n")
	operator_dict = defaultdict(int)
	for i in range(len(operators)):
		operator_symbol = operators[i].entity_name[8:]
		operator_symbol_name = mapper[operator_symbol]
		operator_template_postfix = "operator" + operator_symbol_name
		dict_key = "%s%s" % (operators[i].class_name, operator_template_postfix)
		if not dict_key in operator_dict.keys():
			out_file.write("TEST(%sOperator, %s) {\n\n" % (operators[i].class_name, operator_template_postfix))
			vpl_out_file.write("REQUIRES=%sOperator::%s\n" % (operators[i].class_name, operator_template_postfix))
			operator_dict[dict_key] = 2
		else:
			out_file.write("TEST(%sOperator, %s%d) {\n\n" % (operators[i].class_name, operator_template_postfix, operator_dict[dict_key]))
			vpl_out_file.write("REQUIRES=%sOperator::%s%d\n" % (operators[i].class_name, operator_template_postfix, operator_dict[dict_key]))
			operator_dict[dict_key] += 1
		message_operator = '"Clasa \'%s\' nu conține operatorul \'%s\' sau nu s-a respectat signatura specificată în cerință!"' % (operators[i].class_name, operator_symbol)
		out_file.write("    ASSERT_OPERATOR(%s, %s, %s, %s);\n\n" % (operators[i].class_name, operator_template_postfix, operators[i].template_postfix, message_operator))
		out_file.write("}\n\n")
	vpl_out_file.write("\n")


def reflect_functions(out_file, entities):
	signatures = set()
	functions = entities["FUNCTION"]
	for i in range(len(functions)):
		signatures.add(functions[i])
	signatures = list(signatures)
	for i in range(len(signatures)):
		out_file.write("REGISTER_FUNCTION(%s, %s, %s, %s);\n" % (signatures[i].return_type, signatures[i].entity_name, signatures[i].args, signatures[i].template_postfix))
	out_file.write("\n")
	return entities
	
def reflect_classes(out_file, entities):
	attributes = entities["ATTRIBUTE"]
	methods = entities["METHOD"]
	operators = entities["OPERATOR"]
	classes = entities["CLASS"]

	for cls in classes:
		
		out_file.write("REGISTER_CLASS(%s) {\n" % (cls.class_name))
		
		unique_attributes = set()
		for attribute in attributes:
			if (cls.class_name == attribute.class_name):
				unique_attributes.add(attribute.entity_name)
		unique_attributes = list(unique_attributes)
		
		unique_methods = set()
		for method in methods:
			if (cls.class_name == method.class_name):
				unique_methods.add(method.entity_name)
		unique_methods = list(unique_methods)
		
		unique_operators = set()
		for operator in operators:
			if (cls.class_name == operator.class_name):
				unique_operators.add(operator.entity_name)
		unique_operators = list(unique_operators)
		
		for attribute in attributes:
			if (cls.class_name == attribute.class_name):
				if attribute.entity_name in unique_attributes:
					out_file.write("    REGISTER_ATTRIBUTE(%s, %s, %s);\n" % (attribute.entity_name, attribute.attr_type, attribute.template_postfix))
					unique_attributes.remove(attribute.entity_name)
				else:
					out_file.write("    REGISTER_SAME_ATTRIBUTE(%s, %s, %s);\n" % (attribute.entity_name, attribute.attr_type, attribute.template_postfix))
		for method in methods:
			if (cls.class_name == method.class_name):
				if method.entity_name in unique_methods:
					out_file.write("    REGISTER_METHOD(%s, %s(Class::*)(%s)%s, %s);\n" % (method.entity_name, method.return_type, method.args, method.const_qualified, method.template_postfix))
					unique_methods.remove(method.entity_name)
				else:
					out_file.write("    REGISTER_SAME_METHOD(%s, %s(Class::*)(%s)%s, %s);\n" % (method.entity_name, method.return_type, method.args, method.const_qualified, method.template_postfix))
		for operator in operators:
			if (cls.class_name == operator.class_name):
				operator_identifier = "operator" + mapper[operator.entity_name[8:]]
				if operator.entity_name in unique_operators:
					out_file.write("    REGISTER_OPERATOR(%s, %s, %s(Class::*)(%s)%s, %s);\n" % (operator.entity_name, operator_identifier, operator.return_type, operator.args, operator.const_qualified, operator.template_postfix))
					unique_operators.remove(operator.entity_name)
				else:
					out_file.write("    REGISTER_SAME_OPERATOR(%s, %s, %s(Class::*)(%s)%s, %s);\n" % (operator.entity_name, operator_identifier, operator.return_type, operator.args, operator.const_qualified, operator.template_postfix))
		
		out_file.write("};\n\n")
	
	return entities


FILE_EXTRACTOR = "extract.conf"
TEST_FILE = "tests.h"
VPL_TEST_FILE = "vpl_evaluate.cases"

entities = {
	"ABSTRACT": list(),
	"ATTRIBUTE": list(),
	"CLASS": list(),
	"CONSTRUCTOR": list(),
	"DESTRUCTOR": list(),
	"FUNCTION": list(),
	"INHERITANCE": list(),
	"METHOD": list(),
	"OPERATOR": list()
}

class Object:
	
	def __init__(self, test_name, class_name=None, derived_class=None, attr_type=None, entity_name=None, args=None, return_type=None, template_postfix=None, const_qualified=None):
		self.test_name = test_name
		self.class_name = class_name
		self.derived_class = derived_class
		self.attr_type = attr_type
		self.entity_name = entity_name
		self.args = args
		self.return_type = return_type
		self.template_postfix = template_postfix
		self.const_qualified=const_qualified

	def __cmp__(self):
		return object.__cmp__(self)

	def __eq__(self, rhs):
		return (self.test_name == rhs.test_name) and \
			(self.derived_class == rhs.derived_class) and \
			(self.attr_type == rhs.attr_type) and \
			(self.entity_name == rhs.entity_name) and \
			(self.args == rhs.args) and \
			(self.return_type == rhs.return_type)
		
	def __hash__(self):
		if (self.test_name == "ATTRIBUTE"):
			return hash(self.entity_name + " " + self.attr_type + " T::*")
		elif (self.test_name in ["METHOD", "OPERATOR", "FUNCTION"]):
			return hash(self.entity_name + " " + self.return_type + " (T::*)(" + self.args + ")")
		else:
			return hash(self.class_name)

input_file = open(FILE_EXTRACTOR, "r")
lines = filter(lambda string: len(string) != 0, input_file.readlines())

for line in lines:

    line = line.split(":-:")
    test = line[0].strip()
    content = line[1].strip()

    components = dict(map(lambda string: tuple(string.split(":=:")), list(map(lambda string: string.strip(), content.split("|")))))

    class_name = components.get("CLASS")
    derived_class = components.get("DERIVED")
    attr_type = components.get("TYPE")
    entity_name = components.get("NAME")
    args = components.get("ARGS")
    return_type = components.get("RETURN")
    const_qualified = " const" if components.get("CONST") == "True" else ""
    template_postfix = get_random_string(8)

    if args == "None":
        args = ""

    entities[test].append(Object(test_name=test, class_name=class_name, derived_class=derived_class, attr_type=attr_type, \
								 entity_name=entity_name, args=args, return_type=return_type, template_postfix=template_postfix, const_qualified=const_qualified))
		
test_file = open(TEST_FILE, 'a')
vpl_test_file = open(VPL_TEST_FILE, 'a')
vpl_test_file.truncate(0)

test_file.write("/**\n \
 * Unit Test Framework for compile time checking in VPL\n \
 * @Copyright (C) 2020 Andrei-Edward Popa\n \
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>\n \
 */\n\n")

entities = reflect_classes(test_file, entities)
entities = reflect_functions(test_file, entities)
test_seg_fault(test_file)
test_function(test_file, vpl_test_file, entities)
test_class(test_file, vpl_test_file, entities)
test_destructor(test_file, vpl_test_file, entities)
test_abstract(test_file, vpl_test_file, entities)
test_inheritance(test_file, vpl_test_file, entities)
test_attribute(test_file, vpl_test_file, entities)
test_method(test_file, vpl_test_file, entities)
test_operator(test_file, vpl_test_file, entities)
test_constructor(test_file, vpl_test_file, entities)

test_file.close()

test_file = open(TEST_FILE, 'r')
test_file_data = test_file.read()
test_file.close()

test_file = open(TEST_FILE, 'a')
test_file.truncate(0)

matches = re.findall("<.*>", test_file_data);
for i in range(len(matches)):
	test_file_data = test_file_data.replace(matches[i], matches[i].replace(",", " COMMA"))
	
test_file.write(test_file_data)
test_file.close()

