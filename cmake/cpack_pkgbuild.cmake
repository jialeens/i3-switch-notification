# CPack script for building an archlinux package
FIND_PROGRAM(MAKEPKG makepkg)
IF(MAKEPKG STREQUAL makepkg-NOTFOUND)
  MESSAGE(STATUS "Could not find makepkg, skipping archlinux package")
  RETURN()
ENDIF()

EXECUTE_PROCESS(COMMAND ${MAKEPKG} "-f" ".")
