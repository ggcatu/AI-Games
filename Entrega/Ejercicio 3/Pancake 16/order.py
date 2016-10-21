ordenado = "pancake16.txt"
desordenado = "e.txt"

with open(ordenado, "r") as f:
    ordenados = f.read()

ordenados = ordenados.split("\n")

with open(desordenado, "r") as f:
    desordenados = f.read()

desordenados = desordenados.split("\n")

with open("resultado.txt", "w+") as f:
    for x in ordenados:
        re = "NO ME CALCULARON: "+x+"\n"
        for j in desordenados:
            if x in j:
                re = j+"\n"
        f.write(re)
