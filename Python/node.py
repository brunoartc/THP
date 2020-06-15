asm = ["SYS_EXIT equ 1",
"SYS_READ equ 3",
"SYS_WRITE equ 4",
"STDIN equ 0",
"STDOUT equ 1",
"True equ 1",
"False equ 0",
"segment .data",
"segment .bss",
"res RESB 1",
"section .text",
"global _start",
"print:",
"PUSH EBP",
"MOV EBP, ESP",
"MOV EAX, [EBP+8]",
"XOR ESI, ESI",

"print_dec:",
"MOV EDX, 0",
"MOV EBX, 0x000A",
"DIV EBX",
"ADD EDX, '0'",
"PUSH EDX",
"INC ESI",
"CMP EAX, 0",
"JZ print_next",
"JMP print_dec",

"print_next:",
"CMP ESI, 0",
"JZ print_exit",
"DEC ESI",
"MOV EAX, SYS_WRITE",
"MOV EBX, STDOUT",
"POP ECX",
"MOV [res], ECX",
"MOV ECX, res",
"MOV EDX, 1",
"INT 0x80",
"JMP print_next",

"print_exit:",
"POP EBP",
"RET"
"binop_je:",
"JE binop_true",
"JMP binop_false",

"binop_jg:",
"JG binop_true",
"JMP binop_false",

"binop_jl:",
"JL binop_true",
"JMP binop_false",

"binop_false:",
"MOV EBX, False",
"JMP binop_exit",

"binop_true:",
"MOV EBX, True",

"binop_exit:",
"RET",


 "start:", "PUSH EBP", "MOV EBP, ESP"]
class Node():
    i = 0
    def __init__(self):
        self.value = None
        self.children = []
        self.id = Node.newID()

    def evaluate(self, st):
        pass

    @staticmethod
    def newID():
        Node.i += 1
        return Node.i

class BinOp(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        child_value = []
        child_type = []
        for i in range(len(self.children)):
            value = self.children[i].evaluate(st)
            child_value.append(value[0])
            child_type.append(value[1])
            if i == 1:
                asm.append("POP EAX")
            else:
                asm.append("PUSH EBX")


        

        if child_type[0] == "INT" and child_type[1] == "INT": #TODO CHANGE COMMENT NAMES
            if self.value == "-":
                asm.append("SUB EAX, EBX      ; Subtraction: {} - {}".format(child_value[0], child_value[1]))
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] - child_value[1], "INT")

            elif self.value == "+":
                asm.append("ADD EAX, EBX      ; Addition: {} + {}".format(child_value[0], child_value[1]))
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] + child_value[1], "INT")

            elif self.value == "*":
                asm.append("IMUL EBX          ; Multiplication: {} * {}".format(child_value[0], child_value[1]))
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] * child_value[1], "INT")

            elif self.value == "/":
                asm.append("IDIV EBX          ; Division: {} / {}".format(child_value[0], child_value[1]))
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] // child_value[1], "INT")

            elif self.value == ">":
                asm.append("CMP EAX, EBX      ; Greater-than: {} > {}".format(child_value[0], child_value[1]))
                asm.append("CALL binop_jg")
                asm.append("\n")
                return (child_value[0] > child_value[1], "BOOL")

            elif self.value == "<":
                asm.append("CMP EAX, EBX      ; Less-than: {} < {}".format(child_value[0], child_value[1]))
                asm.append("CALL binop_jl")
                asm.append("\n")
                return (child_value[0] < child_value[1], "BOOL")

            elif self.value == "=":
                asm.append("CMP EAX, EBX      ; Equal: {} == {}".format(child_value[0], child_value[1]))
                asm.append("CALL binop_je")
                asm.append("\n")
                return (child_value[0] == child_value[1], "BOOL")

            elif self.value == "OR":
                asm.append("OR EAX, EBX       ; Or: {} | {}".format(child_value[0], child_value[1]))
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] or child_value[1], "BOOL")

            elif self.value == "AND":
                asm.append("AND EAX, EBX")
                asm.append("MOV EBX, EAX")
                asm.append("\n")
                return (child_value[0] and child_value[1], "BOOL")
        else:
            raise EnvironmentError()

class UnOp(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        child_value = self.children[0].evaluate(st)[0]
        child_type = self.children[0].evaluate(st)[1]

        if child_type == "INT" and self.value == "-":
            return (-child_value, "INT")

        elif child_type == "INT" and self.value == "+":
            return (child_value, "INT")
            
        elif child_type == "BOOL" and self.value == "NOT":
            return (not child_value, "BOOL")
        else:
            raise EnvironmentError()

class IntVal(Node):
    def __init__(self, value):
        self.value = value
        self.id = Node.newID()

    def evaluate(self, st):
        asm.append("\n")
        
        
        asm.append("MOV EBX, {}".format(self.value))
        return (self.value, "INT")

class Var(Node):
    def __init__(self, value):
        self.value = value
        self.id = Node.newID()

    def evaluate(self, st):
        child_value, child_type, memloc = st.getter(self.value)
        asm.append("MOV EBX, [EBP-{}]".format(memloc))
        #asm.append("PUSH EBX")
        return child_value, child_type, memloc

class Equal(Node):

    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        if (not st.has_key(self.children[0].value)):
            asm.append("PUSH DWORD 0")
        else:
            pass
            #asm.append("PUSH EBX") VER ESSA C
        st.setter(self.children[0].value, self.children[1].evaluate(st)[0])
        
        child_value, child_type, memloc = st.getter(self.children[0].value)
        asm.append("MOV [EBP-{}], EBX ; value = {}".format(memloc, self.children[0].value))

class Program(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        for child in self.children:
            child.evaluate(st)

class Print(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        self.children[0].evaluate(st)
        asm.append("PUSH EBX")
        asm.append("CALL print")
        asm.append("POP EBX")

class While(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        asm.append("\n")
        asm.append("LOOP_{}:".format(self.id))
        value , relexp_type = self.children[0].evaluate(st)
        #print(relexp_type)
        if relexp_type == "BOOL":
            asm.append("CMP EBX, False")
            asm.append("JE EXIT_{}".format(self.id))
            for child in self.children[1].children:
                child.evaluate(st)
            asm.append("JMP LOOP_{}".format(self.id))
            asm.append("EXIT_{}:".format(self.id))
            asm.append("\n")
        else:
            raise EnvironmentError()

class If(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children
        self.id = Node.newID()

    def evaluate(self, st):
        relexp_value, relexp_type = self.children[0].evaluate(st)

        if relexp_type == "BOOL":
            if self.children[2] is not None:
                asm.append("CMP EBX, False")
                asm.append("JE ELSE_{}".format(self.id))
                self.children[1].evaluate(st)
                asm.append("JMP EXIT_{}".format(self.id))
                asm.append("ELSE_{}:".format(self.id))
                self.children[2].evaluate(st)
                asm.append("EXIT_{}:".format(self.id))
            else:
                asm.append("CMP EBX, False")
                asm.append("JE EXIT_{}".format(self.id))
                for child in self.children[1]:
                    child.evaluate(st)
                asm.append("EXIT_{}:".format(self.id))
        else:
            raise EnvironmentError()

class Input(Node):
    def __init__(self, value):
        self.value = value
        self.id = Node.newID()

    def evaluate(self, st):
        try:
            input_value = int(input("Input: "))
            asm.append("MOV EBX, {}".format(input_value))
            return (input_value, "INT")
        except:
            raise EnvironmentError()

class BoolValue(Node):
    def __init__(self, value):
        self.value = value
        self.id = Node.newID()

    def evaluate(self, st):
        if self.value == "TRUE":
            asm.append("\n")
            asm.append("MOV EBX, {}".format("True"))
            return (True, "BOOL")
        else:
            asm.append("MOV EBX, {}".format("False"))
            return (False, "BOOL")

#REMUVED DOE TO LOU - NOU UZI

class Type(Node):
    def __init__(self, value):
        self.value = value
        self.id = Node.newID()

    def evaluate(self, st):
        return self.value

class NoOp(Node):
    def __init__(self):
        pass

    def evaluate(self, st):
        pass