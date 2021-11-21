# Documentação relacionada ao servidor

Todas as funções documentadas aqui estão declaradas em [server.h](../server.h)

- main: faz a conexão com os clientes e cuida do fluxo de mensagens
- usage: imprime a maneira correta de chamar o servidor
- receiveMessage: recebe uma mensagem do cliente
- sendResponse: envia uma resposta para o cliente
- removeTrailingWhitespace: remove espaços vazios do começo e do final de uma sring
- removeLeadingWhitespaces: remove espaços vazios no começo de uma string
- removeTailWhitespaces: remove espaços vazios do final de uma string
- isWhiteSpace: verifica se um caractere é espaço vazio
- clearStr: limpa todos os caracteres de uma string
- sanitizeInput: faz a limpeza dos inputs removendo caracteres indesejados, como '\n'
- clearPokedex: limpa todas as posições da pokedex e retora o pokedexIndex para 0
- processMessage: identifica qual o tipo de comando foi recebido e chama a função correspondente
- handleAdd: trata do caso de adição de um ou mais pokemons
- addPokemonToPokedex: adiciona um pokemon a uma pokedex
- handleRemove: trata do caso da remoção de um pokemon
- handleList: trata do caso da listagem de pokemons
- handleExchange: trata do caso da troca de pokemons
- isValidPokemonName: verifica se o nome do pokemon contem menos caracteres alfanuméricos
- searchPokemon: verifica se o pokemon existe na pokedex
