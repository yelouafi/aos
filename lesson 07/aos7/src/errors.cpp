#include "./include/system.h"

static char* errorMsgs[] = {
	"",
	"áÇ ÊæÌÏ ĞÇßÑÉ ßÇİíÉ",
	"ŞíãÉ ÎÇØÆÉ áÚÇãá ÇáÏÇáÉ"
};

void error(int err) {
	printf(errorMsgs[-err]);
}
