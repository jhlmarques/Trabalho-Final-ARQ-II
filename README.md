# Descrição
Trabalho T2 da cadeira de Arquitetura e Organização de Computadores II<br>
<b>Grupo:</b><br>
Bernardo Beneduzi<br>
Heric Rodrigues<br>
José Henrique Marques<br>
Vitor Caruso


# Como rodar os programas:
<b>Fibonacci</b> (Escolha Livre): PUSH N na primeira linha, de forma a obter F(0) a F(N) na pilha<br>
Ex: <br>
N = 0 -> [0]<br>
N = 1 -> [0 1]<br>
N = 2 -> [0 1 1]<br>
N = 3 -> [0 1 1 2]<br>
N = 15 -> [0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610] <br>

<b>Primos</b>: PUSH N na primeira linha, de forma a obter o N-ésimo número primo<br>
Ex:<br>
```
N = 1 -> [2]
N = 2 -> [3]
N = 10 -> [29]
```

<b>Raiz quadrada</b>: PUSH N na primeira linha, de forma a obter a raiz quadrada de N.<br>
Valores não-quadrados de N retornam -1.<br>

Ex:<br>
```
N = -4 -> [-1]
N = -1 -> [-1]
N = 0 -> [0]
N = 3 -> [-1]
N = 4 -> [2]
N = 64 -> [8]
```

<b>Bháskhara</b>: PUSH A na primeira linha, PUSH -B e PUSH C nas linhas 7 e 8, de forma a obter<br>
-b +- ( sqrt(b² - 4 · a · c) ) / 2 · a na pilha<br>

Ex:<br>
```
A = 1, B = 8, C = -9 -> [1, -9]
```





