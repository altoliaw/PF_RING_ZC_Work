#include "../Headers/Test_LibPFRingPacketLost.h"

void Test_LibPFRingPacketLost_insertMultiProcessProcess(void** State) {
    LibPFRingPacketLost* packetLost = NULL;
    packetLost = test_calloc(1, sizeof(LibPFRingPacketLost));
	LibPFRingPacketLost_Construct(packetLost);
	packetLost->pf_execPacketLost(packetLost);
	LibPFRingPacketLost_Destruct(packetLost);
    test_free(packetLost);
}