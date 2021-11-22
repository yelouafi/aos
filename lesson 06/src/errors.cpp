#include "system.h"
#include "video.h"

static char* errorMsgs[] = {
	"",
	"�� ���� ����� �����",
	"���� ����� ����� ������"
};

void error(Video *v, int err) {
	v->printf(errorMsgs[-err]);
}
