#ifndef SERVER_H
#define SERVER_H

int main(int argc, char **argv);
void usage(int argc, char **argv);
int receiveMessage(int csock, char *buf);
void sendResponse(int csock, char *response);
void removeTrailingWhitespaces(char *str);
void removeLeadingWhitespaces(char *str);
void removeTailWthitespaces(char *str);
int isWhiteSpace(char c);
void clearStr(char *str);
void sanitizeInput(char *str);
void clearPokedex();
void processMessage(char *message, char *response);
void handleAdd(char *message, char *response);
int addPokemonToPokedex(char *pokemonName);
void handleRemove(char *message, char *response);
void removePokedexEmptySpaces();
void handleList(char *response);
void handleExchange(char *message, char *response);
int isValidPokemonName(char *pokemon);
char *searchPokemon(char *pokemon);

#endif