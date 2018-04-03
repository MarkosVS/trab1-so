def somatorio(n):
    soma = 1
    for i in range(2, n + 1):
        soma += i

    return soma


# testes
print(somatorio(1))
print(somatorio(2))
print(somatorio(5))
print(somatorio(10))
print(somatorio(20))
