# Unit Test Framework for compile time checking in VPL
# @Copyright (C) 2020 Andrei-Edward Popa
# @Author Andrei-Edward Popa <andrei_edward.popa@upb.ro>

import lxml.etree
import glob

FILENAME                 =     "extract.conf"
PATH_TO_FILES            =     "xml/*.xml"
FUNCTION_EXTRACT         =     '//memberdef[@kind="function"]'
VARIABLE_EXTRACT         =     '//memberdef[@kind="variable"]'
CLASS_EXTRACT            =     '//compounddef[@kind="class"]'
DEFINITION_EXTRACT       =     './/definition/text()'
ARGS_EXTRACT             =     './/argsstring/text()'
TYPE_EXTRACT             =     './/type/text()'
TYPE_REF_EXTRACT         =     './/type/ref/text()'
COMPOUND_EXTRACT         =     './/compoundname/text()'
DERIVED_COMPOUND_EXTRACT =     './/derivedcompoundref/text()'
DESTRUCTOR               =     "DESTRUCTOR:-:     CLASS=%s | ARGS=%s\n"
CONSTRUCTOR              =     "CONSTRUCTOR:-:    CLASS=%s | ARGS=%s\n"
ATTRIBUTE                =     "ATTRIBUTE:-:      CLASS=%s | NAME=%s | TYPE=%s\n"
METHOD                   =     "METHOD:-:         CLASS=%s | NAME=%s | RETURN=%s | ARGS=%s | CONST=%s\n"
FUNCTION                 =     "FUNCTION:-:       NAME=%s | RETURN=%s | ARGS=%s\n"
OPERATOR                 =     "OPERATOR:-:       CLASS=%s | NAME=%s | RETURN=%s | ARGS=%s | CONST=%s\n"
ABSTRACT                 =     "ABSTRACT:-:       CLASS=%s\n"
INHERITANCE              =     "INHERITANCE:-:    CLASS=%s | DERIVED=%s\n"
CLASS                    =     "CLASS:-:          CLASS=%s\n"

out_file = open(FILENAME, "a+")
out_file.truncate(0)

for xml_file in glob.glob(PATH_TO_FILES):

    doxygen_file = open(xml_file, "r")
    xml = lxml.etree.parse(doxygen_file)
    
    
    
    for x in xml.xpath(FUNCTION_EXTRACT):
    
        args = x.xpath(ARGS_EXTRACT)[0]
        split_definition = x.xpath(DEFINITION_EXTRACT)[0].split(" ")
        
        args = args[1:len(args)-1] if (args[0] == "(") and (args[len(args)-1] == ")") else args
        args = "None " if (len(args) == 0 or args == "()=default" or args == "()=delete") else args
       
        class_name = ""
        type_name = ""
        
        for split_item in split_definition:
        	colon_colon_split = split_item.split("::")
        	if (len(colon_colon_split) == 2):
        		class_name = colon_colon_split[0]
        		type_name = colon_colon_split[1]
        		break
        		
        if (((class_name == type_name) or (class_name == type_name[1:])) and (class_name != "")):
        	parsed_args = args.split(",")
        	
        	for i in range(len(parsed_args)):
        		while parsed_args[i][-1].isalnum():
        			parsed_args[i] = parsed_args[i][:-1]
        		parsed_args[i] = parsed_args[i][:-1]
        		
        	params = ",".join(parsed_args)
        	
        	if (class_name == type_name):
        		out_file.write(CONSTRUCTOR % (class_name, params))
        	else:
        		out_file.write(DESTRUCTOR % (class_name, params))
        		
        		
        		
    for x in xml.xpath(FUNCTION_EXTRACT):
    
        args = x.xpath(ARGS_EXTRACT)[0]
        split_definition = x.xpath(DEFINITION_EXTRACT)[0].split(" ")
        
        args = args[1:len(args)-1] if (args[0] == "(") and (args[len(args)-1] == ")") else args
        args = "None " if (len(args) == 0) else args
        
        if (len(split_definition) > 0):
            return_type = " ".join(split_definition[:-1])
            class_name_and_entity_name_arr = split_definition[-1].split("::")
            
            if (len(class_name_and_entity_name_arr) == 1):
                function_name = class_name_and_entity_name_arr[0]
                
                if (function_name != "main"):
                    parsed_args = args.split(",")
                    
                    for i in range(len(parsed_args)):
                        while parsed_args[i][-1].isalnum():
                            parsed_args[i] = parsed_args[i][:-1]
                    
                    params = ",".join(parsed_args)
                    
                    out_file.write(FUNCTION % (function_name, return_type, params))
                    
                    
        		
    for x in xml.xpath(FUNCTION_EXTRACT):
    
        args = x.xpath(ARGS_EXTRACT)[0]
        split_definition = x.xpath(DEFINITION_EXTRACT)[0].split(" ")
        
        is_const_qualified = False
        closing_index = args.find(")");
        has_qualifiers = args[closing_index + 1:].strip()
        
        if (len(has_qualifiers) != 0):
        	is_const_qualified = has_qualifiers.find("const") != -1      		
        	args = args[:closing_index + 1] + args[closing_index + 1:].replace("const", "")
        	args = args[:closing_index + 1] + args[closing_index + 1:].replace("override", "")
        	args = args[:closing_index + 1] + args[closing_index + 1:].replace("noexcept", "")
        	args = args.strip()
        
        args = args[1:len(args)-1] if (args[0] == "(") and (args[len(args)-1] == ")") else args
        args = "None " if (len(args) == 0 or args == "()=default" or args == "()=delete") else args
        	
        
        if (len(split_definition) > 0):   
            return_type = " ".join(split_definition[:-1]).replace("virtual ", "")
            class_name_and_entity_name_arr = split_definition[-1].split("::")
            
            if (len(class_name_and_entity_name_arr) != 1):      
                class_name = class_name_and_entity_name_arr[0]
                entity_name = class_name_and_entity_name_arr[1]
                
                if (entity_name[0] == "~" or entity_name == class_name):
                    continue
                    
                if (args[-1] == "0"):
                    out_file.write(ABSTRACT % (class_name))
                        
                args = args[1:len(args)-3] if (args[-1] == "0") else args
                args = "None " if (len(args) == 0) else args
                
                parsed_args = args.split(",")
                
                for i in range(len(parsed_args)):
                    while parsed_args[i][-1].isalnum():
                        parsed_args[i] = parsed_args[i][:-1]
                
                params = ",".join(parsed_args)
                
                if not (entity_name.isidentifier()):
                    out_file.write(OPERATOR % (class_name, entity_name, return_type, params, "True" if is_const_qualified == 1 else "False"))
                else:
                    out_file.write(METHOD % (class_name, entity_name, return_type, params, "True" if is_const_qualified == 1 else "False"))
                    
                    
                        
    for x in xml.xpath(VARIABLE_EXTRACT):
    
        definition = x.xpath(DEFINITION_EXTRACT)[0]
        
        attribute_definition_arr = definition.split(" ")
        attribute_type = " ".join(attribute_definition_arr[:-1])
        attribute_name_and_class_name_arr =  attribute_definition_arr[-1].split("::")
        
        if len(attribute_name_and_class_name_arr) > 1:      
        	attribute_name = attribute_name_and_class_name_arr[1]  
        	class_name = attribute_name_and_class_name_arr[0]
        	
        	out_file.write(ATTRIBUTE % (class_name, attribute_name, attribute_type))
        	
        	
        
    for x in xml.xpath(CLASS_EXTRACT):
    
        base_class = x.xpath(COMPOUND_EXTRACT)[0]
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

