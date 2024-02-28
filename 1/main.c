/***
1-) https://curl.se/windows/dl-8.4.0_3/curl-8.4.0_3-win32-mingw.zip indir
2-) indirilen zip dosyasını bilgisayarda herhangi bir yere aç. ben C:\MinGW dizini içine açtım.
3-) örnek c dosyasını main.c adı ile oluşturdum.
4-) derlemek için komut satırından şunu kullandım
    gcc .\main.c -IC:\MinGW\curl-8.4.0_3-win32-mingw\include -LC:\MinGW\curl-8.4.0_3-win32-mingw\lib -lcurl
5-) bu şekilde derleme tamamlanıp exe dosyası oluştu. bende a.exe dosyası oluştu.
6-) a.exe dosyasının bulunduğu dizini içine C:\MinGW\curl-8.4.0_3-win32-mingw\bin dizini içinde bulunan libcurl.dll
	dosyasını kopyaladım.
7-) a.exe dosyasını komut satırından çalıştırdığımda "https://www.pamukkale.bel.tr" sitesinin html kodlarından 1000 satırını
	ekrana bastı.

***/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <curl/curl.h>

void getWebsitehtmlcode(const char *url);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

char *bg_utf8_ISO88591(char *str);
char *bg_replace_substr(const char *str, const char *find, const char *replace);

struct MemoryStruct {
  char *memory;
  size_t size;
};

int main(void)
{
    // Türkçe karakterler için
	setlocale(LC_ALL, "Turkish");

    getWebsitehtmlcode("https://www.pamukkale.bel.tr");

    return 0;
}

void getWebsitehtmlcode(const char *url)
{
  char *rdizi;
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);  // Okunan bilginin büyüklüğüne göre realloc() fonksiyonu ile artırılır.
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  // curl oturumu açma
  curl_handle = curl_easy_init();

  if(curl_handle) {
     // Web site adresi
     curl_easy_setopt(curl_handle, CURLOPT_URL, url);

     curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L); // https için gerekli
     curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L); // https için gerekli

     // Alınan veriler bu fonksiyona gönderilir.
     curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
     curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
     curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

     // İşlem gerçekleştirme
     res = curl_easy_perform(curl_handle);

     // Hata kontrolu
     if(res != CURLE_OK) {
        printf("curl_easy_perform() hatası: %s\n", curl_easy_strerror(res));
     }
     else {
        printf("Alınan byte sayısı: %lu\n", (unsigned long)chunk.size);

        if(strstr(chunk.memory, "charset=utf-8") || strstr(chunk.memory, "charset=UTF-8")) {
           rdizi = bg_utf8_ISO88591(chunk.memory);
           printf("%.1000s\n", rdizi);
        }
        else printf("%.1000s\n", chunk.memory);
     }

     curl_easy_cleanup(curl_handle);
  }

  free(chunk.memory);
  free(rdizi);

  curl_global_cleanup();
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
     printf("Yetersiz bellek\n");
     return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

char *bg_utf8_ISO88591(char *str)
{
  char *nstr = malloc(strlen(str)+1);
  nstr = str;
  char *sdizi;
  char utf8_char[][3] = { "\xC4\xB1", "\xC3\xBC", "\xC4\x9F", "\xC5\x9F", "\xC3\xA7", "\xC3\xB6", "\xC3\x9C",
                          "\xC4\xB0", "\xC3\x87", "\xC3\x96", "\xC4\x9E", "\xC5\x9E" };
  char tr_char[][2] = { "ı", "ü", "ğ", "ş", "ç", "ö", "Ü", "İ", "Ç", "Ö", "Ğ", "Ş" };
  int id;

  for (id=0; id<12; id++) {
       sdizi = bg_replace_substr(nstr, utf8_char[id], tr_char[id]);
       nstr = realloc(nstr, strlen(sdizi)+1);
       strcpy(nstr, sdizi);
       free(sdizi);
  }

  return nstr;
}

char *bg_replace_substr(const char *str, const char *find, const char *replace)
{
  unsigned find_len, replace_len;
  char *newstr;
  unsigned int newstr_size;      // Yeni oluşturulan karakter dizisinin boyutu
  unsigned int newstr_act = 0;   // Yeni oluşturulan karakter dizisinin aktif konumu
  char *pfind;                   // Değiştirilecek olan karakter dizisinin adresi

  find_len = strlen(find);       // Değiştirilecek olan karakter dizisinin boyutu

  // Ana karakter dizisi veya değiştirilecek karakter dizisi boş ise
  if (!strlen(str) || !find_len) return NULL;

  replace_len = strlen(replace);
  newstr_size = strlen(str) + 1;
  newstr = malloc(newstr_size);

  while ((pfind = strstr(str, find))) {
     // Bulunan karakter dizisine kadar olan karakterleri newstr belleğine kopyalama
     memcpy(newstr + newstr_act, str, pfind - str);
     // Bulunan karakter dizisinin başlangıç adresinin
     // orjinal str işaretçisi ile olan farkını newstr_act değerine ekleme
     newstr_act += pfind - str;
     // Orjinal karakter dizisi adresini, bulunan karakter dizisinin adresinden
     // find uzunluğu kadar ileri alma
     str = pfind + find_len;
     // Yeni oluşturulan karakter dizisine eklenecek veya çıkarılacak olan karakter
     // sayısına göre newstr_size boyutunu hesaplama
     newstr_size = newstr_size - find_len + replace_len;
     // Yeni oluşturulan karakter dizisinin bellek ayırma işlemini yenileme
     newstr = realloc(newstr, newstr_size);
     // replace karakter dizisini newstr dizisine ekleme
     memcpy(newstr + newstr_act, replace, replace_len);
     // newstr_act değerini replace uzunluğu kadar artırma
     newstr_act += replace_len;
  }

  // Kalan değerleri newstr belleğine kopyalama
  memcpy(newstr + newstr_act, str, strlen(str));
  newstr_act += strlen(str);
  *(newstr+newstr_act) = '\0';

  return newstr;
}