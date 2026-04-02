#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tämä ohjelma lukee syötteen joko inputista tai tiedostosta.
 * 
 * Tulostaa rivit käänteisessä järjestyksessä.
 *
 * Virhetilanteet käsitellään ohjeiden mukaisesti.
 * Apua tehtävään koulun matskuista, https://www.geeksforgeeks.org/c/reverse-string-in-c/ sekä stack overflow
 * Tekoälyä käytettiin myös apuna virheenkäsittelyssä
 */

int main(int argc, char *argv[]) {

    // Oletuksena luetaan näppäimistöltä ja tulostetaan ruudulle
    FILE *input = stdin;
    FILE *output = stdout;

    // Jos argumentteja liikaa niin virhe
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // Jos annettu input-tiedosto
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Jos käyttäjä antoi myös output-tiedoston
    if (argc == 3) {

        // Input ja output eivät saa olla sama tiedosto
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "input- and outputfiles must differ\n");
            exit(1);
        }

        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }
    // Taulukko johon tallennetaan kaikki rivit
    char **lines = NULL;
    size_t line_count = 0;
    size_t capacity = 0;

    char *buffer = NULL;
    size_t length = 0;
    // Luetaan tiedosto rivi kerrallaan
    while (getline(&buffer, &length, input) != -1) {

        // Jos taulukko täynnä niin tehdään isompi
        if (line_count == capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;

            char **temp = realloc(lines, capacity * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }

            lines = temp;
        }

        // Rivin kopioiminen omaan muistiin
        lines[line_count] = strdup(buffer);
        if (lines[line_count] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        line_count++;
    }

    // Tulostus käänteisesti
    free(buffer);
    for (size_t i = line_count; i > 0; i--) {
        fprintf(output, "%s", lines[i - 1]);
        free(lines[i - 1]);
    }    

    // Suljetaan tiedostot jos ne eivät ole stdin/stdout
    free(lines);
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}

