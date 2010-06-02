# Mozilla - structure of Installable Bundle
# https://developer.mozilla.org/en/Bundles#Platform-specific_Subdirectories

if(APPLE)
	message(STATUS "${CMAKE_SYSTEM_NAME} == Darwin")
	set(MOZILLA_PLATFORM Darwin)
	set(MOZILLA_COMPILER gcc3)
elseif(WIN32)
	set(MOZILLA_PLATFORM WINNT)
	set(MOZILLA_COMPILER msvc)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	set(MOZILLA_PLATFORM Linux)
	set(MOZILLA_COMPILER gcc3)
else()
	message(FATAL_ERROR "Not implemented for ${CMAKE_SYSTEM_NAME} yet.")
endif()

if("${ARCH}" STREQUAL "")
	set(MOZILLA_ARCH ${CMAKE_SYSTEM_PROCESSOR})
else()
	set(MOZILLA_ARCH ${ARCH})
endif()

set(MOZILLA_TARGET "${MOZILLA_PLATFORM}_${MOZILLA_ARCH}-${MOZILLA_COMPILER}")
set(MOZILLA_PATH   "platform/${MOZILLA_TARGET}/plugins/")