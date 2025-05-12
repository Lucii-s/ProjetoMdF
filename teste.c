#include "RSA.h"
#include <stdio.h>

int main() {

    // Corrigindo: usando strings para os números (em vez de caracteres)
    char *chavesRSA = gerarChavesRSA("1907", "647", "25");
    char *encriptacao = encriptarMensagem("Lucinha", "25", "1233829");
    char *desencriptacao = desencriptarMensagem("1907", "647", "25", "856169 520859 931393 158654 952471 639133 893376");
    
    // Adicionando impressão do resultado
    printf("%s\n", chavesRSA);
    printf("%s\n", encriptacao);
    printf("%s\n", desencriptacao);
    
    return 0;
}