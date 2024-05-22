#include <stdlib.h>
#include <stdlib.h>
#include "Headers/LibPFRingPacketLost.h"

int main() {
    LibPFRingPacketLost* packetLost = NULL;
    packetLost = calloc(1, sizeof(LibPFRingPacketLost));
	LibPFRingPacketLost_Construct(packetLost);
	packetLost->pf_execPacketLost(packetLost);
	LibPFRingPacketLost_Destruct(packetLost);
    free(packetLost);
    return 0;
}