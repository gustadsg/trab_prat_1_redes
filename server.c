#include "common.h"

#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "server.h"

#define BUFSZ 500
#define MAX_POKEMON 40
#define MAX_POKEMON_NAME 11
#define KILL_MSG "kill\n"

char pokedex[MAX_POKEMON][MAX_POKEMON_NAME];
int pokedexIndex = 0;

int main(int argc, char **argv)
{
    int isWrongUsage = argc < 3;
    if (isWrongUsage)
    {
        usage(argc, argv);
    }

    clearPokedex();

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        logexit("bind");
    }

    if (0 != listen(s, 10))
    {
        logexit("listen");
    }

    printf("[log] server is listening on %s:%s\n", argv[1], argv[2]);

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);

    while (1)
    {
        printf("[log] accepting connections\n");
        // accept connection
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1)
        {
            logexit("accept");
        }

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n\n\n", caddrstr);

        // receive message
        char buf[BUFSZ];
        size_t count;
        while (1)
        {
            printf("[log] receiving message\n");
            memset(buf, 0, BUFSZ); // clear buffer
            count = receiveMessage(csock, buf);

            // if received kill message, close connection and exit
            if (strcmp(buf, KILL_MSG) == 0)
            {
                printf("[log] kill message received.\n");
                fflush(stdout);
                break;
            }

            printf("[log] received %li bytes\n", count);
            printf("[msg] %s", buf);
            fflush(stdout);

            char response[BUFSZ];
            memset(response, 0, BUFSZ);

            sanitizeInput(buf);

            processMessage(buf, response);

            // not recognized command
            if (strcmp(response, KILL_MSG) == 0)
            {
                break;
            }

            printf("[log] sending response: %s\n\n\n", response);

            //clear buffer again
            clearStr(buf);
            // send message
            memcpy(buf, response, BUFSZ);
            removeTrailingWhitespaces(buf);
            strcat(buf, "\n");
            sendResponse(csock, buf);
        }
        printf("[log] closing connection\n");
        sendResponse(csock, "\n");
        clearPokedex();
        close(csock);
    }

    exit(EXIT_SUCCESS);
}

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

void clearStr(char *str)
{
    memset(str, 0, strlen(str));
}

void sanitizeInput(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
        i++;
    }
}

void clearPokedex()
{
    for (int i = 0; i < MAX_POKEMON; i++)
    {
        memset(pokedex[i], 0, MAX_POKEMON_NAME);
    }
    pokedexIndex = 0;
    printf("[log] cleared pokedex\n");
}

void processMessage(char *message, char *response)
{
    if (strstr(message, "add"))
    {
        handleAdd(message, response);
        printf("[log] added %s\n", response);
    }
    else if (strstr(message, "remove"))
    {
        handleRemove(message, response);
        printf("[log] removed %s\n", response);
    }
    else if (strcmp(message, "list") == 0)
    {
        handleList(response);
        printf("[log] listed %s\n", response);
    }
    else if (strstr(message, "exchange"))
    {
        handleExchange(message, response);
        printf("[log] exchanged %s\n", message);
    }
    else // in case of invalid command, kill connection
    {
        memcpy(response, KILL_MSG, strlen(KILL_MSG));
    }
}

void handleAdd(char *message, char *response)
{
    memset(response, 0, BUFSZ);

    char delimiter[2] = " ";
    char *pokemonName = strtok(message, delimiter);
    pokemonName = strtok(NULL, delimiter);

    int thereArePokemonsInInput = pokemonName != NULL;
    while (thereArePokemonsInInput)
    {
        if (!isValidPokemonName(pokemonName))
        {
            strcpy(response, "invalid message");
            return;
        }

        int isNewPokemon = searchPokemon(pokemonName) == NULL;

        if (!isNewPokemon)
        {
            char aux[BUFSZ];
            sprintf(aux, "%s already exists", pokemonName);
            strcat(response, aux);
            return;
        }

        int statusCode = addPokemonToPokedex(pokemonName);
        int pokemonAddedWithSuccess = statusCode > 0;

        if (!pokemonAddedWithSuccess)
        {
            if (statusCode == -1)
            {
                strcpy(response, "limit exceeded");
                return;
            }
            strcpy(response, "invalid message");
            return;
        }

        char aux[BUFSZ];
        sprintf(aux, "%s added ", pokemonName);
        strcat(response, aux);

        pokemonName = strtok(NULL, delimiter);
    };
}

int addPokemonToPokedex(char *pokemonName)
{
    if (pokedexIndex >= MAX_POKEMON)
    {
        return -1;
    }

    pokedexIndex++;
    strcpy(pokedex[pokedexIndex], pokemonName);
    return pokedexIndex;
}

void handleRemove(char *message, char *response)
{
    memset(response, 0, BUFSZ);
    char delimiter[2] = " ";
    char *pokemonName = strtok(message, delimiter);
    pokemonName = strtok(NULL, delimiter);

    char *pokemonInPokedex = searchPokemon(pokemonName);
    int pokemonExistsInPokedex = pokemonInPokedex != NULL;

    if (!isValidPokemonName(pokemonName))
    {
        strcpy(response, "invalid message");
        return;
    }

    if (!pokemonExistsInPokedex)
    {
        printf("[log] %s not found\n", pokemonName);
        sprintf(response, "%s does not exist", pokemonName);
        return;
    }

    strcpy(pokemonInPokedex, "\0");

    pokedexIndex--;

    removePokedexEmptySpaces();

    sprintf(response, "%s removed", pokemonName);
}

void removePokedexEmptySpaces()
{

    for (int i = 0; i < MAX_POKEMON; i++)
    {
        int currentPositionIsEmpty = strlen(pokedex[i]) < 1;
        int nextPositionIsEmpty = strlen(pokedex[i]) < 1;
        int shouldSwap = currentPositionIsEmpty && !nextPositionIsEmpty;
        if (shouldSwap)
        {
            strcpy(pokedex[i], pokedex[i + 1]);
        }
    }
}

void handleList(char *response)
{
    memset(response, 0, BUFSZ);
    int pokedexIsEmpty = pokedexIndex <= 0;
    if (pokedexIsEmpty)
    {
        strcpy(response, "none");
        return;
    }

    for (int i = 0; i <= pokedexIndex; i++)
    {
        int positionIsEmpty = strlen(pokedex[i]) < 1;
        if (!positionIsEmpty)
        {
            removeTrailingWhitespaces(pokedex[i]);
            strcat(response, pokedex[i]);
            strcat(response, " ");
        }
    }
}

void handleExchange(char *message, char *response)
{
    memset(response, 0, BUFSZ);
    char delimiter[2] = " ";
    char *oldPokemon = strtok(message, delimiter);
    oldPokemon = strtok(NULL, delimiter);

    if (!isValidPokemonName(oldPokemon))
    {
        strcpy(response, "invalid message");
        return;
    }

    char *existentPokemonInPokedex = searchPokemon(oldPokemon);
    int pokemonExistsInPokedex = existentPokemonInPokedex != NULL;
    if (!pokemonExistsInPokedex)
    {
        strcpy(response, oldPokemon);
        strcat(response, " does not exist");
        return;
    }

    char *secondPokemon = strtok(NULL, delimiter);

    if (!isValidPokemonName(secondPokemon))
    {
        strcpy(response, "invalid message");
        return;
    }
    pokemonExistsInPokedex = searchPokemon(secondPokemon) != NULL;
    if (pokemonExistsInPokedex)
    {
        sprintf(response, "%s already exists ", secondPokemon);
        return;
    }

    strcpy(existentPokemonInPokedex, secondPokemon);
    sprintf(response, "%s exchanged", oldPokemon);
}

int isValidPokemonName(char *pokemon)
{
    if (strlen(pokemon) > MAX_POKEMON_NAME)
    {
        return 0;
    }

    regex_t regex;
    regcomp(&regex, "^[a-z0-9_ ]*$", 0);

    int match = regexec(&regex, pokemon, 0, NULL, 0) == 0;

    if (!match)
    {
        return 0;
    }

    return 1;
}

char *searchPokemon(char *pokemon)
{
    for (int i = 0; i < MAX_POKEMON; i++)
    {
        if (strcmp(pokemon, pokedex[i]) == 0)
        {
            return pokedex[i];
        }
    }
    return NULL;
}

void sendResponse(int csock, char *response)
{
    size_t count = 0;

    while (count < strlen(response))
    {
        count += send(csock, response + count, strlen(response) - count, 0);
    }
}

int receiveMessage(int csock, char *buf)
{
    size_t count = 0;
    memset(buf, 0, BUFSZ);

    while (buf[strlen(buf) - 1] != '\n') // receive content until a newline is received
    {
        count += recv(csock, buf + count, BUFSZ - count, 0);
    }

    return count;
}

void removeTrailingWhitespaces(char *str)
{
    removeLeadingWhitespaces(str);
    removeTailWthitespaces(str);
    str[strlen(str)] = '\0';
}

void removeLeadingWhitespaces(char *str)
{
    int i = 0;
    while (i < strlen(str) && isWhiteSpace(str[i]))
    {
        str[i] = 0;
        i++;
    }
}

void removeTailWthitespaces(char *str)
{
    int i = strlen(str) - 1;
    while (i >= 0 && isWhiteSpace(str[i]))
    {
        str[i] = 0;
        i--;
    }
}

int isWhiteSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}