#include "./include/system.h"

static char* errorMsgs[] = {
	"",
	"�� ���� ����� �����",
	"���� ����� ����� ������"
};

void error(int err) {
	printf(errorMsgs[-err]);
}
