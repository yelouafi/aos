#include "system.h"
#include "video.h"

static char* errorMsgs[] = {
	"",
	"áÇ ÊæÌÏ ÐÇßÑÉ ßÇÝíÉ",
	"ÞíãÉ ÎÇØÆÉ áÚÇãá ÇáÏÇáÉ"
};

void error(Video *v, int err) {
	v->printf(errorMsgs[-err]);
}
