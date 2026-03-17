execute_process(COMMAND cppcheck
        --enable=warning,performance,portability,information,missingInclude
        --std=c++20
        --template="[{severity}][{id}]{message}{callstack}\(On{file}:{line}\)"
        --verbose
        --quiet
        --suppress=missingIncludeSystem

        ${CMAKE_SOURCE_DIR}/sources
        ${CMAKE_SOURCE_DIR}/includes

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}

        OUTPUT_QUIET
        COMMAND_ERROR_IS_FATAL ANY
)