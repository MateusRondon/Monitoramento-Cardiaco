pessoa1 = [1, 1, 0]
pessoa2 = [1, 1, 0]
pessoa3 = [1, 1, 0]
usuario4 = [1, 1, 1]
usuario5 = [0, 1, 1]
usuario6 = [0, 1, 1]
 
dados = [pessoa1, pessoa2, pessoa3, usuario4, usuario5, usuario6]
 
marcacoes = [1, 1, 1, -1, -1, -1]

from sklearn.naive_bayes import MultinomialNB

modelo = MultinomialNB()
modelo.fit(dados, marcacoes)

misterioso1 = [1, 1, 1]
misterioso2 = [1, 0, 0]
misterioso3 = [0, 0, 1]

teste = [misterioso1, misterioso2, misterioso3]

marcacoes_teste = [-1, 1, 1]

resultado = modelo.predict(teste)

diferencas = resultado - marcacoes_teste

acertos = [d for d in diferencas if d == 0]

total_de_acertos = len(acertos)
total_de_elementos = len(teste)

taxa_de_acerto = 100.0 * total_de_acertos / total_de_elementos

print(resultado)
print(diferencas)
print(taxa_de_acerto)