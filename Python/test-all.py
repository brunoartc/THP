from main import main as runner
import os, sys

files = os.listdir("testes")
sys.argv.append("teste")
for i in files:
    if ".php" in i:
        sys.argv[1] = "testes/" + i
        try:
            runner()
        except Exception as e:
            print(f"Erro no teste {i} -> {e}")
