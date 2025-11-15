#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game_manager.h"
#include "card_node.h"
#include "pilha.h"
#include "bot.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define GEMINI_KEY  "AIzaSyBViaVswaJ09NO66qi7TwDZh5dvwlhqTDI"
#define GEMINI_CURL "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent"

//------------------------------------------------------
// 1. STRUCT **antes** das funções → corrige o warning
//------------------------------------------------------
struct response_string {
    char *ptr;
    size_t len;
};

typedef enum Valor {
	ZERO,
	UM,
	DOIS,
	TRES,
	QUATRO,
	CINCO,
	SEIS,
	SETE,
	OITO,
	NOVE,
	BLOQUEAR,
	DEFINIR_COR,
	MAIS_2,
	MAIS_4,
	REVERSO,
} Valor;

typedef enum Cor {
	AMARELO,
	AZUL,
	VERDE,
	VERMELHO,
	INCOLOR,
} Cor;

const char *corCarta(Carta *carta){
    switch(carta->cor){
        case AMARELO: return "Amarelo";
        case VERDE: return "Verde";
        case VERMELHO: return "Vermelho";
        case AZUL: return "Azul";
        case INCOLOR: return "Incolor";
        default: return "Erro com cor";
    }
}

const char *valorCarta(Carta *carta){
    switch(carta->valor){
        case ZERO: return "0";
        case UM: return "1";
        case DOIS: return "2";
        case TRES: return "3";
        case QUATRO: return "4";
        case CINCO: return "5";
        case SEIS: return "6";
        case SETE: return "7";
        case OITO: return "8";
        case NOVE: return "9";
        case BLOQUEAR: return "Bloquear";
        case DEFINIR_COR: return "DefinirCor";
        case MAIS_2: return "Mais2";
        case MAIS_4: return "Mais4";
        case REVERSO: return "Reverso";
        default: return "Erro com valor";
    }
}


//------------------------------------------------------
// 2. Função do libcurl para armazenar o texto recebido
//------------------------------------------------------
static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct response_string *s) {
    size_t new_len = s->len + size * nmemb;

    s->ptr = realloc(s->ptr, new_len + 1);
    if (!s->ptr) {
        printf("Erro ao realocar memória!\n");
        exit(1);
    }

    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}

int tamanhoMaoproximoJogador(){

    CartaNode *maoProximo = game.jogador_da_vez->next->mao;
    int i = 1;


    while (maoProximo != NULL) {
        maoProximo = maoProximo ->next;
        i++;
    }

    return i;
}

char *listarCartasParaPrompt(CartaNode *mao) {
    char *texto = malloc(2048);
    texto[0] = '\0';

    CartaNode *atual = mao;
    int i = 1;

    while (atual != NULL) {
        char linha[128];
        snprintf(linha, sizeof(linha),
                 "%d) Cor: %s | Valor: %s\n",
                 i,
                 corCarta(atual->carta->cor),
                 valorCarta(atual->carta->valor));

        strcat(texto, linha);

        atual = atual->next;
        i++;
    }

    return texto; // lembre-se de dar free() depois
}


//------------------------------------------------------
// 4. Enviar pergunta ao Gemini
//------------------------------------------------------
char *perguntarGemini(const char *prompt) {

    CURL *curl = curl_easy_init();
    struct response_string resposta = { malloc(1), 0 };

    if (!curl) {
        printf("Erro ao inicializar CURL!\n");
        return NULL;
    }

    char json_envio[2048];
    snprintf(json_envio, sizeof(json_envio),
        "{\"contents\": [{\"parts\": [{\"text\": \"%s\"}]}]}",
        prompt
    );

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char auth_header[300];
    snprintf(auth_header, sizeof(auth_header), "x-goog-api-key: %s", GEMINI_KEY);
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_URL, GEMINI_CURL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_envio);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resposta);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return resposta.ptr;  // devolver texto da IA
}

//------------------------------------------------------
// 5. Criar prompt para o bot escolher jogada
//------------------------------------------------------
int decidirJogadaBot(CartaNode *mao) {

    Carta *cartaTopo = check_top(game.pilha);

    char *maoTexto = listarCartasParaPrompt(game.jogador_da_vez->mao);

    char prompt[3000];
    snprintf(prompt, sizeof(prompt),
    "Você é um jogador profissional de UNO.\n"
    "A carta atual da mesa é: Cor = %s, Valor = %s.\n"
    "Aqui estão minhas cartas:\n%s\n"
    "Aqui estão o número de cartas do proximo jogador:\n%d\n"
    "Com base nas regras do UNO, qual carta devo jogar?\n"
    "Caso o proximo jogador tenha menos de 4 cartas, priorize cartas especiais e de compra\n"
    "Caso queira jogar incolor, SEMPRE escolha a cor que mais tem na sua mão\n"
    "Responda APENAS e EXCLUSIVAMENTE com o número da carta (1, 2, 3...).\n"
    "Caso não tenha nenhuma carta válida SEMPRE ESPONDA com o número 0",
    corCarta(cartaTopo),
    valorCarta(cartaTopo),
    maoTexto,
    tamanhoMaoproximoJogador(game.jogador_da_vez->mao)
);
    free(maoTexto);

    // 3. Envia ao Gemini
    char *resposta = perguntar_gemini(prompt);

    // 4. Converter texto para número
    int escolha = atoi(resposta);

    free(resposta);

    return escolha;  // 0 = não jogar nada
}

int fallback_jogada() {
    CartaNode *temp = game.jogador_da_vez->mao;
    int i = 1;

    while (temp) {
        if (validar_jogada(temp->carta, game.pilha))
            return i; // devolve jogada válida
        temp = temp->next;
        i++;
    }

    return 0; // nenhuma jogada → comprar
}
