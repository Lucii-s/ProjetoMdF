#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <sys/stat.h>
#include <sys/types.h>

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

// Função para criar diretórios se não existirem
void criar_diretorios() {
    mkdir("chaves", 0755);    // Cria pasta para chaves
    mkdir("mensagens", 0755); // Cria pasta para mensagens
}

// Função para gerar chaves RSA
void gerar_chaves_rsa() {
    mpz_t p, q, e, n, phi, d, p1, q1;
    mpz_inits(p, q, e, n, phi, d, p1, q1, NULL);

    // Solicita os valores ao usuário em uma linha
    printf("Digite os valores de p, q e e (separados por espaço): ");
    gmp_scanf("%Zd %Zd %Zd", p, q, e);

    // Verifica se p é primo
    if (!eh_primo(p)) {
        gmp_printf("Erro: %Zd não é um número primo!\n", p);
        mpz_clears(p, q, e, n, phi, d, p1, q1, NULL);
        return;
    }

    // Verifica se q é primo
    if (!eh_primo(q)) {
        gmp_printf("Erro: %Zd não é um número primo!\n", q);
        mpz_clears(p, q, e, n, phi, d, p1, q1, NULL);
        return;
    }

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

    // Exibe apenas a chave pública
    gmp_printf("Chave publica: %Zd %Zd\n", e, n);

    // Garante que os diretórios existam
    criar_diretorios();

    // Salva a chave pública
    FILE *chavePublica = fopen("chaves/chave_publica.txt", "w");
    if (chavePublica == NULL) {
        perror("Erro ao criar chaves/chave_publica.txt");
        return;
    }
    gmp_fprintf(chavePublica, "%Zd %Zd", e, n);
    fclose(chavePublica);

    mpz_clears(p, q, e, n, phi, d, p1, q1, NULL);
    printf("Chave publica salva em: chaves/chave_publica.txt\n");
}

// Função para criptografar mensagem a partir de um arquivo
void criptografar_mensagem() {
    char input[2048];
    mpz_t n, e, m, c;
    mpz_inits(n, e, m, c, NULL);

    // Garante que os diretórios existam
    criar_diretorios();

    // Solicita as chaves em uma linha
    printf("Digite os valores de e e n (separados por espaço): ");
    fgets(input, sizeof(input), stdin);
    
    // Extrai e e n da entrada
    char *token = strtok(input, " ");
    if (token) mpz_set_str(e, token, 10);
    token = strtok(NULL, " \n");
    if (token) mpz_set_str(n, token, 10);

    // Abre os arquivos
    FILE *arquivo_orig = fopen("mensagens/m_desencriptada.txt", "r");
    if (arquivo_orig == NULL) {
        perror("Erro ao abrir mensagens/m_desencriptada.txt");
        mpz_clears(n, e, m, c, NULL);
        return;
    }

    FILE *arquivo_cripto = fopen("mensagens/m_criptografada.txt", "w");
    if (arquivo_cripto == NULL) {
        perror("Erro ao criar mensagens/m_criptografada.txt");
        fclose(arquivo_orig);
        mpz_clears(n, e, m, c, NULL);
        return;
    }

    // Processa o arquivo caractere por caractere
    printf("\n=== Mensagem Criptografada ===\n");
    int ch;
    while ((ch = fgetc(arquivo_orig)) != EOF) {
        // Ignora quebras de linha no Windows (\r)
        if (ch == '\r') continue;
        
        mpz_set_ui(m, (unsigned int)ch);
        mpz_powm(c, m, e, n);
        
        gmp_printf("%Zd ", c);
        
        // Escreve no arquivo
        gmp_fprintf(arquivo_cripto, "%Zd ", c);
        
        // Se for quebra de linha, adiciona uma também no arquivo criptografado e no terminal
        if (ch == '\n') {
            fprintf(arquivo_cripto, "\n");
            printf("\n");
        }
    }

    printf("\n=============================\n");
    fclose(arquivo_orig);
    fclose(arquivo_cripto);
    mpz_clears(n, e, m, c, NULL);
    printf("\nMensagem criptografada salva em: mensagens/m_criptografada.txt\n");
}

void descriptografar_mensagem() {
    char input[2048];
    mpz_t p, q, e, d, n, phi, curr, descriptado;
    mpz_inits(p, q, e, d, n, phi, curr, descriptado, NULL);

    criar_diretorios();

    printf("Digite os valores de p, q e e (separados por espaço): ");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " ");
    if (token) mpz_set_str(p, token, 10);
    token = strtok(NULL, " ");
    if (token) mpz_set_str(q, token, 10);
    token = strtok(NULL, " \n");
    if (token) mpz_set_str(e, token, 10);

    if (!eh_primo(p)) {
        gmp_printf("Erro: %Zd não é um número primo!\n", p);
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    if (!eh_primo(q)) {
        gmp_printf("Erro: %Zd não é um número primo!\n", q);
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    mpz_mul(n, p, q);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);
    inverso_modular(d, e, phi);

    FILE *arquivo_cripto = fopen("mensagens/m_criptografada.txt", "r");
    if (arquivo_cripto == NULL) {
        perror("Erro ao abrir mensagens/m_criptografada.txt");
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    FILE *arquivo_descripto = fopen("mensagens/m_desencriptada.txt", "w");
    if (arquivo_descripto == NULL) {
        perror("Erro ao criar mensagens/m_desencriptada.txt");
        fclose(arquivo_cripto);
        mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
        return;
    }

    printf("\n=== Mensagem Descriptografada ===\n");
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), arquivo_cripto) != NULL) {
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            token[strcspn(token, "\n\r")] = '\0';
            if (strlen(token) == 0) {
                token = strtok(NULL, " ");
                continue;
            }

            if (strcmp(token, "-1") == 0) {
                fputc('\n', arquivo_descripto);
                printf("\n");
                token = strtok(NULL, " ");
                continue;
            }

            if (mpz_set_str(curr, token, 10) == -1) {
                token = strtok(NULL, " ");
                continue;
            }

            exp_modular(descriptado, curr, d, n);
            unsigned long caractere = mpz_get_ui(descriptado);
            if (caractere <= 255) {
                fputc((char)caractere, arquivo_descripto);
                printf("%c", (char)caractere);
            }

            token = strtok(NULL, " ");
        }
    }

    printf("\n===============================\n");
    fclose(arquivo_cripto);
    fclose(arquivo_descripto);
    mpz_clears(p, q, e, d, n, phi, curr, descriptado, NULL);
    printf("\nMensagem descriptografada salva em: mensagens/m_desencriptada.txt\n");
}


#endif
