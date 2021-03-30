# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

import random
import string

mapper = {
	'+': 'Plus', '-': 'Minus', '*': 'Star', '&': 'And', '~': 'Tilda', '!': 'Not', '++': 'PlusPlus', '--': 'MinusMinus', '()': 'Call', '->': 'Arrow',
	'/': 'Div', '%': 'Mod', '^': 'Hat', '|': "Or", '<': 'Less', '>': 'Greater', '<=': 'LessEqual', '>=': 'GreaterEqual', '<<': "Stdout", '>>': 'Stdin',
	'<<=': 'ShiftLeftEqual', '>>=': 'ShiftRightEqual', '==': 'Equal', '!=': 'NotEqual', '&&': 'AndAnd', '||': 'OrOr', '+=': 'PlusEqual', '-=': 'MinusEqual',
	'/=': 'DivEqual', '%=': 'ModEqual', '^=': 'HatEqual', '&=': 'AndEqual', '|=': 'OrEqual', '*=': 'StarEqual', '[]': 'Index', '->*': 'ArrowStar', 
	',': 'Comma', ' new': 'New', ' delete': 'Delete', ' new[]': 'NewArr', ' delete[]': 'DeleteArr'
}

def test_seg_fault(out_file):
	out_file.write("TEST(Segmentation, Fault) {\n    /* insert code */\n}\n\n")

def test_abstract(out_file, entities):
	abstracts = entities["ABSTRACT"]
	for i in range(len(abstracts)):
		out_file.write("TEST(Abstract, %s) {\n\n" % (abstracts[i].class_name))
		message = '"Clasa \'%s\' nu este abstractă!"' % (abstracts[i].class_name)
		out_file.write("    ASSERT_ABSTRACT(%s, %s);\n\n" % (abstracts[i].class_name, message))
		out_file.write("}\n\n")

def test_attribute(out_file, entities):
	attributes = entities["ATTRIBUTE"]
	for i in range(len(attributes)):
		out_file.write("TEST(%sAttribute, %s) {\n\n" % (attributes[i].class_name, attributes[i].entity_name))
		message = '"Clasa \'%s\' nu conține atributul \'%s\' sau tipul specificat în cerință este incorect!"' % (attributes[i].class_name, attributes[i].entity_name)
		out_file.write("    ASSERT_ATTRIBUTE(%s, %s, %s);\n" % (attributes[i].class_name, attributes[i].entity_name, message))
		out_file.write("}\n\n")

def test_class(out_file, entities):
	classes = entities["CLASS"]
	for i in range(len(classes)):
		out_file.write("TEST(Existence, %s) {\n\n" % (classes[i].class_name))
		message = '"Clasa \'%s\' nu există!"' % (classes[i].class_name)
		out_file.write("    ASSERT_CLASS(%s, %s);\n\n" % (classes[i].class_name, message))
		out_file.write("}\n\n")

def test_constructor(out_file, entities):
	constructors = entities["CONSTRUCTOR"]
	for i in range(len(constructors)):
		nr_params = len(constructors[i].args.split(",")) if constructors[i].args != "" else 0
		out_file.write("TEST(Constructor, %sParam_%s) {\n\n" % (nr_params, constructors[i].class_name))
		message = '"Constructorul cu %s param. pentru clasa \'%s\' nu există!"' % (nr_params, constructors[i].class_name)
		if (len(constructors[i].args.strip()) != 0):
			out_file.write("    ASSERT_CONSTRUCTOR(%s, %s, %s) BEGIN {\n        /* insert code */\n    } END\n\n" % (message, constructors[i].class_name, constructors[i].args))
		else:
			out_file.write("    ASSERT_CONSTRUCTOR(%s, %s) BEGIN {\n        /* insert code */\n    } END\n\n" % (message, constructors[i].class_name))
		out_file.write("}\n\n")
	
def test_destructor(out_file, entities):
	destructors = entities["DESTRUCTOR"]
	for i in range(len(destructors)):
		out_file.write("TEST(Destructor, %s) {\n\n" % (destructors[i].class_name))
		message = '"Clasa \'%s\' nu conține un destructor!"' % (destructors[i].class_name)
		out_file.write("    ASSERT_DESTRUCTOR(%s, %s);\n\n" % (message, destructors[i].class_name))
		out_file.write("}\n\n")
	
def test_function(out_file, entities):
	functions = entities["FUNCTION"]
	for i in range(len(functions)):
		out_file.write("TEST(Function, %s) {\n\n" % (functions[i].entity_name))
		message = '"Funcția \'%s\' nu există sau nu conține tipul specificat în cerință!"' % (functions[i].entity_name)
		out_file.write("    ASSERT_FUNCTION(%s, %s) BEGIN {\n        /* insert code */\n    } END\n\n" % (functions[i].entity_name, message))
		out_file.write("}\n\n")
	
def test_inheritance(out_file, entities):
	inheritances = entities["INHERITANCE"]
	for i in range(len(inheritances)):
		out_file.write("TEST(Inheritance, %s) {\n\n" % (inheritances[i].derived_class))
		message = '"Clasa \'%s\' nu este moștenită din clasa \'%s\'!"' % (inheritances[i].derived_class, inheritances[i].class_name)
		out_file.write("    ASSERT_INHERITANCE(%s, %s, %s);\n\n" % (inheritances[i].class_name, inheritances[i].derived_class, message))
		out_file.write("}\n\n")
	
def test_method(out_file, entities):
	methods = entities["METHOD"]
	for i in range(len(methods)):
		out_file.write("TEST(%sMethod, %s) {\n\n" % (methods[i].class_name, methods[i].entity_name))
		message = '"Clasa \'%s\' nu conține metoda \'%s\' sau tipul specificat în cerință este incorect!"' % (methods[i].class_name, methods[i].entity_name)
		out_file.write("    ASSERT_METHOD(%s, %s, %s);\n" % (methods[i].class_name, methods[i].entity_name, message))
		out_file.write("}\n\n")
	
def test_operator(out_file, entities):
	operators = entities["OPERATOR"]
	for i in range(len(operators)):
		operator_symbol = operators[i].entity_name[8:]
		operator_symbol_name = mapper[operator_symbol]
		operator_identifier = "operator" + operator_symbol_name
		out_file.write("TEST(%sOperator, %s) {\n\n" % (operators[i].class_name, operator_identifier))
		message = '"Clasa \'%s\' nu conține operatorul \'%s\' sau tipul specificat în cerință este incorect!"' % (operators[i].class_name, operator_identifier)
		out_file.write("    ASSERT_OPERATOR(%s, %s, %s);\n" % (operators[i].class_name, operator_identifier, message))
		out_file.write("}\n\n")

def reflect_functions(out_file, entities):
	signatures = set()
	functions = entities["FUNCTION"]
	classes = entities["CLASS"]
	for i in range(len(functions)):
		signatures.add(functions[i])
	signatures = list(signatures)
	for i in range(len(signatures)):
		for j in range(len(classes)):
			signatures[i].return_type = signatures[i].return_type.replace(classes[j].class_name, "GlobalFunctionsWrapper::" + classes[j].class_name)
			signatures[i].args = signatures[i].args.replace(classes[j].class_name, "GlobalFunctionsWrapper::" + classes[j].class_name)
		out_file.write("REFLECT_FUNCTION(%s, %s, %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args))
	out_file.write("\n")
	return entities
	
def reflect_classes(out_file, entities):
	attributes = entities["ATTRIBUTE"]
	methods = entities["METHOD"]
	operators = entities["OPERATOR"]
	classes = entities["CLASS"]
	
	for cls in classes:
		
		out_file.write("REFLECT_CLASS(%s) {\n" % (cls.class_name))
		
		for attribute in attributes:
			if (cls.class_name == attribute.class_name):
				out_file.write("    REFLECT_ATTRIBUTE(%s, %s);\n" % (attribute.entity_name, attribute.attr_type))
		for method in methods:
			if (cls.class_name == method.class_name):
				if (method.args.split(",")[-1].strip() == "const"):
					method.args = method.args[:method.args.rfind(",") + 1] + " /* Method Qualifiers */ " + method.args[method.args.rfind(",") + 1:]
				else:
					method.args = method.args + " /* Method Qualifiers */ "
				out_file.write("    REFLECT_METHOD(%s, %s, %s);\n" % (method.entity_name, method.return_type, method.args))
		for operator in operators:
			if (cls.class_name == operator.class_name):
				operator_identifier = "operator" + mapper[operator.entity_name[8:]]
				if (operator.args.split(",")[-1].strip() == "const"):
					operator.args = operator.args[:operator.args.rfind(",")] + " /* Operator Qualifiers */ " + operator.args[operator.args.rfind(","):]
				else:
					operator.args = operator.args + " /* Operator Qualifiers */ "
				out_file.write("    REFLECT_OPERATOR(%s, %s, %s, %s);\n" % (operator.entity_name, operator_identifier, operator.return_type, operator.args))
		
		out_file.write("};\n\n")
	
	return entities


FILE_EXTRACTOR = "extract.conf"
TEST_FILE = "tests.h"

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
	
	def __init__(self, test_name, class_name=None, derived_class=None, attr_type=None, entity_name=None, args=None, return_type=None):
		self.test_name = test_name
		self.class_name = class_name
		self.derived_class = derived_class
		self.attr_type = attr_type
		self.entity_name = entity_name
		self.args = args
		self.return_type = return_type

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

	split_line = line.split(":-:")
	test = split_line[0].strip()
	split_line[1] = split_line[1].replace("operator==", "operatorEqualEqualTemp")
	split_line[1] = split_line[1].replace("operator=", "operatorEqualTemp")
	components = dict(map(lambda string: tuple(string.split("=")), list(map(lambda string: string.strip(), split_line[1].strip().split("|")))))
	class_name = components.get("CLASS")
	derived_class = components.get("DERIVED")
	attr_type = components.get("TYPE")
	entity_name = components.get("NAME")
	if (entity_name != None):
		entity_name = entity_name.replace("operatorEqualEqualTemp", "operator==")
		entity_name = entity_name.replace("operatorEqualTemp", "operator=")
	args = components.get("ARGS")
	args = "" if (args == None or args == "None") else args
	return_type = components.get("RETURN")
	return_type = "" if (return_type == "None" or return_type == None) else return_type
	return_type = return_type.replace("virtual ", "")
	entities[test].append(Object(test_name=test, class_name=class_name, derived_class=derived_class, attr_type=attr_type, \
								 entity_name=entity_name, args=args, return_type=return_type))	
		
test_file = open(TEST_FILE, 'a')
test_file.truncate(0)

test_file.write("/**\n \
 * Unit Test Framework for compile time checking in VPL\n \
 * @Copyright (C) 2020 Andrei-Edward Popa\n \
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>\n \
 */\n\n")

entities = reflect_classes(test_file, entities)
entities = reflect_functions(test_file, entities)
test_seg_fault(test_file)
test_function(test_file, entities)
test_class(test_file, entities)
test_destructor(test_file, entities)
test_abstract(test_file, entities)
test_inheritance(test_file, entities)
test_attribute(test_file, entities)
test_method(test_file, entities)
test_operator(test_file, entities)
test_constructor(test_file, entities)

test_file.close()

