#!/usr/bin/python3
import sys
import argparse
import xml.etree.ElementTree as el_tree
import re


#process the arguments
def arguments(argv):
	#help
	if("--help" in argv):
		if(len(argv) != 2):
			sys.stderr.write("wrong arguments\n")
			exception("parametr")
		else:
			print("Program nacte XML reprezentaci programu ze zadaneho souboru a tento program s vyuzitim standardniho vstupu a vystupu interpretuje")
			exit(0)
	#surce and input
	elif any("--source=" in s for s in argv) or any("--input=" in s for s in argv):
		arg = argparse.ArgumentParser()
		arg.add_argument('--source')
		arg.add_argument('--input')
		args=arg.parse_args()
		source_file = args.source
		input_file = args.input

		if((source_file == "" or source_file == None) and (input_file == "" or input_file == None)):
			sys.stderr.write("wrong arguments\n")
			exception("parametr")
		
		elif(source_file != "" and  source_file != None and input_file != "" and input_file != None):
			return source_file, input_file

		#ask for missing argument
		elif(source_file == "" or source_file == None):
			return None, input_file
		elif(input_file == "" or input_file == None):	
			return source_file, None

	
	else:
		sys.stderr.write("wrong arguments\n")
		exception("parametr")

def exception(value):
	switch = {
	"parametr":10,
	"input_file":11,
	"output_file":12,
	"xml_format":31,
	"xml_syntax":32,
	"semantic":52,
	"operands":53,
	"variable":54,
	"frame":55,
	"missing_value":56,
	"wrong_value":57,
	"string":58,
	"internal_error":99,
	}
	exit (switch.get(value))

#check the root element
def root_check(root):
	#check the tag and attributes
	if(root.tag != "program" or root.attrib["language"] != "IPPcode19"):
		exception("xml_syntax")

	for k in root.attrib:
		if k not in["language","name","description"]:
			exception("xml_syntax")

#check for instructions with the same values of order
def check_for_duplicates(orders):
	unique = set(orders)  
	for each in unique:  
		count = orders.count(each)  
		if count > 1:
			sys.stderr.write("duplicate order\n")
			exception("xml_syntax")

#check the instructions
def instruction_check(root):
	instructions = []
	orders = []
	for instruction in root:
		#check the atrributes
		if("order" in instruction.attrib and "opcode" in instruction.attrib):
			orders.append(int(instruction.attrib["order"]))
			#check the tag 
			if (instruction.tag =="instruction" and len(instruction.attrib) == 2):
				instructions.append(instruction)
			else:
				sys.stderr.write("wrong instruction\n")
				exception("xml_syntax")
		else:
			exception("xml_syntax")
	#sort instructions by order
	instructions.sort(key=lambda x: int(x.attrib["order"]))
	check_for_duplicates(orders)
	return instructions


#parse input xml
def xml_parse(source_file):	

	if(source_file == None):
		try:
			tree = el_tree.parse(sys.stdin)
		except:
			exception("xml_format")

	else:
		try:
			f=open(source_file, "r")
		except:
			exception("input_file")
		try:
			tree = el_tree.parse(source_file)
		except:
			exception("xml_format")

	root = tree.getroot()

	root_check(root)
	instructions_xml = instruction_check(root)
	instructions_object = []
	
	for instruction in instructions_xml:
		instructions_object.append(create_instruction_object(instruction))


	return instructions_object
#create instruction object from xml
def create_instruction_object(instruction):
	opcode = instruction.attrib["opcode"].lower()

	if(opcode == "move"):
		return move_instruction(instruction)
	elif(opcode == "createframe"):
		return createframe_instruction(instruction)
	elif(opcode == "pushframe"):
		return pushframe_instruction(instruction)	
	elif(opcode == "popframe"):
		return popframe_instruction(instruction)
	elif(opcode == "defvar"):
		return defvar_instruction(instruction)
	elif(opcode == "call"):
		return call_instruction(instruction)
	elif(opcode == "return"):
		return return_instruction(instruction)
	elif(opcode == "pushs"):
		return pushs_instruction(instruction)
	elif(opcode == "pops"):
		return pops_instruction(instruction)
	elif(opcode == "add"):
		return add_instruction(instruction)
	elif(opcode == "sub"):
		return sub_instruction(instruction)
	elif(opcode == "mul"):
		return mul_instruction(instruction)
	elif(opcode == "idiv"):
		return idiv_instruction(instruction)
	elif(opcode == "lt"):
		return lt_instruction(instruction)
	elif(opcode == "gt"):
		return gt_instruction(instruction)
	elif(opcode == "eq"):
		return eq_instruction(instruction)
	elif(opcode == "and"):
		return and_instruction(instruction)
	elif(opcode == "or"):
		return or_instruction(instruction)
	elif(opcode == "not"):
		return not_instruction(instruction)
	elif(opcode == "int2char"):
		return int2char_instruction(instruction)
	elif(opcode == "stri2int"):
		return stri2int_instruction(instruction)
	elif(opcode == "read"):
		return read_instruction(instruction)
	elif(opcode == "write"):
		return write_instruction(instruction)
	elif(opcode == "concat"):
		return concat_instruction(instruction)
	elif(opcode == "strlen"):
		return strlen_instruction(instruction)
	elif(opcode == "getchar"):
		return getchar_instruction(instruction)
	elif(opcode == "setchar"):
		return setchar_instruction(instruction)
	elif(opcode == "type"):
		return type_instruction(instruction)
	elif(opcode == "label"):
		return label_instruction(instruction)
	elif(opcode == "jump"):
		return jump_instruction(instruction)
	elif(opcode == "jumpifeq"):
		return jumpifeq_instruction(instruction)
	elif(opcode == "jumpifneq"):
		return jumpifneq_instruction(instruction)
	elif(opcode == "exit"):
		return exit_instruction(instruction)
	elif(opcode == "dprint"):
		return dprint_instruction(instruction)
	elif(opcode == "break"):
		return break_instruction(instruction)
	else:
		exception("xml_syntax")
#check argumkents of instruction
def check_args(instruction_xml, num_of_args):
	args = []
	for arg in instruction_xml:
		args.append(arg)
		

	if(len(args)!= num_of_args):
		exception("xml_syntax")
	args.sort(key=lambda x: x.tag)

	for i in range(0,len(args)):
		if(args[i].tag != "arg"+str(i+1)):
			exception("xml_syntax")

	return args

def check_int(value):
	return re.search("^(\+|\-)?\d+$",str(value))
def check_var(value):
	return re.search("^(TF|LF|GF)@((\w|-|[_$&%*!?])(\w|-|[_$&%*!?]|[0-9])*)$",str(value))
def check_bool(value):
	return re.search("^(true|false)$",str(value))
def check_string(value):
	return re.search("^(\w|[^(\s\\\)]|\d|\\\([0-9][0-9][0-9])|[()])*$",str(value))
def check_nil(value):
	return re.search("^nil$",str(value))
def check_label(value):
	return re.search("^((\w|-|[_$&%*!?])(\w|-|[_$&%*!?]|[0-9])*)$",str(value))
def check_type(value):
	return re.search("^(int|string|bool)$",str(value))
	
def check_type_and_value(argument, expected_type):
	if("type" not in argument.attrib):
		exception("xml_syntax")
	real_type = argument.attrib["type"]
	
	if(expected_type == "symbol"):
		if(real_type not in ["var","int","bool","string","nil"]):
			exception("xml_syntax")

	elif(expected_type != real_type):
		exception("xml_syntax")

	value = argument.text
	if(value == None):
		value = ""

		
	if(real_type == "int" and check_int(value)):
		return container(value = int(value), type = "int")
	elif(real_type == "var" and check_var(value)):
		return container(value, value = None, type = "var")
	elif(real_type == "bool" and check_bool(value)):
		return container(value = value == "true", type = "bool")
	elif(real_type == "string" and check_string(value)):
		return container(value = value, type = "string")
	elif(real_type == "nil" and check_nil(value)):
		return container(value = "nil", type = "nil")
	elif(real_type == "label" and check_label(value)):
		return container(value = value, type = "label")
	elif(real_type == "type" and check_type(value)):
		return container(value = value, type = "type")
	else:
		exception("xml_syntax")

#classes of instructions

class move_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		
	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)
		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")
		arg1_value.value = arg2_value
		arg1_value.type = arg2_type


class createframe_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 0)

	def execute(self, symbol_table,instruction_pointer):
		symbol_table.tf = {}

class pushframe_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 0)
	
	def execute(self, symbol_table,instruction_pointer):
		if(symbol_table.tf == None):
			exception("frame")

		for name, container in symbol_table.tf.items():
			new_name = re.sub("TF@","LF@",name)
			symbol_table.tf[new_name] = symbol_table.tf.pop(name)
			container.name = re.sub("TF@","LF@",container.name)

		symbol_table.stack_of_lfs.append(symbol_table.tf)
		symbol_table.tf = None

class popframe_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 0)

	def execute(self, symbol_table,instruction_pointer):
		if(len(symbol_table.stack_of_lfs) == 0):
			exception("frame")

		symbol_table.tf = symbol_table.stack_of_lfs.pop()

		for name, container in symbol_table.tf.items():
			new_name = re.sub("LF@","TF@",name)
			symbol_table.tf[new_name] = symbol_table.tf.pop(name)
			container.name = re.sub("LF@","TF@",container.name)

class defvar_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "var")
	
	def execute(self,symbol_table,instruction_pointer):
		symbol_table.insert_container(self.arg1)


class call_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "label")

	def execute(self,symbol_table,instruction_pointer):
		if(self.arg1.value not in instruction_pointer.labels):
			exception("semantic")
		instruction_pointer.return_value.append(instruction_pointer.value)
		instruction_pointer.value = instruction_pointer.labels[self.arg1.value]

class return_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 0)

	def execute(self,symbol_table,instruction_pointer):
		if(len(instruction_pointer.return_value)==0):
			exception("missing_value")

		instruction_pointer.value = instruction_pointer.return_value.pop()

class pushs_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		symbol_table.stack.append(self.arg1)


class pops_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "var")

	def execute(self,symbol_table,instruction_pointer):
		if(len(symbol_table.stack)==0):
			excpetion("missing_value")

		container_on_stack = symbol_table.stack.pop()
		symbol_table.get(self.arg1).value = container_on_stack.value
		symbol_table.get(self.arg1).type = container_on_stack.type

class add_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "int" or arg3_type != "int"):
			exception("operands")

		arg1_value.value = arg2_value + arg3_value
		arg1_value.type = "int"


class sub_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")	

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "int" or arg3_type != "int"):
			exception("operands")

		arg1_value.value = arg2_value - arg3_value
		arg1_value.type = "int"
class mul_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")	

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "int" or arg3_type != "int"):
			exception("operands")

		arg1_value.value = arg2_value * arg3_value
		arg1_value.type = "int"

class idiv_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "int" or arg3_type != "int"):
			exception("operands")
		if(arg3_value == 0):
			exception("wrong_value")

		arg1_value.value = arg2_value // arg3_value
		arg1_value.type = "int"

class lt_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["int","bool","string"]):
			exception("operands")

		arg1_value.value = arg2_value < arg3_value
		arg1_value.type = "bool"


class gt_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["int","bool","string"]):
			exception("operands")

		arg1_value.value = arg2_value > arg3_value
		arg1_value.type = "bool"

class eq_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type and arg2_type != "nil" and arg3_type != "nil"):
			exception("operands")
		if(arg2_type not in["int","bool","string","nil"]):
			exception("operands")

		arg1_value.value = arg2_value == arg3_value and arg2_type == arg3_type
		arg1_value.type = "bool"

class and_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["bool"]):
			exception("operands")

		arg1_value.value = arg2_value and arg3_value 
		arg1_value.type = "bool"

class or_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["bool"]):
			exception("operands")

		arg1_value.value = arg2_value or arg3_value 
		arg1_value.type = "bool"

class not_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(arg2_type not in["bool"]):
			exception("operands")

		arg1_value.value = not arg2_value 
		arg1_value.type = "bool"

class int2char_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(arg2_type not in["int"]):
			exception("operands")

		try:
			arg1_value.value = chr(arg2_value) 
			arg1_value.type = "string"	
		except:
			exception("string")

class stri2int_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "string" or arg3_type != "int"):
			exception("operands")
		
		if(arg3_value < 0 or arg3_value >= len(arg2_value)):
			exception("string")

		arg1_value.value = ord(arg2_value[arg3_value])
		arg1_value.type = "int"

class read_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "type")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)
		implicit_value = False
		
		if(input_file == None):
			try:
				input_data = input()
			except:
				implicit_value = True
		else:
			try:
					input_data =f.readline()
					input_data = input_data.strip('\n\r')
			except:
				implicit_value = True

		if(not implicit_value):
			if(self.arg2.value == "int" and check_int(input_data)):
				input_data = int(input_data)
			elif(self.arg2.value == "string" and check_string(input_data)):
				input_data = str(input_data)
			elif(self.arg2.value == "bool"):
				input_data = (input_data.lower() == "true")
			else:
				implicit_value = True
		
		if(implicit_value):
			if(self.arg2.value == "int"):
				input_data = 0
			elif(self.arg2.value == "string"):
				input_data = ""
			elif(self.arg2.value == "bool"):
				input_data = False

		arg1_value.value = input_data
		arg1_value.type = self.arg2.value


		
		
class write_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		if(self.arg1.type == "var"):
			arg1_value = symbol_table.get(self.arg1)
		else:
			arg1_value = self.arg1
		
		if(arg1_value.type == "int"):
			print(arg1_value.value, end = "")
		elif(arg1_value.type == "string"):
			strg = re.sub("\\\(\d\d\d)",lambda x: chr(int(x.group(1))),arg1_value.value)
			print(strg,end="")
		elif(arg1_value.type == "bool"):
			if(arg1_value.value):
				print("true", end = "")
			else:
				print("false", end = "")

		elif(arg1_value.type == "nil"):
			print("", end = "")
class concat_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["string"]):
			exception("operands")

		arg1_value.value = arg2_value + arg3_value 
		arg1_value.type = "string"
class strlen_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		
	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(arg2_type not in["string"]):
			exception("operands")

		arg1_value.value = len(arg2_value) 
		arg1_value.type = "int"	

class getchar_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != "string" or arg3_type != "int"):
			exception("operands")
		
		if(arg3_value < 0 or arg3_value >= len(arg2_value)):
			exception("string")

		arg1_value.value = str(arg2_value[arg3_value])
		arg1_value.type = "string"	

class setchar_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")
	
	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg1_value.type != "string"):
			exception("operands")

		if(arg2_type != "int" or arg3_type != "string"):
			exception("operands")
		
		if(arg2_value < 0 or arg2_value >= len(arg1_value.value)):
			exception("string")

		if(len(arg3_value)==0):
			exception("string")

		chars =list(arg1_value.value)
		chars[arg2_value] = str(arg3_value[0])
		arg1_value.value = str("".join(chars))
		arg1_value.type = "string"


class type_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 2)
		self.arg1 = check_type_and_value(args[0], "var")
		self.arg2 = check_type_and_value(args[1], "symbol")
		
	def execute(self,symbol_table,instruction_pointer):
		arg1_value = symbol_table.get(self.arg1)

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type

		if(arg2_value == None):
			arg1_value.value = ""

		else:
			arg1_value.value = arg2_type

		
class label_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "label")

	def execute(self,symbol_table,instruction_pointer):
		pass

class jump_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "label")
	
	def execute(self,symbol_table,instruction_pointer):
		if(self.arg1.value in instruction_pointer.labels.keys()):
			instruction_pointer.value = instruction_pointer.labels[self.arg1.value]
		else:
			exception("semantic")

class jumpifeq_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "label")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		

		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["int","bool","string","nil"]):
			exception("operands")

		if(arg2_value == arg3_value):
			if(self.arg1.value in instruction_pointer.labels.keys()):
				instruction_pointer.value = instruction_pointer.labels[self.arg1.value]
			else:
				exception("semantic")


class jumpifneq_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 3)
		self.arg1 = check_type_and_value(args[0], "label")
		self.arg2 = check_type_and_value(args[1], "symbol")
		self.arg3 = check_type_and_value(args[2], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		if(self.arg2.type == "var"):
			arg2_value = symbol_table.get(self.arg2).value
			arg2_type = symbol_table.get(self.arg2).type
		else:
			arg2_value = self.arg2.value
			arg2_type = self.arg2.type
		if(arg2_value == None):
			exception("missing_value")

		if(self.arg3.type == "var"):
			arg3_value = symbol_table.get(self.arg3).value
			arg3_type = symbol_table.get(self.arg3).type
		else:
			arg3_value = self.arg3.value
			arg3_type = self.arg3.type
		if(arg3_value == None):
			exception("missing_value")

		if(arg2_type != arg3_type):
			exception("operands")
		if(arg2_type not in["int","bool","string","nil"]):
			exception("operands")

		if(arg2_value != arg3_value):
			if(self.arg1.value in instruction_pointer.labels.keys()):
				instruction_pointer.value = instruction_pointer.labels[self.arg1.value]
			else:
				exception("semantic")

class exit_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		if(self.arg1.type == "var"):
			arg1_value = symbol_table.get(self.arg1)
		else:
			arg1_value = self.arg1

		if(arg1_value.type != "int"):
			exception("operands")
		if(arg1_value.value < 0 or arg1_value.value > 49):
			exception("wrong_value")	

		exit(arg1_value.value)



class dprint_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 1)
		self.arg1 = check_type_and_value(args[0], "symbol")

	def execute(self,symbol_table,instruction_pointer):
		if(self.arg1.type == "var"):
			arg1_value = symbol_table.get(self.arg1)
		else:
			arg1_value = self.arg1
		
		if(arg1_value.type == "int"):
			sys.stderr.write(arg1_value.value)
		elif(arg1_value.type == "string"):
			strg = re.sub("\\\(\d\d\d)",lambda x: chr(int(x.group(1))),arg1_value.value)
			sys.stderr.write(strg)
		elif(arg1_value.type == "bool"):
			if(arg1_value.value):
				sys.stderr.write("true")
			else:
				sys.stderr.write("false")
		elif(arg1_value.type == "nil"):
			sys.stderr.write("")

class break_instruction():
	def __init__(self,instruction_xml):
		args = check_args(instruction_xml, 0)
		
	def execute(self,symbol_table,instruction_pointer):
		sys.stderr.write("Instruction pointer: " + str(instruction_pointer.value) + "\n")
		sys.stderr.write("\n")
		
		sys.stderr.write("Global frame: \n")
		for key,value in symbol_table.gf.items():
			sys.stderr.write(str(key) + ": " + str(value.value) + "\n")
		sys.stderr.write("\n")

		if(symbol_table.tf != None):
			sys.stderr.write("Temporary frame: \n")
			for key,value in symbol_table.tf.items():
				sys.stderr.write(str(key) + ": " + str(value.value) + "\n")
			sys.stderr.write("\n")

		if(len(symbol_table.stack_of_lfs) != 0 and symbol_table.stack_of_lfs[-1] != None):
			sys.stderr.write("Local frame: \n")
			for key,value in symbol_table.stack_of_lfs[-1].items():
				sys.stderr.write(str(key) + ": " + str(value.value) + "\n")
			sys.stderr.write("\n")

#CLASSES FOR INTERPRETING:
class interpret():
	def __init__(self, instructions):
		self.symbol_table = symbol_table()#stores TF LF GF
		self.instruction_pointer = instruction_pointer()#determines which instruction will be executed
		self.instructions = instructions#stored program
		self.instruction_pointer.find_labels(self.instructions)


	def execute(self):
		while self.instruction_pointer.value < len(self.instructions):
			self.instructions[self.instruction_pointer.value].execute(self.symbol_table, self.instruction_pointer)
			self.instruction_pointer.increment()



		#for instruction in self.instructions:
		#	instruction.execute(self.symbol_table, self.instruction_pointer)
		#	self.instruction_pointer.increment

		
		#print(self.symbol_table.gf["GF@x"].value)
		#print(self.symbol_table.gf["GF@y"].value)
		#print(self.symbol_table.gf["GF@z"].value)
		#print(self.symbol_table.gf["GF@z"].type)

class container():
	def __init__(self,var = None, value = None, type = None):
		self.name = var 
		self.value = value #value and type added afterwards
		self.type = type




class symbol_table():
	def __init__(self):
		self.stack = []#for stack instructions
		self.gf = {} #dict
		self.stack_of_lfs = [] #array of dicts
		self.tf = None #dict

	def insert_container(self, container):
		frame = self.get_frame(container.name) 
		if (container.name in frame):
			exception("semantic")
		
		frame[container.name] = container
		
		

	def get_frame(self, variable):
		frame = variable.split("@")[0]
		
		if(frame == "TF"):
			if(self.tf == None):
				exception("frame")
			return self.tf
		
		elif(frame == "LF"):
			if(len(self.stack_of_lfs)== 0):
				exception("frame")
			return self.stack_of_lfs[-1]

		elif(frame == "GF"):
			return self.gf
	
	def get(self, container):
		frame = self.get_frame(container.name)
		if(frame is None):
			exception("frame")
		if(container.name not in frame.keys()):
			exception("variable")

		return frame[container.name]



		
	

class instruction_pointer():
	def __init__(self):
		self.value = 0
		self.labels = {}
		self.return_value = []
	def find_labels(self, program):
		index = 0
		for instruction in program:
			if (isinstance(instruction, label_instruction)):
				if (instruction.arg1.value  in self.labels.keys()):
					exception("semantic")
				self.labels[instruction.arg1.value] = index
			index += 1

	def increment(self):
		self.value += 1


source_file,input_file = arguments(sys.argv)

if(input_file != None):
	try:
		f = open(input_file)
	except:
		exception("input_file")

instructions = xml_parse(source_file)

interpret = interpret(instructions)
interpret.execute()



