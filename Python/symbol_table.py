class SymbolTable():

    def __init__(self):
        self.actualmem = 0
        self.table = {}

    def declare(self, variable_name, variable_type):
        #DEPRECATED
        if variable_name not in self.table.keys():
            self.table[variable_name] = [None, variable_type, self.actualmem]
            self.actualmem += 4
        else:
            raise EnvironmentError()


    def has_key(self, variable_name):
        return variable_name in self.table.keys()

    def getter(self, variable_name):

        if variable_name in self.table.keys():
            return self.table[variable_name]
        else:
            raise EnvironmentError()

    def setter(self, variable_name, variable_value):
        if variable_name not in self.table.keys():
            self.actualmem += 4
            self.table[variable_name] = [variable_value, "INT", self.actualmem]
        else:
            self.table[variable_name] = [variable_value, "INT", self.table[variable_name][2]]
        