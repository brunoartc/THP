import re
y = open('last-result', 'r').read().replace('\n', '\x02')


pattern = re.compile("(&lt;\?php)(.*?)(\?&gt;)")
i = 0


for match in re.finditer(pattern, y):
        
        with open(f"{i}.php", "w") as file:
                file.write(match.group().replace("&lt;", "<").replace("&gt;", ">").replace("&quot;", "\"").replace('\x02', "\n"))
        i+=1


