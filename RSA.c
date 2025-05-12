#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp_inst/gmp-6.3.0/mini-gmp/mini-gmp.c"

// Função de verificação de primos
static int ehPrimo(mpz_t n)
{
    if (mpz_cmp_ui(n, 2) < 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0)
        return 1;
    if (mpz_even_p(n))
        return 0;

    mpz_t i, limit, remainder;
    mpz_init(i);
    mpz_init(limit);
    mpz_init(remainder);
    mpz_sqrt(limit, n);
    mpz_add_ui(limit, limit, 1);

    for (mpz_set_ui(i, 3); mpz_cmp(i, limit) <= 0; mpz_add_ui(i, i, 2))
    {
        mpz_mod(remainder, n, i);
        if (mpz_cmp_ui(remainder, 0) == 0)
        {
            mpz_clear(i);
            mpz_clear(limit);
            mpz_clear(remainder);
            return 0;
        }
    }

    mpz_clear(i);
    mpz_clear(limit);
    mpz_clear(remainder);
    return 1;
}


char *gerarChavesRSA(const char *p_str, const char *q_str, const char *e_str)
{
    static char resultado[4096] = {0};

    mpz_t p, q, e, n, phi, d, p1, q1;
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
    mpz_init(phi);
    mpz_init(d);
    mpz_init(p1);
    mpz_init(q1);

    // Validar entradas
    if (mpz_set_str(p, p_str, 10) != 0 ||
        mpz_set_str(q, q_str, 10) != 0 ||
        mpz_set_str(e, e_str, 10) != 0)
    {
        strcpy(resultado, "Erro: Valores inválidos para p, q ou e");
        goto cleanup;
    }

    // Verificar se são primos
    if (!ehPrimo(p) || !ehPrimo(q))
    {
        strcpy(resultado, "Erro: p e q devem ser números primos");
        goto cleanup;
    }

    // Calcular n = p * q
    mpz_mul(n, p, q);

    // Calcular phi(n) = (p-1)*(q-1)
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // Verificar se e e phi(n) são coprimos
    if (mpz_invert(d, e, phi) == 0)
    {
        strcpy(resultado, "Erro: e deve ser coprimo com (p-1)*(q-1)");
        goto cleanup;
    }

    // Formatar o resultado
    char temp[1024];
    resultado[0] = '\0';

    // Chave pública (e, n)
    strcat(resultado, "Chave Pública (e, n):\n");
    mpz_get_str(temp, 10, e);
    strcat(resultado, "e = ");
    strcat(resultado, temp);
    strcat(resultado, "\n");

    mpz_get_str(temp, 10, n);
    strcat(resultado, "n = ");
    strcat(resultado, temp);
    strcat(resultado, "\n\n");

    // Chave privada (d, n)
    strcat(resultado, "Chave Privada (d, n):\n");
    mpz_get_str(temp, 10, d);
    strcat(resultado, "d = ");
    strcat(resultado, temp);
    strcat(resultado, "\n");

    mpz_get_str(temp, 10, n);
    strcat(resultado, "n = ");
    strcat(resultado, temp);

cleanup:
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(d);
    mpz_clear(p1);
    mpz_clear(q1);

    return resultado;
}


char *encriptarMensagem(const char *mensagem, const char *e_str, const char *n_str) {
    static char resultado[8192] = {0};
    mpz_t n, e, m, c;
    mpz_init(n);
    mpz_init(e);
    mpz_init(m);
    mpz_init(c);

    // Validar entradas (agora com e primeiro)
    if (mpz_set_str(e, e_str, 10) != 0 ||
        mpz_set_str(n, n_str, 10) != 0) {
        strcpy(resultado, "Erro: Valores inválidos para e ou n");
        goto cleanup;
    }

    // Processar cada caractere da mensagem
    resultado[0] = '\0';
    strcat(resultado, "Mensagem criptografada:\n");

    char temp[256];
    for (size_t i = 0; i < strlen(mensagem); i++) {
        mpz_set_ui(m, (unsigned long)mensagem[i]);
        mpz_powm(c, m, e, n); // c = m^e mod n

        mpz_get_str(temp, 10, c);
        strcat(resultado, temp);
        strcat(resultado, " ");
    }

cleanup:
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(m);
    mpz_clear(c);

    return resultado;
}

char *desencriptarMensagem(const char *p_str, const char *q_str, const char *e_str, const char *mensagem_encriptada) {
    static char resultado[8192] = {0};
    mpz_t p, q, e, n, phi, d, c, m;
    
    // Inicializar todas as variáveis GMP
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(n);
    mpz_init(phi);
    mpz_init(d);
    mpz_init(c);
    mpz_init(m);

    // Validar entradas
    if (mpz_set_str(p, p_str, 10) != 0 ||
        mpz_set_str(q, q_str, 10) != 0 ||
        mpz_set_str(e, e_str, 10) != 0) {
        strcpy(resultado, "Erro: Valores inválidos para p, q ou e");
        goto cleanup;
    }

    // Verificar se são primos
    if (!ehPrimo(p) || !ehPrimo(q)) {
        strcpy(resultado, "Erro: p e q devem ser números primos");
        goto cleanup;
    }

    // Calcular n e phi(n)
    mpz_mul(n, p, q);
    mpz_sub_ui(p, p, 1);  // p = p-1
    mpz_sub_ui(q, q, 1);  // q = q-1
    mpz_mul(phi, p, q);   // phi = (p-1)*(q-1)

    // Calcular d (chave privada)
    if (mpz_invert(d, e, phi) == 0) {
        strcpy(resultado, "Erro: e deve ser coprimo com (p-1)*(q-1)");
        goto cleanup;
    }

    // Processar mensagem encriptada
    resultado[0] = '\0';
    strcat(resultado, "Mensagem descriptografada:\n");

    char *token, *resto = strdup(mensagem_encriptada);
    token = strtok(resto, " ");

    while (token != NULL) {
        if (mpz_set_str(c, token, 10) == 0) {
            mpz_powm(m, c, d, n); // m = c^d mod n
            char caractere = (char)mpz_get_ui(m);
            strncat(resultado, &caractere, 1);
        }
        token = strtok(NULL, " ");
    }

    free(resto);

cleanup:
    // Liberar memória
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(d);
    mpz_clear(c);
    mpz_clear(m);

    return resultado;
}

