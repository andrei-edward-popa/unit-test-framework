# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

import lxml.etree
import glob

FILENAME = "extract.conf"
PATH_TO_FILES = "xml/*.xml"
FUNCTION_EXTRACT = '//memberdef[@kind="function"]'
VARIABLE_EXTRACT = '//memberdef[@kind="variable"]'
CLASS_EXTRACT = '//compounddef[@kind="class"]'
DEFINITION_EXTRACT = './/definition/text()'
ARGS_EXTRACT = './/argsstring/text()'
TYPE_EXTRACT = './/type/text()'
TYPE_REF_EXTRACT = './/type/ref/text()'
COMPOUND_EXTRACT = './/compoundname/text()'
DERIVED_COMPOUND_EXTRACT = './/derivedcompoundref/text()'
DESTRUCTOR = "DESTRUCTOR:-:     CLASS=%s | ARGS=%s\n"
CONSTRUCTOR = "CONSTRUCTOR:-:    CLASS=%s | ARGS=%s\n"
ATTRIBUTE = "ATTRIBUTE:-:      TYPE=%s | NAME=%s | CLASS=%s\n"
METHOD = "METHOD:-:         NAME=%s | RETURN=%s | ARGS=%s | CLASS=%s\n"
FUNCTION = "FUNCTION:-:       NAME=%s | RETURN=%s | ARGS=%s\n"
OPERATOR = "OPERATOR:-:       NAME=%s | RETURN=%s | ARGS=%s | CLASS=%s\n"
ABSTRACT = "ABSTRACT:-:       CLASS=%s\n"
INHERITANCE = "INHERITANCE:-:    CLASS=%s | DERIVED=%s\n"
CLASS = "CLASS:-:          CLASS=%s\n"

out_file = open(FILENAME, "a+")
out_file.truncate(0)

for xml_file in glob.glob(PATH_TO_FILES):

    doxygen_file = open(xml_file, "r")
    xml = lxml.etree.parse(doxygen_file)
    
    for x in xml.xpath(FUNCTION_EXTRACT):
        args_arr = x.xpath(ARGS_EXTRACT)
        args = args_arr[0]
        is_const_qualified = 0
        has_const_override = args[args.find(")") + 1:].strip()
        if (len(has_const_override) != 0):
        	if (has_const_override) == "const":
        		args = args[:-5].strip()
        		is_const_qualified = 1
        	elif (has_const_override == "override"):
        		args = args[:-8].strip()
        	elif (has_const_override == "const override"):
        		args = args[:-14].strip()
        		is_const_qualified = 1
        if (args[0] == "(") and (args[len(args)-1] == ")"):
        	args = args[1:len(args)-1]
        if (len(args) == 0):
        	args = "None"
        definition_arr = x.xpath(DEFINITION_EXTRACT)
        definition = definition_arr[0]
        split_definition = definition.split(" ")
        if (len(split_definition) == 1):
        	class_name_and_entity_name = split_definition[0]
        	destructor_arr = class_name_and_entity_name.split("::~")
        	class_name_and_entity_name_arr = class_name_and_entity_name.split("::")
        	class_name = class_name_and_entity_name_arr[0]
        	if len(destructor_arr) == 2:
        		out_file.write(DESTRUCTOR % (class_name, args))
        	else:
        		out_file.write(CONSTRUCTOR % (class_name, args))
        elif (len(split_definition) > 1):
            return_type = " ".join(split_definition[:-1])
            class_name_and_entity_name = split_definition[-1]
            class_name_and_entity_name_arr = class_name_and_entity_name.split("::")
            if (len(class_name_and_entity_name_arr) == 1):
                function_name = class_name_and_entity_name_arr[0]
                if (function_name != "main"):
                	out_file.write(FUNCTION % (function_name, return_type, args))
            else:
                class_name = class_name_and_entity_name_arr[0]
                entity_name = class_name_and_entity_name_arr[1]
                if (args[-1] == "0"):
                    args = args[1:len(args)-3]
                    out_file.write(ABSTRACT % (class_name))
                    if (len(args) == 0):
                        args = "None"
                if (is_const_qualified == 1):
                	args = args + "const"
                if not (entity_name.isidentifier()):
                    out_file.write(OPERATOR % (entity_name, return_type, args, class_name))
                else:
                    out_file.write(METHOD % (entity_name, return_type, args, class_name))
                        
    for x in xml.xpath(VARIABLE_EXTRACT):
        definition_arr = x.xpath(DEFINITION_EXTRACT)
        definition = definition_arr[0]
        attribute_type_arr = x.xpath(TYPE_EXTRACT)
        attribute_type_ref_arr = x.xpath(TYPE_REF_EXTRACT)
        if (len(attribute_type_arr) != 0):
        	attribute_type = attribute_type_arr[0]
        else:
        	attribute_type = attribute_type_ref_arr[0]
        attribute_definition_arr = definition.split(" ")
        attribute_name_and_class_name = attribute_definition_arr[-1]
        attribute_name_and_class_name_arr = attribute_name_and_class_name.split("::")
        if len(attribute_name_and_class_name_arr) > 1:
        	attribute_name = attribute_name_and_class_name_arr[1]  
        	class_name = attribute_name_and_class_name_arr[0]
        	out_file.write(ATTRIBUTE % (attribute_type, attribute_name, class_name))
        
    for x in xml.xpath(CLASS_EXTRACT):  
        base_classes_arr = x.xpath(COMPOUND_EXTRACT)
        base_class = base_classes_arr[0]
        derived_classes_arr = x.xpath(DERIVED_COMPOUND_EXTRACT)
        for i in range(len(derived_classes_arr)):
        	derived_class = derived_classes_arr[i]
        	out_file.write(INHERITANCE % (base_class, derived_class))
        out_file.write(CLASS % (base_class))

out_file.close()
out_file = open(FILENAME, "r+")
lines = list(set(out_file.readlines()))
lines.sort()
out_file.close()
out_file = open(FILENAME, "a+") 
out_file.truncate(0) 
for i in range(len(lines)):
	out_file.write(lines[i])     
out_file.close()

