#include "C:\msys64\mingw64\include\curl\curl.h"
#include <stdio.h>
#define CURL_STATICLIB
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define START 1
#define LOOP 2
#define EXIT 3

typedef struct {
  cJSON *title;
  cJSON *number;
  cJSON *numberItem;
  cJSON *word;
  cJSON *phonetics;
  cJSON *phoneticsItems;
  cJSON *text;
  cJSON *audio;
  cJSON *meanings;
  cJSON *meaningsItems;
  cJSON *partOfSpeech;
  cJSON *partOfSpeechItems;
  cJSON *definitions;
  cJSON *definitionsItems;
  cJSON *definition;
  cJSON *example;
  cJSON *synonyms;
  cJSON *antonyms;

} jsonTypeInfoDictionary_t;

typedef struct {

  cJSON *result;
  cJSON *resultItem;
  cJSON *message;
  cJSON *name;
  cJSON *chat;
  cJSON *chatId;
  cJSON *chatText;
  cJSON *photo;
  cJSON *audio;

} jsonTypeInfoTelegram_t;

typedef struct string_buffer_s {
  char  *ptr;
  size_t len;
} string_buffer_t;

char *error = "No se pudo encontrar una definicion para la palabra que esta buscando.\nRecuerde que solo puede enviar palabras en ingles sin espacios";

static void   string_buffer_initialize(string_buffer_t *);
static void   string_buffer_finish(string_buffer_t *);
static size_t string_buffer_callback(void *, size_t, size_t, void *);
static size_t header_callback(char *, size_t, size_t, void *);
static size_t write_callback(void *, size_t, size_t, void *);

void  sendWelcome(jsonTypeInfoTelegram_t *, char *, string_buffer_t *);
int   sentStatus(string_buffer_t *, cJSON *, char *);
void  count(const char *, int, int *, int *);
void  buildTelegramMessage(jsonTypeInfoDictionary_t *, char *);
int   errorCheck(jsonTypeInfoDictionary_t *, char *);
void  removeQuotation(char *, int len);
char *stringAdjustment(const char *, int);
void  consultApi(char *, string_buffer_t *);
void  askDictionary(char *, char *, string_buffer_t *);
int   parseTelegram(jsonTypeInfoTelegram_t *, string_buffer_t *);
void  sendTelegram(char *, char *, char *, string_buffer_t *);
char *parseDictionary(jsonTypeInfoDictionary_t *, string_buffer_t *);
void  parseExample(jsonTypeInfoDictionary_t *, char *);
void  parseWord(jsonTypeInfoDictionary_t *, char *);
void  parsePhonetics(jsonTypeInfoDictionary_t *, char *);
void  parseSynonyms(jsonTypeInfoDictionary_t *, char *);
void  parseAntonyms(jsonTypeInfoDictionary_t *, char *);
void  parseDefinitions(jsonTypeInfoDictionary_t *, char *);
void  parseMeanings(jsonTypeInfoDictionary_t *, char *);
void  delay(unsigned int);

int main(int argc, char *argv[]) {
  char *message;
  char  previousMessage[1000];
  int   menu = 0;

  string_buffer_t strbuf;
  string_buffer_t strbuf2;

  char *url_api1 = "https://api.telegram.org/bot6888044690:AAG09N83Jrw2ol3QRjalqkr5Xp5sct6etKY/getUpdates";
  char *url_api2 = "https://api.telegram.org/bot6888044690:AAG09N83Jrw2ol3QRjalqkr5Xp5sct6etKY/sendMessage?chat_id=";
  char *url_api3 = "https://api.dictionaryapi.dev/api/v2/entries/en/";

  do {

    int countEnterAndSpaces = 0;
    int countUndefined = 0;

    string_buffer_initialize(&strbuf);

    string_buffer_initialize(&strbuf2);

    jsonTypeInfoTelegram_t messages = {.chat = NULL,
                                       .chatId = NULL,
                                       .chatText = NULL,
                                       .name = NULL,
                                       .message = NULL,
                                       .result = NULL,
                                       .resultItem = NULL,
                                       .audio = NULL,
                                       .photo = NULL};

    jsonTypeInfoDictionary_t wordInfo = {.antonyms = NULL,
                                         .audio = NULL,
                                         .definition = NULL,
                                         .definitions = NULL,
                                         .definitionsItems = NULL,
                                         .example = NULL,
                                         .meanings = NULL,
                                         .meaningsItems = NULL,
                                         .number = NULL,
                                         .numberItem = NULL,
                                         .partOfSpeech = NULL,
                                         .partOfSpeechItems = NULL,
                                         .phonetics = NULL,
                                         .phoneticsItems = NULL,
                                         .synonyms = NULL,
                                         .text = NULL,
                                         .title = NULL,
                                         .word = NULL};

    consultApi(url_api1, &strbuf);

    if (parseTelegram(&messages, &strbuf) == 0) {
      message = cJSON_Print(messages.chatText);
      removeQuotation(message, strlen(message)); // Le saco los corchetes a la palabra
      count(message, strlen(message), &countEnterAndSpaces, &countUndefined);

      if ((strcmp(message, "/start") == 0) && (menu == 0)) {
        sendWelcome(&messages, url_api2, &strbuf2);
        menu = START;
      }

      if ((strcmp(message, "/exit")) == 0 && (menu == LOOP)) {
        sendTelegram(url_api2, cJSON_Print(messages.chatId), "Que tenga un buen dia!", &strbuf2);
        menu = EXIT;
      }

      if (menu == LOOP) {
        if ((strcmp(message, previousMessage)) == 0)
          ;
        else if (countEnterAndSpaces == 0 && countUndefined == 0) {

          memset(previousMessage, 0, sizeof(char));
          strcpy(previousMessage, message);

          string_buffer_initialize(&strbuf);

          askDictionary(url_api3, message, &strbuf); // Le pido al diccionario informacion acerca de la palabra

          char *telegramMessage = parseDictionary(&wordInfo, &strbuf);

          sendTelegram(url_api2, cJSON_Print(messages.chatId), telegramMessage, &strbuf2);

          free(telegramMessage);
        } else {
          sendTelegram(url_api2, cJSON_Print(messages.chatId), error, &strbuf);
          string_buffer_initialize(&strbuf);
          memset(previousMessage, 0, sizeof(char));
          strcpy(previousMessage, message);
        }
      }

      if (menu == START) {
        memset(previousMessage, 0, sizeof(char));
        strcpy(previousMessage, message);
        menu = LOOP;
      }
    }
    delay(2);
  } while (menu != EXIT);
  return 0;
}

void sendWelcome(jsonTypeInfoTelegram_t *wordInfo, char *url_api2, string_buffer_t *messageInfo) {
  char welcome[200] = {0};

  strcat(welcome, "Bienvenido ");
  strcat(welcome, cJSON_Print(wordInfo->name));
  strcat(welcome, " al Diccionario Ingles\nEnvie una palabra en ingles sin espacios para comenzar");

  sendTelegram(url_api2, cJSON_Print(wordInfo->chatId), welcome, messageInfo);
}

void removeQuotation(char *str, int len) {

  for (int i = 0; i < len - 2; i++)
    str[i] = str[i + 1];

  str[len - 2] = '\0';
}

int requestWord(char *url_api1, string_buffer_t *info) {
  CURL *curl_api1;

  curl_api1 = curl_easy_init();
  if (!curl_api1) {
    fprintf(stderr, "Fatal: curl_easy_init() error.\n");
    return EXIT_FAILURE;
  }

  /* Especificar la URL a consultar */
  curl_easy_setopt(curl_api1, CURLOPT_URL, url_api1);
  curl_easy_setopt(curl_api1, CURLOPT_FOLLOWLOCATION, 1L);

  /* Enviar la información de respuesta del encabezado a la función */
  curl_easy_setopt(curl_api1, CURLOPT_WRITEFUNCTION, write_callback);

  /* Preparamos el callback para cuando la API responda */
  curl_easy_setopt(curl_api1, CURLOPT_WRITEDATA, info);

  /* get it! */
  CURLcode res = curl_easy_perform(curl_api1);
  /* check for errors */
  if (res != CURLE_OK) {
    fprintf(stderr, "Request failed: curl_easy_perform(): %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl_api1);
    string_buffer_finish(info);
    return EXIT_FAILURE;
  }
}

int parseTelegram(jsonTypeInfoTelegram_t *chatInfo, string_buffer_t *rawInfo) {

  cJSON *json = cJSON_Parse(rawInfo->ptr);

  chatInfo->result = cJSON_GetObjectItemCaseSensitive(json, "result");

  cJSON_ArrayForEach(chatInfo->resultItem, chatInfo->result) {

    chatInfo->message = cJSON_GetObjectItemCaseSensitive(chatInfo->resultItem, "message");
    chatInfo->chat = cJSON_GetObjectItemCaseSensitive(chatInfo->message, "chat");
    chatInfo->chatText = cJSON_GetObjectItemCaseSensitive(chatInfo->message, "text");
    // printf("%s", cJSON_Print(chatInfo->chatText));

    chatInfo->name = cJSON_GetObjectItemCaseSensitive(chatInfo->chat, "first_name");
    chatInfo->chatId = cJSON_GetObjectItemCaseSensitive(chatInfo->chat, "id");
  }

  if (chatInfo->chatText == NULL) {
    string_buffer_finish(rawInfo);
    return 1;
  }
  string_buffer_finish(rawInfo);
  return 0;
}

void askDictionary(char *url_api3, char *word, string_buffer_t *info) {

  char wordSearch[1000] = {0};

  strcat(wordSearch, url_api3);
  strcat(wordSearch, word);

  consultApi(wordSearch, info);
}

char *parseDictionary(jsonTypeInfoDictionary_t *wordInfo, string_buffer_t *rawInfo) {
  char *errorResponse = NULL;

  errorResponse = calloc((rawInfo->len + 1) * sizeof(char), sizeof(char));

  if (errorResponse == NULL)
    printf("\nerror en la reserva de memoria\n");
  else {

    errorResponse[rawInfo->len] = '\0';

    strcat(errorResponse, rawInfo->ptr);

    if (errorCheck(wordInfo, errorResponse) == 0) {
      string_buffer_finish(rawInfo);
      return errorResponse;
    } else {

      free(errorResponse);

      char *apiResponse = NULL; // Creo un nuevo arreglo para agregarle {"result":}

      int newLen = ((rawInfo->len) + 12);

      apiResponse = calloc(newLen * sizeof(char), sizeof(char));

      if (apiResponse == NULL)
        printf("\nerror en la reserva de memoria\n");
      else {

        apiResponse[newLen - 1] = '\0';

        strcat(apiResponse, "{\"result\":");
        strcat(apiResponse, rawInfo->ptr);
        strcat(apiResponse, "}");

        buildTelegramMessage(wordInfo, apiResponse);

        string_buffer_finish(rawInfo);
        return apiResponse;
      }
    }
  }
}

int errorCheck(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {

  cJSON *json = cJSON_Parse(apiResponse);

  memset(apiResponse, 0, sizeof(char)); // Reinicio el apiResponse para usarlo para enviar el mensaje por telegram

  wordInfo->title = cJSON_GetObjectItemCaseSensitive(json, "title");

  if (wordInfo->title != NULL) {
    strcat(apiResponse, error);
    return 0;
  }
  return 1;
}

void buildTelegramMessage(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {

  static int types = 1;
  char       number[20];

  cJSON *json = cJSON_Parse(apiResponse);

  memset(apiResponse, 0, sizeof(char));

  wordInfo->number = cJSON_GetObjectItemCaseSensitive(json, "result");

  cJSON_ArrayForEach(wordInfo->numberItem, wordInfo->number) {

    sprintf(number, "Type %d for: ", types++);
    strcat(apiResponse, number);

    parseWord(wordInfo, apiResponse);

    parsePhonetics(wordInfo, apiResponse);

    parseMeanings(wordInfo, apiResponse);
  }
  types = 1;
}

void parseWord(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {
  wordInfo->word = cJSON_GetObjectItemCaseSensitive(wordInfo->numberItem, "word");

  strcat(apiResponse, cJSON_Print(wordInfo->word));
  strcat(apiResponse, "\n\n");
}

void parsePhonetics(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {
  wordInfo->phonetics = cJSON_GetObjectItemCaseSensitive(wordInfo->numberItem, "phonetics");

  static int phoneticas = 1;
  char       number[20];

  cJSON_ArrayForEach(wordInfo->phoneticsItems, wordInfo->phonetics) {
    wordInfo->audio = cJSON_GetObjectItemCaseSensitive(wordInfo->phoneticsItems, "audio");

    if (wordInfo->audio != NULL && (strlen(cJSON_Print(wordInfo->audio)) > 3)) {
      sprintf(number, "Phonetica %d:\n", phoneticas++);

      strcat(apiResponse, number);
      strcat(apiResponse, "Audio:");
      strcat(apiResponse, cJSON_Print(wordInfo->audio));
      strcat(apiResponse, "\n");
    }
    strcat(apiResponse, "\n");
  }
  phoneticas = 1;
}

void parseMeanings(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {

  wordInfo->meanings = cJSON_GetObjectItemCaseSensitive(wordInfo->numberItem, "meanings");

  static int significados = 1;

  char number[20];

  cJSON_ArrayForEach(wordInfo->meaningsItems, wordInfo->meanings) {

    wordInfo->partOfSpeech = cJSON_GetObjectItemCaseSensitive(wordInfo->meaningsItems, "partOfSpeech");

    sprintf(number, "Clasificacion %d: ", significados++);

    strcat(apiResponse, number);
    strcat(apiResponse, cJSON_Print(wordInfo->partOfSpeech));
    strcat(apiResponse, "\n\n");

    parseDefinitions(wordInfo, apiResponse);
    parseSynonyms(wordInfo, apiResponse);
    parseAntonyms(wordInfo, apiResponse);
    strcat(apiResponse, "\n");
  }
  significados = 1;
}

void parseDefinitions(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {
  static int definiciones = 1;

  wordInfo->definitions = cJSON_GetObjectItemCaseSensitive(wordInfo->meaningsItems, "definitions");

  cJSON_ArrayForEach(wordInfo->definitionsItems, wordInfo->definitions) {
    char number[20];

    wordInfo->definition = cJSON_GetObjectItemCaseSensitive(wordInfo->definitionsItems, "definition");

    sprintf(number, "Definicion %d: ", definiciones++);

    strcat(apiResponse, number);
    strcat(apiResponse, cJSON_Print(wordInfo->definition));
    strcat(apiResponse, "\n");

    parseExample(wordInfo, apiResponse);
  }
  definiciones = 1;
}

void parseExample(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {

  wordInfo->example = cJSON_GetObjectItemCaseSensitive(wordInfo->definitionsItems, "example");

  if (wordInfo->example != NULL) {
    strcat(apiResponse, "Ejemplo: ");
    strcat(apiResponse, cJSON_Print(wordInfo->example));
    strcat(apiResponse, "\n");
  }
}

void parseSynonyms(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {
  wordInfo->synonyms = cJSON_GetObjectItemCaseSensitive(wordInfo->meaningsItems, "synonyms");

  if (wordInfo->synonyms != NULL && strlen(cJSON_Print(wordInfo->synonyms)) > 4) {
    strcat(apiResponse, "\n");
    strcat(apiResponse, "Sinonimos: ");
    strcat(apiResponse, cJSON_Print(wordInfo->synonyms));
    strcat(apiResponse, "\n");
  }
}

void parseAntonyms(jsonTypeInfoDictionary_t *wordInfo, char *apiResponse) {

  wordInfo->antonyms = cJSON_GetObjectItemCaseSensitive(wordInfo->meaningsItems, "antonyms");

  if (wordInfo->synonyms != NULL && strlen(cJSON_Print(wordInfo->antonyms)) > 4) {
    strcat(apiResponse, "\n");
    strcat(apiResponse, "Antonimos: ");
    strcat(apiResponse, cJSON_Print(wordInfo->antonyms));
    strcat(apiResponse, "\n");
  }
}

void sendTelegram(char *url_api2, char *chat_id, char *info, string_buffer_t *messageInfo) {
  char *mensaje_enviar = NULL;

  char *adjustedString = stringAdjustment(info, strlen(info));

  mensaje_enviar = calloc((strlen(adjustedString) + strlen(url_api2) + strlen(chat_id) + 7) * sizeof(char), sizeof(char));

  if (mensaje_enviar != NULL) {

    strcat(mensaje_enviar, url_api2);
    strcat(mensaje_enviar, chat_id);
    strcat(mensaje_enviar, "&text=");
    strcat(mensaje_enviar, adjustedString);

    consultApi(mensaje_enviar, messageInfo);

    free(adjustedString);
    free(mensaje_enviar);

    cJSON *ok;
    char  *okText;

    if (sentStatus(messageInfo, ok, okText)) {
      string_buffer_finish(messageInfo);
      sendTelegram(url_api2, chat_id, "Info too long, sorry", messageInfo);
    }

    string_buffer_finish(messageInfo);
  } else printf("Error en la reserva de memoria.");
}

char *stringAdjustment(const char *str, const int len) {
  int spaceAndEnterCount = 0;
  int undefinedCount = 0;

  count(str, len, &spaceAndEnterCount, &undefinedCount);

  int newLen = 0;

  newLen = (((len + 1) - undefinedCount) + (2 * spaceAndEnterCount));

  char *newStr = calloc(newLen * sizeof(char), sizeof(char));

  if (newStr != NULL) {
    newStr[newLen - 1] = '\0';

    int posiciones = 0;

    for (int i = 0; i <= len; i++) {
      if (str[i] == ' ') {
        newStr[posiciones] = '%';
        newStr[posiciones + 1] = '2';
        newStr[posiciones + 2] = '0';
        posiciones += 3;
      } else if (str[i] == '\n') {
        newStr[posiciones] = '%';
        newStr[posiciones + 1] = '0';
        newStr[posiciones + 2] = 'A';
        posiciones += 3;
      } else if (str[i] > 0) {
        newStr[posiciones] = str[i];
        posiciones += 1;
      }
    }
    newStr[newLen - 1] = '\0';
    return newStr;
  } else printf("Error en la reserva de memoria\n");
}

void consultApi(char *apiString, string_buffer_t *callback) {
  CURL *curl_api;

  curl_api = curl_easy_init();

  if (!curl_api)
    fprintf(stderr, "Fatal: curl_easy_init() error.\n");

  /* Especificar la URL a consultar */
  curl_easy_setopt(curl_api, CURLOPT_URL, apiString);
  curl_easy_setopt(curl_api, CURLOPT_FOLLOWLOCATION, 1L);

  /* Enviar la información de respuesta del encabezado a la función */
  curl_easy_setopt(curl_api, CURLOPT_WRITEFUNCTION, write_callback);

  /* Preparamos el callback para cuando la API responda */
  curl_easy_setopt(curl_api, CURLOPT_WRITEDATA, callback);
  CURLcode res = curl_easy_perform(curl_api);

  if (res != CURLE_OK) {
    fprintf(stderr, "Request failed: curl_easy_perform(): %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl_api);
    string_buffer_finish(callback);
  }
}

void count(const char *str, int len, int *spaceAndEnterCount, int *undefinedCount) {
  for (int i = 0; i < len; i++) {
    if (str[i] == ' ' || str[i] == '\n') {
      (*spaceAndEnterCount)++;
    } else if (str[i] < 0)
      (*undefinedCount)++;
  }
}

int sentStatus(string_buffer_t *messageInfo, cJSON *ok, char *okText) {

  cJSON *json = cJSON_Parse(messageInfo->ptr);

  ok = cJSON_GetObjectItemCaseSensitive(json, "ok");

  okText = cJSON_Print(ok);

  if (ok != NULL)
    if (strcmp(okText, "false") == 0)
      return 1;

  return 0;
}

void delay(unsigned int seconds) {
  unsigned int ms = 1000 * seconds;
  clock_t      start_time = clock();

  while (clock() < start_time + ms) {
    // Wait for the specified duration
  }
}

static void string_buffer_initialize(string_buffer_t *sb) {
  sb->len = 0;
  sb->ptr = malloc(sb->len + 1); /* will be grown as needed by the realloc belove */
  sb->ptr[0] = '\0';             /* no data at this point */
}

static void string_buffer_finish(string_buffer_t *sb) {
  free(sb->ptr);
  sb->len = 0;
  sb->ptr = NULL;
}

static size_t string_buffer_callback(void *buf, size_t size, size_t nmemb, void *data) {
  string_buffer_t *sb = data;
  size_t           new_len = sb->len + size * nmemb;
  sb->ptr = realloc(sb->ptr, new_len + 1);
  memcpy(sb->ptr + sb->len, buf, size * nmemb);
  sb->ptr[new_len] = '\0';
  sb->len = new_len;
  return size * nmemb;
}

static size_t header_callback(char *buf, size_t size, size_t nmemb, void *data) {
  return string_buffer_callback(buf, size, nmemb, data);
}

static size_t write_callback(void *buf, size_t size, size_t nmemb, void *data) {
  return string_buffer_callback(buf, size, nmemb, data);
}