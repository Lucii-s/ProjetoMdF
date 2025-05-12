#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"

void limpar_buffer() {
    while (getchar() != '\n');
}

void menu_gerar_chaves() {
    printf("\n--- Gerar Chaves RSA ---\n");
    gerar_chaves_rsa();
}

void menu_criptografar() {
    printf("\n--- Criptografar Mensagem ---\n");
    limpar_buffer(); // Limpa o buffer do teclado
    criptografar_mensagem();
}

void menu_descriptografar() {
    printf("\n--- Descriptografar Mensagem ---\n");
    limpar_buffer(); // Limpa o buffer do teclado
    descriptografar_mensagem();
}

void exibir_menu() {
    int opcao;
    
    do {
        printf("\n=== MENU RSA CRYPTO ===\n");
        printf("1. Gerar chaves RSA\n");
        printf("2. Criptografar mensagem\n");
        printf("3. Descriptografar mensagem\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida!\n");
            limpar_buffer();
            continue;
        }
        
        switch(opcao) {
            case 1:
                menu_gerar_chaves();
                break;
            case 2:
                menu_criptografar();
                break;
            case 3:
                menu_descriptografar();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);
}

int main() {
    exibir_menu();
    return 0;
}
