add_library(LibStack
    LibStack.c
    LibQueuingMachine.c
    LibPFRingPacketLost.c
    LibFileSystem.c
)
target_include_directories(LibStack PUBLIC ../Headers)