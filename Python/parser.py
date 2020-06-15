from node import *
from symbol_table import SymbolTable
from lexer import Tokenizer
import re

def pre(code):
        filtered = re.sub("\n", "", code)
        return re.sub(r"(\/\*(.*?)\*\/)", '', filtered) 

class Parser:

    @staticmethod
    def parseProgram():
        commands = []


        #print(Parser.tokens.actual.value)
        if Parser.tokens.actual.value == "<?php":
            Parser.tokens.selectNext()
            while Parser.tokens.actual.value != "?>":
                #print(Parser.tokens.actual.value)
                commands.append(Parser.parseCommand())
                
        else:
            raise EnvironmentError()
        Parser.tokens.selectNext()
        return Program('prg', commands)


    @staticmethod
    def parseBlock(name='prg'):
        commands = []



        if Parser.tokens.actual.value == "{":
            Parser.tokens.selectNext()
            while Parser.tokens.actual.value != "}":
                commands.append(Parser.parseCommand())
                
        else:
            raise EnvironmentError(Parser.tokens.actual.value)
        Parser.tokens.selectNext()
        return Program(name, commands)

        

    @staticmethod
    def parseCommand(name='prg'):

        #print("COMMAND" + Parser.tokens.actual.value)
        if Parser.tokens.actual.type == "VAR":
            VAR = Indentifier(Parser.tokens.actual.value)
            identi = Parser.tokens.actual.value
            Parser.tokens.selectNext()

            if Parser.tokens.actual.type == "EQUAL":
                Parser.tokens.selectNext()
                
                output = Assigment("=", [VAR, Parser.parseRelExpression()])
                #print(output.children[1].value)
                if (Parser.tokens.actual.type == "COMMANDEND"):
                    Parser.tokens.selectNext()
                    return output
                else:
                  raise EnvironmentError()
            elif Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                args = []
                while Parser.tokens.actual.value != ")":
                    if Parser.tokens.actual.type == "VAR":
                        #print(Parser.tokens.actual.value)
                        args.append(Indentifier(Parser.tokens.actual.value))
                        Parser.tokens.selectNext()
                        if Parser.tokens.actual.value == ",":
                            Parser.tokens.selectNext()
                    elif Parser.tokens.actual.type == "INT":
                        #print(Parser.tokens.actual.value)
                        args.append(IntVal(Parser.tokens.actual.value))
                        Parser.tokens.selectNext()
                        if Parser.tokens.actual.value == ",":
                            Parser.tokens.selectNext()
                    else:
                        #print(Parser.tokens.actual.type)
                        raise EnvironmentError()
                Parser.tokens.selectNext()
                #Parser.tokens.selectNext() #CHECK O PUNTU E VIRGULU
                #print("ARGGGG=", args)
                output = FuncCall(identi, args)
                #print("VITORIA=", output)
                return output
                #raise EnvironmentError() MOTIVATION OF TESTS ONLY
            else:
                #print(Parser.tokens.actual.value)
                raise EnvironmentError()
        elif Parser.tokens.actual.type == "FUNCTION":
            Parser.tokens.selectNext()
            if Parser.tokens.actual.type == "VAR":
                args = []
                func_name = Parser.tokens.actual.value
                Parser.tokens.selectNext()
                if Parser.tokens.actual.value == "(":
                    Parser.tokens.selectNext()
                    while Parser.tokens.actual.value != ")":
                        if Parser.tokens.actual.type == "VAR":
                            args.append(Indentifier(Parser.tokens.actual.value))
                            Parser.tokens.selectNext()
                            if Parser.tokens.actual.value == ",":
                                Parser.tokens.selectNext()
                        else:
                            raise EnvironmentError()
                    Parser.tokens.selectNext()
                    command_func = Parser.parseCommand(name="FUN :(")
                    args.append(command_func)

                    output = FuncDec(func_name, args)
                    #print(output.children)
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
                #print(Parser.tokens.actual.type)
                raise EnvironmentError()

        elif Parser.tokens.actual.type == "RETURN":
            Parser.tokens.selectNext()
            
            output = Return('ECHO', [Parser.parseRelExpression()])
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
                    else:
                        command_else = NoOp()
                    
                    return If("IF", [rel_exp, command_if, command_else])

                else:
                    #print(Parser.tokens.actual.type)
                    raise EnvironmentError()
            else:
               raise EnvironmentError() 
        elif (Parser.tokens.actual.type == "COMMANDEND" or Parser.tokens.actual.value == "}"):
            #print("SAIU UM NOOP" + Parser.tokens.actual.value)
            Parser.tokens.selectNext()
            return NoOp()
        else:
            #print("VAI ENTRAR BLOCk" + Parser.tokens.actual.value)
            return Parser.parseBlock(name=name)

    @staticmethod
    def parseExpression():
        output = Parser.parseTerm()

        while Parser.tokens.actual.value in ["+", "-", "OR", "."]:
            if Parser.tokens.actual.value == "+":
                Parser.tokens.selectNext()
                output = BinOp("+", [output, Parser.parseTerm()])

            elif Parser.tokens.actual.value == "-":
                Parser.tokens.selectNext()
                output = BinOp("-", [output, Parser.parseTerm()])

            elif Parser.tokens.actual.value == ".":
                Parser.tokens.selectNext()
                output = BinOp(".", [output, Parser.parseTerm()])

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
        
        elif Parser.tokens.actual.type == "STR":
            output = StrVal(Parser.tokens.actual.value)
            Parser.tokens.selectNext()

        
        elif Parser.tokens.actual.type == "VAR":
            identi = Parser.tokens.actual.value
            output = Indentifier(Parser.tokens.actual.value)
            Parser.tokens.selectNext()
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                args = []
                while Parser.tokens.actual.value != ")":
                    if Parser.tokens.actual.type == "VAR":
                        #print(Parser.tokens.actual.value)
                        args.append(Indentifier(Parser.tokens.actual.value))
                        Parser.tokens.selectNext()
                        if Parser.tokens.actual.value == ",":
                            Parser.tokens.selectNext()
                    elif Parser.tokens.actual.type == "INT":
                        #print(Parser.tokens.actual.value)
                        args.append(IntVal(Parser.tokens.actual.value))
                        Parser.tokens.selectNext()
                        if Parser.tokens.actual.value == ",":
                            Parser.tokens.selectNext()
                    else:
                        #print(Parser.tokens.actual.type)
                        raise EnvironmentError()
                Parser.tokens.selectNext()
                #Parser.tokens.selectNext() #CHECK O PUNTU E VIRGULU
                #print("ARGGGG=", args)
                output = FuncCall(identi, args)
                #print("VITORIA=", output)
                
                #raise EnvironmentError() MOTIVATION OF TESTS ONLY


        elif Parser.tokens.actual.type == "READLINE":
            output = Input("READLINE")
            Parser.tokens.selectNext()
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                if Parser.tokens.actual.value == ")":
                    Parser.tokens.selectNext()
                else:
                    raise EnvironmentError(Parser.tokens.actual.value)
            else:
                raise EnvironmentError()
            #print(Parser.tokens.actual.type)

        elif Parser.tokens.actual.type == "BRACKETS":
            if Parser.tokens.actual.value == "(":
                Parser.tokens.selectNext()
                output = Parser.parseRelExpression()

                if Parser.tokens.actual.value == ")":
                    Parser.tokens.selectNext()
                else:
                    
                    raise EnvironmentError(Parser.tokens.actual.value)
            else:
                raise EnvironmentError()

        elif Parser.tokens.actual.value in ["+", "-", "!"]:
            if Parser.tokens.actual.value == "+":
                Parser.tokens.selectNext()
                output = UnOp("+", [Parser.parseFactor()])

            elif Parser.tokens.actual.value == "-":
                Parser.tokens.selectNext()
                output = UnOp("-", [Parser.parseFactor()])

            elif Parser.tokens.actual.value == "!":
                Parser.tokens.selectNext()
                output = UnOp("!", [Parser.parseFactor()])

        elif Parser.tokens.actual.value in ["TRUE", "FALSE"]:
            output = BoolValue(Parser.tokens.actual.value)
            Parser.tokens.selectNext()

        else:
            raise EnvironmentError(Parser.tokens.actual.value)
        return output

    @staticmethod
    def parseRelExpression():
        output = Parser.parseExpression()

        while Parser.tokens.actual.value in ["==", ">", "<"]:
            if Parser.tokens.actual.value == "==":
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
        res = Parser.parseProgram()

        Parser.tokens.selectNext()
        if Parser.tokens.actual.value != "EOF":
            raise EnvironmentError("missing EOF")

        res.evaluate(st)
