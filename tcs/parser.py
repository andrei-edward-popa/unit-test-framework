# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

import lxml.etree
import glob
import re

FILENAME                 =     "extract.conf"
PATH_TO_FILES            =     "xml/*.xml"
FUNCTION_EXTRACT         =     '//memberdef[@kind="function"]'
VARIABLE_EXTRACT         =     '//memberdef[@kind="variable"]'
CLASS_EXTRACT            =     '//compounddef[@kind="class"]'
STRUCT_EXTRACT            =     '//compounddef[@kind="struct"]'
DEFINITION_EXTRACT       =     './/definition/text()'
ARGS_EXTRACT             =     './/argsstring/text()'
TYPE_EXTRACT             =     './/type/text()'
TYPE_REF_EXTRACT         =     './/type/ref/text()'
COMPOUND_EXTRACT         =     './/compoundname/text()'
DERIVED_COMPOUND_EXTRACT =     './/derivedcompoundref/text()'
DESTRUCTOR               =     "DESTRUCTOR:-:     CLASS:=:%s | ARGS:=:%s\n"
CONSTRUCTOR              =     "CONSTRUCTOR:-:    CLASS:=:%s | ARGS:=:%s\n"
ATTRIBUTE                =     "ATTRIBUTE:-:      CLASS:=:%s | NAME:=:%s | TYPE:=:%s\n"
METHOD                   =     "METHOD:-:         CLASS:=:%s | NAME:=:%s | RETURN:=:%s | ARGS:=:%s | CONST:=:%s\n"
FUNCTION                 =     "FUNCTION:-:       NAME:=:%s | RETURN:=:%s | ARGS:=:%s\n"
OPERATOR                 =     "OPERATOR:-:       CLASS:=:%s | NAME:=:%s | RETURN:=:%s | ARGS:=:%s | CONST:=:%s\n"
ABSTRACT                 =     "ABSTRACT:-:       CLASS:=:%s\n"
INHERITANCE              =     "INHERITANCE:-:    CLASS:=:%s | DERIVED:=:%s\n"
CLASS                    =     "CLASS:-:          CLASS:=:%s\n"

def extract_arguments(args):
    args = args.split(",")
    for i in range(len(args)):
        s_arg = args[i].split("=")[0].split(" ")
        matches = re.finditer("(\*+|\&+)", s_arg[-1]);
        if not re.search("(\*+|\&+)", s_arg[-1]):
            s_arg.pop()
        for match in matches:
            s_arg[-1] = match.group(0)
        args[i] = " ".join(s_arg)
    args = ",".join(args)
    return args

out_file = open(FILENAME, "a+")
out_file.truncate(0)

# Iterate over xml files
for xml_file in glob.glob(PATH_TO_FILES):

    doxygen_file = open(xml_file, "r")
    xml = lxml.etree.parse(doxygen_file)

    # Extract constructors and destructors
    for x in xml.xpath(FUNCTION_EXTRACT):

        args = x.xpath(ARGS_EXTRACT)[0]
        defs = x.xpath(DEFINITION_EXTRACT)[0].split(" ")

        if len(defs) > 1:
            continue

        definition = defs[0]
        (class_name, entity_name) = ("x", "y")

        args = args.replace("(", "").replace(")", "").strip()

        splitted = definition.split("::")
        if len(splitted) != 2:
            continue

        (class_name, entity_name) = splitted
        is_constructor = (class_name == entity_name)
        is_destructor = (entity_name[0] == "~")

        if not is_constructor and not is_destructor:
            continue

        args = extract_arguments(args)

        if not args:
            args = "None"

        if is_constructor:
            out_file.write(CONSTRUCTOR % (class_name, args))
        if is_destructor:
            out_file.write(DESTRUCTOR % (class_name, args))

    # Extract functions
    for x in xml.xpath(FUNCTION_EXTRACT):

        args = x.xpath(ARGS_EXTRACT)[0]
        defs = x.xpath(DEFINITION_EXTRACT)[0].split(" ")

        if len(defs) == 1:
            continue

        args = args.replace("(", "").replace(")", "").strip()

        return_type = " ".join(defs[:-1])

        splitted = defs[-1].split("::")
        if len(splitted) != 1:
            continue;

        function_name = splitted[0]
        if function_name == "main":
            continue;

        args = extract_arguments(args)

        if not args:
            args = "None"

        out_file.write(FUNCTION % (function_name, return_type, args))

    # Extract methods, operators and if class is abstract
    for x in xml.xpath(FUNCTION_EXTRACT):

        args = x.xpath(ARGS_EXTRACT)[0]
        defs = x.xpath(DEFINITION_EXTRACT)[0].split(" ")
        qualifiers = ""

        matches = re.finditer("(\(.*\))(.*)", args);
        for match in matches:
            args = match.group(1)
            qualifiers = match.group(2)

        is_const = "True" if re.search("const", qualifiers) else "False"

        args = args.replace("(", "").replace(")", "").strip()

        if len(defs) == 0:
            continue

        return_type = " ".join(defs[:-1])
        return_type = re.sub("virtual\s*", "", return_type)
        splitted = defs[-1].split("::")

        if len(splitted) == 1:
            continue

        class_name = splitted[0]
        entity_name = splitted[1]

        if entity_name[0] == "~" or entity_name == class_name:
            continue

        if re.search("=\s*0", qualifiers):
            out_file.write(ABSTRACT % (class_name))

        args = extract_arguments(args)

        if not args:
            args = "None"

        if not entity_name.isidentifier():
            out_file.write(OPERATOR % (class_name, entity_name, return_type, args, is_const))
            continue
        out_file.write(METHOD % (class_name, entity_name, return_type, args, is_const))

    # Extract attributes
    for x in xml.xpath(VARIABLE_EXTRACT):

        defs = x.xpath(DEFINITION_EXTRACT)[0].split()

        attribute_type = " ".join(defs[:-1])
        splitted =  defs[-1].split("::")

        if len(splitted) == 1:
            continue

        class_name = splitted[0]
        attribute_name = re.sub("\[.*\]", "", splitted[1])

        out_file.write(ATTRIBUTE % (class_name, attribute_name, attribute_type))

    # Extract classes and inheritances
    for x in xml.xpath(CLASS_EXTRACT):
    
        base_class = x.xpath(COMPOUND_EXTRACT)[0]
        derived_classes = x.xpath(DERIVED_COMPOUND_EXTRACT)
        
        for i in range(len(derived_classes)):
            out_file.write(INHERITANCE % (base_class, derived_classes[i]))

        out_file.write(CLASS % (base_class))

    # Extract structs and inheritances
    for x in xml.xpath(STRUCT_EXTRACT):

        base_class = x.xpath(COMPOUND_EXTRACT)[0]
        derived_classes = x.xpath(DERIVED_COMPOUND_EXTRACT)

        for i in range(len(derived_classes)):
            out_file.write(INHERITANCE % (base_class, derived_classes[i]))

        out_file.write(CLASS % (base_class))

out_file.seek(0)
lines = list(set(out_file.readlines()))
lines.sort()
out_file.truncate(0)
for line in lines:
    out_file.write(line)
out_file.close()
