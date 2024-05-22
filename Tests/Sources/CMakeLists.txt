add_library(Test_Lib
	Test_LibStack.c
	Test_LibQueuingMachine.c
	Test_LibPFRingPacketLost.c
	Test_LibFileSystem.c
)
target_include_directories(Test_Lib PUBLIC ../Headers)