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

        elif self.code[self.position] == "<" and self.code[self.position+1] != "?":
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

        elif self.code[self.position] == ",":
            self.actual = Token("VIRGULOKO", ",")
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

        elif self.code[self.position] == "<" or self.code[self.position] == "?":
            php_tags = ""
            while self.position < len(self.code) and (self.code[self.position].isalnum() or self.code[self.position] == "$" or self.code[self.position] == "?" or self.code[self.position] == "<" or self.code[self.position] == ">"):
                php_tags += str(self.code[self.position])
                self.position += 1
            reserved_words = ["<?php", "?>"]
            if php_tags in reserved_words:
                self.actual = Token(php_tags, php_tags)
            else:
                #print(php_tags)
                raise EnvironmentError()

        elif self.code[self.position].isalpha() or self.code[self.position] == "$":
            VAR_token = ""
            while self.position < len(self.code) and (self.code[self.position].isalnum() or self.code[self.position] == "$"):
                VAR_token += str(self.code[self.position]).upper()
                self.position += 1

            reserved_words = ["ECHO", "WHILE", "IF", "ELSE", "READLINE", "TRUE", "FALSE", "AND", "OR", "NOT", "FUNCTION", "RETURN"]
            if VAR_token in reserved_words:
                self.actual = Token(VAR_token, VAR_token)
            else:
                self.actual = Token("VAR", VAR_token)
        else:
            raise EnvironmentError("Unknown Token")