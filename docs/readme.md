# Redes de Computadores - Trabalho Prático 1

## Aluno: Gustavo da Silva Gomes

### Descrição

Este projeto é uma implementação de um sistema de sockets simples, em que o cliente simula um treinador pokemon e o servidor simula uma pokedex.

## Requisitos funcionais

O treinador (cliente) deve poder efetuar os seguintes comandos:

- add: adiciona um novo pokemon à pokedex
- remove: remove um pokemon da pokedex
- list: lista todos os pokemons salvos na pokedex
- exchange: troca um pokemon da pokedex
- kill: termina a conexão entre o servidor e cliente

## Requisitos não funcionais

Os comandos devem ser executados seguindo o seguinte padrão:

- add pokemonX
- add pokemonY pokemonZ
- remove pokemonX
- list
- exchange pokemonX pokemonY
- kill

As respostas devem seguir o seguinte padrão:

- pokemonX added
- pokemonY added pokemonZ added
- pokemonX removed
- pokemonY pokemonZ
- exchange pokemonX pokemonY

Outros:

- a pokedex deve ter espaço para exatamente 40 pokemons
- o nome de um pokemon deve ter no maximo 10 caracteres

## Regras de negócio

- caso um usuário tente adicionar um pokemon quando a pokedex estiver cheia, ele deve receber a mensagem de erro "limit exceeded"
- caso um usuário tente adicionar um pokemon que já existe na pokedex, ele deve receber a mensagem de erro "pokemonX already exists"
- caso um usuário tente remover um pokemon que não existe na pokedex, ele deve receber a mensagem de erro "pokemonX does not exist"
- caso um usuário tente trocar um pokemon que não existe na pokedex, ele deve receber a mensagem de erro "pokemonX does not exist"
- caso um usuário tente trocar um pokemon por outro que que já existe na pokedex, ele deve receber a mensagem de erro "pokemonX already exists"
- caso um usuário insira um nome de pokemon que contenha algo alem de caracteres alfanuméricos, ele deve receber a mensagem de erro "invalid message"
- caso um usuário insira um nome de pokemon com mais caracteres do que o limite, ele deve receber a mensagem de erro "invalid message"

## Mais

Informações detalhadas sobre implementação podem ser obridas em: [docs](./docs)
