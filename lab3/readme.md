# Respostas

### Os resultados coincidem? Por que? Qual solução se aproxima mais rapidamente do valor de π?
```Da forma como dividi o trabalho entre as threads, o resultado coincidiu independente do número de threads. Creio que isso ocorreu por que cada parte da série -- calculada por diferentes threads -- ficou com ordens de grandeza semelhantes. Assim, não houve distorções de ponto flutuante no momento da soma.```

### Exiba os resultados na saída padrão e avalie e ganho de desempenho obtido.
```O ganho de desempenho obtido foi próximo do esperado, com tempos de aproximadamente 2.4s, 4.4s e 8.8s para 1, 2 e 4 threads, respectivamente (caso N = 1000000000).```