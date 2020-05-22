from parser import Parser
import sys

def main():
    try:
        filename = sys.argv[1]
    except IndexError:
        print("NO file input")
        sys.exit(1)

    with open (filename, 'r') as file:
        code = file.read()
    Parser.run(code)

if __name__ == "__main__":
    main()