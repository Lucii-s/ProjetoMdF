#ifndef RSA_CRYPTO_H
#define RSA_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <sys/stat.h>
#include <sys/types.h>

// Função para criar diretórios se não existirem
void criar_diretorios() {
    mkdir("chaves", 0755);    // Cria pasta para chaves
    mkdir("mensagens", 0755); // Cria pasta para mensagens
}

// Função para gerar chaves RSA
void gerar_chaves_rsa() {
    mpz_t p, q, e, n, phi, d, p1, q1;
    mpz_inits(p, q, e, n, phi, d, p1, q1, NULL);

    // Solicita os valores ao usuário
    gmp_printf("Digite o valor de p: ");
    gmp_scanf("%Zd", p);

    gmp_printf("Digite o valor de q: ");
    gmp_scanf("%Zd", q);

    gmp_printf("Digite o valor de e: ");
    gmp_scanf("%Zd", e);

    // Calcula n = p * q
    mpz_mul(n, p, q);

    // Calcula phi = (p - 1) * (q - 1)
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // Verifica se e tem inverso modular em relação a phi
    if (mpz_invert(d, e, phi) == 0) {
        printf("Erro: e e phi(n) não são coprimos!\n");
        mpz_clears(p, q, e, n, phi, d, p1, q1, NULL);
        return;
    }

    // Exibe as chaves
    gmp_printf("Sua chave pública é: (e=%Zd, n=%Zd)\n", e, n);
    gmp_printf("Sua chave privada é: (d=%Zd, n=%Zd)\n", d, n);

    // Garante que os diretórios existam
    criar_diretorios();

    // Salva chave pública
    FILE *chavePublica = fopen("chaves/chave_publica.txt", "w");
    if (chavePublica == NULL) {
        perror("Erro ao criar chaves/chave_publica.txt");
        return;
    }
    gmp_fprintf(chavePublica, "%Zd %Zd", e, n);
    fclose(chavePublica);

    // Salva chave privada
    FILE *chavePrivada = fopen("chaves/chave_privada.txt", "w");
    if (chavePrivada == NULL) {
        perror("Erro ao criar chaves/chave_privada.txt");
        return;
    }
    gmp_fprintf(chavePrivada, "%Zd %Zd", d, n);
    fclose(chavePrivada);

    mpz_clears(p, q, e, n, phi, d, p1, q1, NULL);
    printf("Suas chaves foram salvas em:\n");
    printf("- chaves/chave_publica.txt\n");
    printf("- chaves/chave_privada.txt\n");
}

// Função para criptografar mensagem a partir de um arquivo
void criptografar_mensagem() {
    char n_str[1024], e_str[1024];
    mpz_t n, e, m, c;
    mpz_inits(n, e, m, c, NULL);

    // Garante que os diretórios existam
    criar_diretorios();

    // Abre o arquivo de mensagem original
    FILE *arquivo = fopen("encriptar.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir encriptar.txt");
        mpz_clears(n, e, m, c, NULL);
        return;
    }

    // Lê o conteúdo do arquivo
    char *mensagem = NULL;
    size_t tamanho = 0;
    ssize_t lidos = getdelim(&mensagem, &tamanho, '\0', arquivo);
    fclose(arquivo);
    
    if (lidos == -1) {
        perror("Erro ao ler encriptar.txt");
        free(mensagem);
        mpz_clears(n, e, m, c, NULL);
        return;
    }

    // Remove possível nova linha no final
    mensagem[strcspn(mensagem, "\n")] = '\0';

    printf("Digite o valor de e: ");
    fgets(e_str, sizeof(e_str), stdin);
    e_str[strcspn(e_str, "\n")] = '\0';

    printf("Digite o valor de n: ");
    fgets(n_str, sizeof(n_str), stdin);
    n_str[strcspn(n_str, "\n")] = '\0';

    mpz_set_str(n, n_str, 10); 
    mpz_set_str(e, e_str, 10);  

    // Abre arquivo para salvar mensagem criptografada
    FILE *arquivo_cripto = fopen("mensagens/criptografia.txt", "w");
    if (arquivo_cripto == NULL) {
        perror("Erro ao criar mensagens/criptografia.txt");
        free(mensagem);
        mpz_clears(n, e, m, c, NULL);
        return;
    }

    printf("Mensagem criptografada (valores inteiros):\n");
    for (size_t i = 0; i < strlen(mensagem); i++) {
        char letra = mensagem[i];
        int ascii = (int)letra;
        mpz_set_ui(m, ascii);
        mpz_powm(c, m, e, n);
        gmp_printf("%Zd ", c);
        gmp_fprintf(arquivo_cripto, "%Zd ", c);
    }

    printf("\n");
    fclose(arquivo_cripto);

    mpz_clears(n, e, m, c, NULL);
    free(mensagem);
    printf("Sua mensagem criptografada foi salva em: mensagens/criptografia.txt\n");
}

// Função auxiliar para exponenciação modular
void exp_modular(mpz_t resultado, mpz_t base, mpz_t expoente, mpz_t modulo) {
    mpz_powm(resultado, base, expoente, modulo);
}

// Função auxiliar para verificar se um número é primo
int eh_primo(mpz_t num) {
    return mpz_probab_prime_p(num, 25) > 0;
}

// Função auxiliar para calcular inverso modular
void inverso_modular(mpz_t resultado, mpz_t a, mpz_t m) {
    if (mpz_invert(resultado, a, m) == 0) {
        gmp_printf("Erro: O inverso modular não existe para %Zd modulo %Zd\n", a, m);
        exit(1);
    }
}

// Função para descriptografar mensagem a partir de um arquivo
void descriptografar_mensagem() {
    mpz_t p, q, e, d, n, phi, curr, descriptado;
    mpz_inits(p, q, e, d, n, phi, curr, descriptado, NULL);

    // Garante que os diretórios existam
    criar_diretorios();

    // Abre o arquivo fixo "criptografia.txt"
    FILE *arquivo = fopen("criptografia.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo criptografia.txt");
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    // Lê o conteúdo do arquivo
    char *conteudo = NULL;
    size_t tamanho = 0;
    ssize_t lidos = getdelim(&conteudo, &tamanho, '\0', arquivo);
    fclose(arquivo);
    
    if (lidos == -1) {
        perror("Erro ao ler o arquivo criptografia.txt");
        free(conteudo);
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    // Solicita os valores p, q e e
    do {
        printf("Digite o valor de p (deve ser primo): ");
        gmp_scanf("%Zd", p);
    } while (!eh_primo(p));

    do {
        printf("Digite o valor de q (deve ser primo): ");
        gmp_scanf("%Zd", q);
    } while (!eh_primo(q));

    printf("Digite o valor de e: ");
    gmp_scanf("%Zd", e);

    // Calcula n = p * q
    mpz_mul(n, p, q);

    // Calcula phi = (p - 1) * (q - 1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);

    // Calcula d (inverso modular de e em relação a phi)
    inverso_modular(d, e, phi);

    // Abre arquivo para salvar mensagem descriptografada
    FILE *arquivo_descripto = fopen("mensagens/mensagem_descriptografada.txt", "w");
    if (arquivo_descripto == NULL) {
        perror("Erro ao criar mensagens/mensagem_descriptografada.txt");
        free(conteudo);
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    // Processa a mensagem criptografada
    printf("Mensagem descriptografada: ");
    char *token = strtok(conteudo, " ");
    while (token != NULL) {
        if (mpz_set_str(curr, token, 10) == -1) {
            printf("\nErro: Valor criptografado inválido encontrado\n");
            break;
        }
        exp_modular(descriptado, curr, d, n);
        unsigned long caractere = mpz_get_ui(descriptado);
        if (caractere > 255) {
            printf("\nErro: Valor descriptografado inválido\n");
            break;
        }
        printf("%c", (char)caractere);
        fprintf(arquivo_descripto, "%c", (char)caractere);
        token = strtok(NULL, " ");
    }
    printf("\n");

    // Fecha arquivo e libera memória
    fclose(arquivo_descripto);
    mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
    free(conteudo);
    printf("Sua mensagem descriptografada foi salva em: mensagens/mensagem_descriptografada.txt\n");
}

#endif
