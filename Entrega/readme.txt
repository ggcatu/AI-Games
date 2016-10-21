Proyecto 1
Inteligencia Artificial I


12-11006 Gabriel Gimenez
	Anthony El Kharek
	Eliot Diaz



Ejercicio 1)
Se implemento un DFS con una cota, para lograr el objetivo.
El algoritmo se puede encontrar en /Entrega/dfs.c

Se pudo notar que estos problemas, en general tienen factores de ramificacion muy altos,
Lo que no permitia que en algunos casos la profundidad de busqueda avanzara rapidamente, por la gran cantidad de nodos.
Por otro lado, la eliminacion de generacion de padres y abuelos, vemos como mejora nuestro factor de ramificacion, disminuyendolo,
Y tratando de crear escenarios mucho mas manejables


Ejercicio 2)
Se utilizaron los resultados del DFS, y se aprovecharon los mismos.
Una vez contado los nodos y el factor de ramificacion empirico, se busco n tal que ramificacion^n > estados => n = log(estados)/log(ramificacion)
Todos los ejercicios contienen su DFS, donde solo difieren los #DEFINE al comienzo del archivo donde se especifica la profundidad desdeada + 1, junto
con la cantidad de estados del problema.
Los resultados devueltos por N deben ser interpretados como n = ceil(n), pues ciertamente no se consideran corridas no discretas.

Ejercicio 3)
Se implemento el algoritmo IDDFS para atacar el problema, incrementando la cota de 1 en 1, con el objetivo de encontrar el costo del camino minimo.
Ciertamente al ser un algoritmo de busqueda ciega, la cantidad de nodos explorados asi como generados es bastante alta, respecto a otros algoritmos
informados. Varios de los casos de prueba no lograban ser resueltos antes de los 10 minutos, pero podiamos ver como algunos tardaban hasta 8 minutos en ser resueltos. Una vez mas, la complejidad se encontraba en los altos factores de ramificacion, asi como que los goal se encontraban a distancias mas profundas.

