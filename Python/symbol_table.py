class SymbolTable():

    table = {}


    @staticmethod
    def set_new(key, value):
        if key not in SymbolTable.table.keys():
            SymbolTable.table[key] = value
        else:
            raise EnvironmentError("NAUM PODE, COME UM ABACATE")

    def __init__(self, name="Simple"):
        self.name = name
        self.table = {}


    

    def declare(self, variable_name, variable_type):

        if variable_name not in self.table.keys():
            self.table[variable_name] = [None, variable_type]
        else:
            raise EnvironmentError()

    def getter(self, variable_name):
        #print(variable_name)
        if variable_name in self.table.keys():
            return self.table[variable_name]
        else:
            #print(variable_name)
            raise EnvironmentError()

    def setter(self, variable_name, variable_value, variable_type = "INT"):
        self.table[variable_name] = [variable_value, variable_type]