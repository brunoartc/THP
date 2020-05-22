class SymbolTable():

    def __init__(self):
        self.table = {}

    def declare(self, variable_name, variable_type):

        if variable_name not in self.table.keys():
            self.table[variable_name] = [None, variable_type]
        else:
            raise EnvironmentError()

    def getter(self, variable_name):

        if variable_name in self.table.keys():
            return self.table[variable_name]
        else:
            raise EnvironmentError()

    def setter(self, variable_name, variable_value):
        self.table[variable_name] = [variable_value, "INT"]