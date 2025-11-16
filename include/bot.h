#pragma once
#include "carta.h"
#include "card_node.h"
//#include <curl/curl.h>

#define GEMINI_KEY  "SUA_CHAVE_AQUI"
#define GEMINI_CURL "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent"

struct response_string {
    char* ptr;
    size_t len;
};


// Converte enums em texto
const char* corCarta(Carta* carta);
const char* valorCarta(Carta* carta);

// Monta a lista de cartas no formato usado no prompt
char* listarCartasParaPrompt(CartaNode* mao);

// Envia prompt para a API
char* perguntarGemini(const char* prompt);

// Integra cartaTopo + mão ? retorna número da carta escolhida
int decidirJogadaBot(CartaNode* mao);

// Se a IA falhar ? devolve jogada válida ou 0
int fallback_jogada();