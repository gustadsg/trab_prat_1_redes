# Documentação relacionada ao servidor

Todas as funções documentadas aqui estão declaradas em [server.h](../server.h)

- usage: imprime a maneira correta de chamar o servidor
- clearStr: limpa todos os caracteres de uma string
- clearPokedex: limpa todas as posições da pokedex e retora o pokedexIndex para 0
- sanitizeInput: faz a limpeza dos inputs removendo caracteres indesejados, como '\n'
- processMessage: identifica qual o tipo de comando foi recebido e chama a função correspondente
- handleAdd: trata do caso de adição de um pokemon
- handleRemove: trata do caso da remoção de um pokemon
- handleList: trata do caso da listagem de pokemons
- handleExchange: trata do caso da troca de pokemons
- isValidPokemonName: verifica se o nome do pokemon contem menos caracteres alfanuméricos
- searchPokemon: verifica se o pokemon existe na pokedex
- receiveMessage: recebe uma mensagem do cliente
- sendResponse: envia uma resposta para o cliente
- removeTrailingWhitespace: remove espaços vazios do final de uma string
- isWhiteSpace: verifica se um caractere é espaço vazio
