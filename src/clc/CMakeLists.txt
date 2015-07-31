project(clc)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	)

list(APPEND CLC_SOURCES
	algorithm/Bitops.h
	data/BitMask.cpp
	data/BitMask.h
	data/BitString.cpp
	data/BitString.h
	data/Buffer.cpp
	data/Buffer.h
	os/Clock.cpp
	os/Clock.h
	storage/Path.cpp
	storage/Path.h
	support/Intrinsics.h
	support/ByteOrder.h
	support/Debug.cpp
	support/Debug.h
	support/Exception.h
	support/LogAppenders.h
	support/Logger.cpp
	support/Logger.h
	support/Debug.h
	support/Debug.cpp
)

add_library(clc STATIC ${CLC_SOURCES})

#add_subdirectory(test)
