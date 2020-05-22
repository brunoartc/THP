class Node():
    def __init__(self):
        self.value = None
        self.children = []

    def evaluate(self, st):
        pass

class BinOp(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        children = [child.evaluate(st) for child in self.children]
        child_value = [child[0] for child in children]
        child_type = [child[1] for child in children]

        

        if child_type[0] == "INT" and child_type[1] == "INT":
            if self.value == "-":
                return (child_value[0] - child_value[1], "INT")
            elif self.value == "+":
                return (child_value[0] + child_value[1], "INT")
            elif self.value == "*":
                return (child_value[0] * child_value[1], "INT")
            elif self.value == "/":
                return (child_value[0] // child_value[1], "INT")
            elif self.value == ">":
                return (child_value[0] > child_value[1], "BOOL")
            elif self.value == "<":
                return (child_value[0] < child_value[1], "BOOL")
            elif self.value == "=":
                return (child_value[0] == child_value[1], "BOOL")
            elif self.value == "OR":
                return (child_value[0] or child_value[1], "BOOL")
            elif self.value == "AND":
                return (child_value[0] and child_value[1], "BOOL")
        else:
            raise EnvironmentError("")

class UnOp(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        child_value, child_type = self.children[0].evaluate(st)

        if child_type == "INT" and self.value == "-":
            return - child_value
        elif child_type == "INT" and self.value == "+":
            return child_value
        elif child_type == "BOOL" and self.value == "NOT":
            return not child_value
        else:
            raise EnvironmentError("")

class IntVal(Node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, st):
        return (self.value, "INT")

class Indentifier(Node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, st):
        return st.getter(self.value)

class Assigment(Node):

    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        st.setter(self.children[0].value, self.children[1].evaluate(st)[0])

class Program(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        for child in self.children:
            child.evaluate(st)

class Print(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        print(self.children[0].evaluate(st))

class While(Node):
    
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        value , relexp_type = self.children[0].evaluate(st)
        if relexp_type == "BOOL":
            while self.children[0].evaluate(st)[0]:
                for child in self.children[1].children:
                    child.evaluate(st)
        else:
            raise EnvironmentError("")

class If(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        relexp_value, relexp_type = self.children[0].evaluate(st)

        if relexp_type == "BOOL" or relexp_type == "INT":
            if relexp_value:
                self.children[1].evaluate(st)
            else:
                self.children[2].evaluate(st)
        else:
            raise EnvironmentError("")

class Input(Node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, st):
        try:
            return (int(input()), "INT")
        except:
            raise EnvironmentError("")

class BoolValue(Node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, st):
        if self.value == "TRUE":
            return (True, "BOOL")
        else:
            return (False, "BOOL")

class VarDec(Node):
    def __init__(self, value, children):
        self.value = value
        self.children = children

    def evaluate(self, st):
        st.declare(self.children[0].value, self.children[1].evaluate(st))

class Type(Node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, st):
        return self.value

class NoOp(Node):
    def __init__(self):
        pass

    def evaluate(self, st):
        pass