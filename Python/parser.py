from node import *
from symbol_table import SymbolTable
import re



class Token:
    def __init__(self, token_type, value):
        self.type = token_type
        self.value = value

class Tokenizer:
    def __init__(self, code):
        self.code = code
        self.position = 0
        self.actual = Token("None", "None")

    def selectNext(self):
        if self.position == len(self.code):
            self.actual = Token("EOF", "EOF")
            return

        while self.code[self.position] == " ":
            self.position += 1

        if self.code[self.position] == "-":
            self.actual = Token("AUNOP", "-")
            self.position += 1

        elif self.code[self.position] == "+":
            self.actual = Token("AUNOP", "+")
            self.position += 1

        elif self.code[self.position] == "/":
            self.actual = Token("ABINOP", "/")
            self.position += 1

        elif self.code[self.position] == "*":
            self.actual = Token("ABINOP", "*")
            self.position += 1

        elif self.code[self.position] == ">":
            self.actual = Token("CMPBINOP", ">")
            self.position += 1

        elif self.code[self.position] == "<":
            self.actual = Token("CMPBINOP", "<")
            self.position += 1

        elif self.code[self.position] == "(":
            self.actual = Token("BRACKETS", "(")
            self.position += 1

        elif self.code[self.position] == ")":
            self.actual = Token("BRACKETS", ")")
            self.position += 1

        elif self.code[self.position] == "{":
            self.actual = Token("A_BRACKETS", "{")
            self.position += 1

        elif self.code[self.position] == "}":
            self.actual = Token("B_BRACKETS", "}")
            self.position += 1

        elif self.code[self.position] == "=":
            if self.code[self.position+1] == "=":
                self.actual = Token("EQUALCMP", "==")
                self.position += 2
            else:
                self.actual = Token("EQUAL", "=")
                self.position += 1

        elif self.code[self.position] == "\n":
            self.actual = Token("LINEFEED", "\n")
            self.position += 1

        elif self.code[self.position] == ";":
            self.actual = Token("COMMANDEND", ";")
            self.position += 1

        elif self.code[self.position].isdigit():
            int_token = ""
            while self.position < len(self.code) and self.code[self.position].isdigit():
                int_token += str(self.code[self.position])
                self.position += 1
            self.actual = Token("INT", int(int_token))

        elif self.code[self.position].isalpha() or self.code[self.position] == "$":
            VAR_token = ""
            while self.position < len(self.code) and (self.code[self.position].isalnum() or self.code[self.position] == "_" or self.code[self.position] == "$"):
                VAR_token += str(self.code[self.position]).upper()
                self.position += 1

            reserved_words = ["ECHO", "WHILE", "IF", "ELSE", "READLINE", "TRUE", "FALSE", "AND", "OR", "NOT"]
            if VAR_token in reserved_words:
                self.actual = Token(VAR_token, VAR_token)
            else:
                self.actual = Token("VAR", VAR_token)
        else:
            raise EnvironmentError("Unknown Token")



        

def pre(code):
        filtered = re.sub("\n", "", code)
        return re.sub(r"(\/\*(.*?)\*\/)", '', filtered) 

class Parser:
    @staticmethod
    def parseBlock():
        statements = []



        if Parser.tokens.actual.value == "{":
            Parser.tokens.selectNext()
            while Parser.tokens.actual.value != "}":
                statements.append(Parser.parseCommand())
                
        else:
            raise EnvironmentError()
        Parser.tokens.selectNext()
        return Program('prg', statements)

        

    @staticmethod
    def parseCommand():

        #print("COMMAND" + Parser.tokens.actual.value)
        if Parser.tokens.actual.type == "VAR":
            VAR = Var(Parser.tokens.actual.value)
            Parser.tokens.selectNext()


            if Parser.tokens.actual.type == "EQUAL":
                Parser.tokens.selectNext()
                
                output = Equal("=", [VAR, Parser.parseRelExpression()])
                #print(output.children[1].value)
                if (Parser.tokens.actual.type == "COMMANDEND"):
                    Parser.tokens.selectNext()
                    return output
                else:
                  raise EnvironmentError()
     
            else:
                raise EnvironmentError()
   
        elif Parser.tokens.actual.type == "ECHO":
            Parser.tokens.selectNext()
            
            output = Print('ECHO', [Parser.parseRelExpression()])
            if (Parser.tokens.actual.type == "COMMANDEND"):
                Parser.tokens.selectNext()
                #print("RETORNANDO ECHO" + Parser.tokens.actual.value)
                return output
            else:
                raise EnvironmentError()

        elif Parser.tokens.actual.type == "WHILE":
            Parser.tokens.selectNext()
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                rel_exp = Parser.parseRelExpression()
                if Parser.tokens.actual.value == ")":
                    Parser.tokens.selectNext()

                    command = Parser.parseCommand()

                    #print("RETORNANDO DO WHILE")
                    return While("WHILE", [rel_exp, command])

                else:
                    raise EnvironmentError()
            else:
               raise EnvironmentError() 

        elif Parser.tokens.actual.type == "IF":
            Parser.tokens.selectNext()
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                rel_exp = Parser.parseRelExpression()
                if Parser.tokens.actual.value == ")":
                    Parser.tokens.selectNext()

                    #print("TERMINOU REL EXP"+ Parser.tokens.actual.value)


                    
                    command_if = Parser.parseCommand()
                    command_else = None
                    #print("TERMINOU IF STAT"+ Parser.tokens.actual.value)
                    
                    if Parser.tokens.actual.type == "ELSE":
                        Parser.tokens.selectNext()
                        command_else = Parser.parseCommand()

                    
                    return If("IF", [rel_exp, command_if, command_else])

                else:
                    raise EnvironmentError()
            else:
               raise EnvironmentError() 
        elif (Parser.tokens.actual.type == "COMMANDEND" or Parser.tokens.actual.value == "}"):
            #print("SAIU UM NOOP" + Parser.tokens.actual.value)
            return NoOp()
        else:
            #print("VAI ENTRAR BLOCk" + Parser.tokens.actual.value)
            return Parser.parseBlock()

    @staticmethod
    def parseExpression():
        output = Parser.parseTerm()

        while Parser.tokens.actual.value in ["+", "-", "OR"]:
            if Parser.tokens.actual.value == "+":
                Parser.tokens.selectNext()
                output = BinOp("+", [output, Parser.parseTerm()])

            elif Parser.tokens.actual.value == "-":
                Parser.tokens.selectNext()
                output = BinOp("-", [output, Parser.parseTerm()])

            elif Parser.tokens.actual.value == "OR":
                Parser.tokens.selectNext()
                output = BinOp("OR", [output, Parser.parseTerm()])
        return output

    @staticmethod
    def parseTerm():
        output = Parser.parseFactor()

        while Parser.tokens.actual.value in ["*", "/", "AND"]:
            if Parser.tokens.actual.value == "*":
                Parser.tokens.selectNext()
                output = BinOp("*", [output, Parser.parseFactor()])

            elif Parser.tokens.actual.value == "/":
                Parser.tokens.selectNext()
                output = BinOp("/", [output, Parser.parseFactor()])

            elif Parser.tokens.actual.value == "AND":
                Parser.tokens.selectNext()
                output = BinOp("AND", [output, Parser.parseFactor()])
        return output

    @staticmethod
    def parseFactor():
        output = 0

        if Parser.tokens.actual.type == "INT":
            output = IntVal(Parser.tokens.actual.value)
            Parser.tokens.selectNext()

        elif Parser.tokens.actual.type == "VAR":
            output = Var(Parser.tokens.actual.value)
            Parser.tokens.selectNext()

        elif Parser.tokens.actual.type == "READLINE":
            output = Input("READLINE")
            Parser.tokens.selectNext()

        elif Parser.tokens.actual.type == "BRACKETS":
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                output = Parser.parseExpression()

                if Parser.tokens.actual.value == ")":
                    Parser.tokens.selectNext()
                else:
                    raise EnvironmentError()
            else:
                raise EnvironmentError()

        elif Parser.tokens.actual.value in ["+", "-", "NOT"]:
            if Parser.tokens.actual.value == "+":
                Parser.tokens.selectNext()
                output = UnOp("+", [Parser.parseFactor()])

            elif Parser.tokens.actual.value == "-":
                Parser.tokens.selectNext()
                output = UnOp("-", [Parser.parseFactor()])

            elif Parser.tokens.actual.value == "NOT":
                Parser.tokens.selectNext()
                output = UnOp("NOT", [Parser.parseFactor()])

        elif Parser.tokens.actual.value in ["TRUE", "FALSE"]:
            output = BoolValue(Parser.tokens.actual.value)
            Parser.tokens.selectNext()

        else:
            raise EnvironmentError()
        return output

    @staticmethod
    def parseRelExpression():
        output = Parser.parseExpression()

        while Parser.tokens.actual.value in ["=", ">", "<"]:
            if Parser.tokens.actual.value == "=":
                Parser.tokens.selectNext()
                output = BinOp("=", [output, Parser.parseExpression()])

            elif Parser.tokens.actual.value == ">":
                Parser.tokens.selectNext()
                output = BinOp(">", [output, Parser.parseExpression()])

            elif Parser.tokens.actual.value == "<":
                Parser.tokens.selectNext()
                output = BinOp("<", [output, Parser.parseExpression()])
        return output

    @staticmethod
    def run(code):
        st = SymbolTable()
        Parser.tokens = Tokenizer(pre(code))
        Parser.tokens.selectNext()
        res = Parser.parseBlock()

        Parser.tokens.selectNext()
        if Parser.tokens.actual.value != "EOF":
            raise EnvironmentError("missing EOF")

        res.evaluate(st)

        print("\n".join(asm) + "POP EBP\nMOV EAX, 1\nINT 0x80\n")
