# get git commit sha
EXEC_PROGRAM(
        "git"
        packaging
        ARGS "log" "-1" "--format=\"%h\""
        OUTPUT_VARIABLE COMMIT_SHA
        RETURN_VALUE GIT_RETURN
)

# set commit sha
IF("${GIT_RETURN}" EQUAL "0")
    SET(PROJECT_COMMIT_SHA "${COMMIT_SHA}")
ENDIF()