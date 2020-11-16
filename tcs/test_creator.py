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

def get_random_string(length):
    letters = string.ascii_lowercase
    result_string = "".join(random.choice(letters) for i in range(length))
    return result_string

def test_seg_fault(out_file):
	out_file.write("TEST(Segmentation, Fault) {\n    /* insert code */\n}\n\n")

def test_abstract(out_file, entities):
	abstracts = entities["ABSTRACT"]
	for i in range(len(abstracts)):
		out_file.write("TEST(%s, Abstract) {\n\n" % (abstracts[i].class_name))
		message = '"Clasa \'%s\' nu este abstractă!"' % (abstracts[i].class_name)
		out_file.write("    ASSERT_CLASS_ABSTRACT(%s, %s);\n\n" % (abstracts[i].class_name, message))
		out_file.write("}\n\n")

def test_attribute(out_file, entities):
	attributes = entities["ATTRIBUTE"]
	for i in range(len(attributes)):
		out_file.write("TEST(%s, Attribute_%s) {\n\n" % (attributes[i].class_name, attributes[i].entity_name))
		message_attribute = '"Clasa \'%s\' nu conține atributul \'%s\'!"' % (attributes[i].class_name, attributes[i].entity_name)
		message_signature = '"Clasa \'%s\' nu conține atributul \'%s\' cu tipul specificat în cerință!"' % (attributes[i].class_name, attributes[i].entity_name)
		out_file.write("    ASSERT_CLASS_ATTRIBUTE(%s, %s, %s);\n" % (attributes[i].class_name, attributes[i].entity_name, message_attribute))
		out_file.write("    ASSERT_CLASS_ATTRIBUTE_SIGNATURE(%s, %s, %s, %s);\n\n" % (attributes[i].class_name, attributes[i].entity_name, attributes[i].template_postfix, message_signature))
		out_file.write("}\n\n")

def test_class(out_file, entities):
	classes = entities["CLASS"]
	for i in range(len(classes)):
		out_file.write("TEST(%s, Existence) {\n\n" % (classes[i].class_name))
		message = '"Clasa \'%s\' nu există!"' % (classes[i].class_name)
		out_file.write("    ASSERT_CLASS(%s, %s);\n\n" % (classes[i].class_name, message))
		out_file.write("}\n\n")

def test_constructor(out_file, entities):
	constructors = entities["CONSTRUCTOR"]
	for i in range(len(constructors)):
		nr_params = len(constructors[i].args.split(",")) if constructors[i].args != "" else 0
		out_file.write("TEST(%s, Constructor_%s_Param) {\n\n" % (constructors[i].class_name, nr_params))
		message = '"Constructorul cu %s param. pentru clasa \'%s\' nu există!"' % (nr_params, constructors[i].class_name)
		out_file.write("    ASSERT_CLASS_CONSTRUCTOR(%s, %s, %s) BEGIN {\n        /* insert code */\n    } END\n\n" % (message, constructors[i].class_name, constructors[i].args))
		out_file.write("}\n\n")
	
def test_destructor(out_file, entities):
	destructors = entities["DESTRUCTOR"]
	for i in range(len(destructors)):
		out_file.write("TEST(%s, Destructor) {\n\n" % (destructors[i].class_name))
		message = '"Clasa \'%s\' nu conține un destructor!"' % (destructors[i].class_name)
		out_file.write("    ASSERT_CLASS_DESTRUCTOR(%s, %s);\n\n" % (message, destructors[i].class_name))
		out_file.write("}\n\n")
	
def test_function(out_file, entities):
	functions = entities["FUNCTION"]
	for i in range(len(functions)):
		out_file.write("TEST(Function, %s_%s) {\n\n" % (functions[i].entity_name, functions[i].template_postfix))
		message_function = '"Funcția \'%s\' nu există!"' % (functions[i].entity_name)
		message_signature = '"Funcția \'%s\' nu conține tipul specificat în cerință!"' % (functions[i].entity_name)
		out_file.write("    ASSERT_FUNCTION(%s, %s);\n" % (functions[i].entity_name, message_function))
		out_file.write("    ASSERT_FUNCTION_SIGNATURE(%s, %s, %s) BEGIN {\n        /* insert code */\n    } END\n\n" % (functions[i].entity_name, functions[i].template_postfix, message_signature))
		out_file.write("}\n\n")
	
def test_inheritance(out_file, entities):
	inheritances = entities["INHERITANCE"]
	for i in range(len(inheritances)):
		out_file.write("TEST(%s, Inheritance) {\n\n" % (inheritances[i].derived_class))
		message = '"Clasa \'%s\' nu este moștenită din clasa \'%s\'!"' % (inheritances[i].derived_class, inheritances[i].class_name)
		out_file.write("    ASSERT_CLASS_INHERITANCE(%s, %s, %s);\n\n" % (inheritances[i].class_name, inheritances[i].derived_class, message))
		out_file.write("}\n\n")
	
def test_method(out_file, entities):
	methods = entities["METHOD"]
	for i in range(len(methods)):
		out_file.write("TEST(%s, Method_%s) {\n\n" % (methods[i].class_name, methods[i].entity_name))
		message_method = '"Clasa \'%s\' nu conține metoda \'%s\'!"' % (methods[i].class_name, methods[i].entity_name)
		message_signature = '"Clasa \'%s\' nu conține metoda \'%s\' cu tipul specificat în cerință!"' % (methods[i].class_name, methods[i].entity_name)
		out_file.write("    ASSERT_CLASS_METHOD(%s, %s, %s);\n" % (methods[i].class_name, methods[i].entity_name, message_method))
		out_file.write("    ASSERT_CLASS_METHOD_SIGNATURE(%s, %s, %s, %s);\n\n" % (methods[i].class_name, methods[i].entity_name, methods[i].template_postfix, message_signature))
		out_file.write("}\n\n")
	
def test_operator(out_file, entities):
	operators = entities["OPERATOR"]
	for i in range(len(operators)):
		operator_symbol = operators[i].entity_name[8:]
		operator_symbol_name = mapper[operator_symbol]
		operator_template_postfix = "operator" + operator_symbol_name
		out_file.write("TEST(%s, %s) {\n\n" % (operators[i].class_name, operator_template_postfix))
		message_operator = '"Clasa \'%s\' nu conține operatorul \'%s\'!"' % (operators[i].class_name, operator_symbol)
		message_signature = '"Clasa \'%s\' nu conține operatorul \'%s\' cu tipul specificat în cerință!"' % (operators[i].class_name, operator_symbol)
		out_file.write("    ASSERT_CLASS_OPERATOR(%s, %s, %s);\n" % (operators[i].class_name, operator_template_postfix, message_operator))
		out_file.write("    ASSERT_CLASS_OPERATOR_SIGNATURE(%s, %s, %s, %s);\n\n" % (operators[i].class_name, operator_template_postfix, operators[i].template_postfix, message_signature))
		out_file.write("}\n\n")

def check_attributes(out_file, entities):
	unique_attributes = set()
	signatures = set()
	attributes = entities["ATTRIBUTE"]
	for i in range(len(attributes)):
		signatures.add(attributes[i])
		unique_attributes.add(attributes[i].entity_name)
		entities["ATTRIBUTE"][i].template_postfix = list(signatures)[list(signatures).index(entities["ATTRIBUTE"][i])].template_postfix
	unique_attributes = list(unique_attributes)
	signatures = list(signatures)
	for i in range(len(unique_attributes)):	
		out_file.write("CHECK_CLASS_ATTRIBUTE(%s);\n" % (unique_attributes[i]))
	out_file.write("\n")
	for i in range(len(signatures)):
		out_file.write("CHECK_CLASS_ATTRIBUTE_SIGNATURE(%s, %s T::*, %s);\n" % (signatures[i].entity_name, signatures[i].attr_type, signatures[i].template_postfix))
	out_file.write("\n")
	return entities

def check_classes(out_file, entities):
	classes = entities["CLASS"]
	for i in range(len(classes)):
		out_file.write("CLASS(%s);\n" % (classes[i].class_name))
	out_file.write("\n")
	return entities

def check_functions(out_file, entities):
	unique_functions = set()
	signatures = set()
	functions = entities["FUNCTION"]
	classes = entities["CLASS"]
	for i in range(len(functions)):
		unique_functions.add(functions[i].entity_name)
		signatures.add(functions[i])
		entities["FUNCTION"][i].template_postfix = list(signatures)[list(signatures).index(entities["FUNCTION"][i])].template_postfix
	unique_functions = list(unique_functions)
	signatures = list(signatures)
	for i in range(len(unique_functions)):
		out_file.write("CHECK_FUNCTION(%s);\n" % (unique_functions[i]))
	out_file.write("\n")
	for i in range(len(signatures)):
		for j in range(len(classes)):
			signatures[i].return_type = signatures[i].return_type.replace(classes[j].class_name, "__HACK__::" + classes[j].class_name)
			signatures[i].args = signatures[i].args.replace(classes[j].class_name, "__HACK__::" + classes[j].class_name)
		out_file.write("CHECK_FUNCTION_SIGNATURE(%s, %s (T::*)(%s), %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args, signatures[i].template_postfix))
	out_file.write("\n")
	return entities

def check_methods(out_file, entities):
	out_file.write("CHECK_CLASS_METHOD(size);\n")
	unique_methods = set()
	signatures = set()
	methods = entities["METHOD"]
	for i in range(len(methods)):
		signatures.add(methods[i])
		unique_methods.add(methods[i].entity_name)
		entities["METHOD"][i].template_postfix = list(signatures)[list(signatures).index(entities["METHOD"][i])].template_postfix
	unique_methods = list(unique_methods)
	signatures = list(signatures)
	for i in range(len(unique_methods)):
		out_file.write("CHECK_CLASS_METHOD(%s);\n" % (unique_methods[i]))
	out_file.write("\n")
	for i in range(len(signatures)):
		if (signatures[i].args[-5:] == "const"):
			out_file.write("CHECK_CLASS_METHOD_SIGNATURE(%s, %s (T::*)(%s) const, %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args, signatures[i].template_postfix))
		else:
			out_file.write("CHECK_CLASS_METHOD_SIGNATURE(%s, %s (T::*)(%s), %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args, signatures[i].template_postfix))
	out_file.write("\n")
	return entities
	
def check_operators(out_file, entities):
	unique_operators = set()
	signatures = set()
	operators = entities["OPERATOR"]
	for i in range(len(operators)):
		signatures.add(operators[i])
		unique_operators.add(operators[i].entity_name)
		entities["OPERATOR"][i].template_postfix = list(signatures)[list(signatures).index(entities["OPERATOR"][i])].template_postfix
	unique_operators = list(unique_operators)
	signatures = list(signatures)
	for i in range(len(unique_operators)):
		operator_template_postfix = "operator" + mapper[unique_operators[i][8:]]
		out_file.write("CHECK_CLASS_OPERATOR(%s, %s);\n" % (unique_operators[i], operator_template_postfix))
	out_file.write("\n")
	for i in range(len(signatures)):
		if (signatures[i].args[-5:] == "const"):
			out_file.write("CHECK_CLASS_OPERATOR_SIGNATURE(%s, %s (T::*)(%s) const, %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args, signatures[i].template_postfix))
		else:
			out_file.write("CHECK_CLASS_OPERATOR_SIGNATURE(%s, %s (T::*)(%s), %s);\n" % (signatures[i].entity_name, signatures[i].return_type, signatures[i].args, signatures[i].template_postfix))
	out_file.write("\n")
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
	
	def __init__(self, test_name, class_name=None, derived_class=None, attr_type=None, entity_name=None, args=None, return_type=None, template_postfix=None):
		self.test_name = test_name
		self.class_name = class_name
		self.derived_class = derived_class
		self.attr_type = attr_type
		self.entity_name = entity_name
		self.args = args
		self.return_type = return_type
		self.template_postfix = template_postfix

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
	template_postfix = get_random_string(8)
	entities[test].append(Object(test_name=test, class_name=class_name, derived_class=derived_class, attr_type=attr_type, \
								 entity_name=entity_name, args=args, return_type=return_type, template_postfix=template_postfix))	
		
test_file = open(TEST_FILE, 'a')
test_file.truncate(0)

test_file.write("/**\n \
 * Unit Test Framework for compile time checking in VPL\n \
 * @Copyright (C) 2020 Andrei-Edward Popa\n \
 * @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>\n \
 */\n\n")
entities = check_classes(test_file, entities)
entities = check_attributes(test_file, entities)
entities = check_methods(test_file, entities)
entities = check_operators(test_file, entities)
entities = check_functions(test_file, entities)
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
	
