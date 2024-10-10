# src/CMakeLists.txt

# Define library sources
set(DAMLEV_SOURCES
        algorithms/damlev.cpp
        algorithms/damlevp.cpp  ##TODO rename to damlSim
        #algorithms/damlevlim_p.cpp   ## TODO Need to add this with a limit, say look for words with 80% similar.. damlSimlimit
        #algorithms/damlevlimmin_p.cpp   ## TODO Need to add this with a limit, say look for words with 80% similar.. damlSimlimitMin
        algorithms/damlevlim.cpp
        algorithms/damlevconst.cpp
        algorithms/damlevconstmin.cpp
        algorithms/damlev2D.cpp
        algorithms/noop.cpp
)

# Modify the buffer size if needed (default is 4096ull)
set(BUFFER_SIZE 4096ull)

# Add the damlev library as SHARED
add_library(damlev SHARED ${DAMLEV_SOURCES})

# Set compile definitions
target_compile_definitions(damlev PRIVATE
        WORDS_PATH="/usr/share/dict/words"
        MYSQL_DYNAMIC_PLUGIN
        DAMLEV_BUFFER_SIZE=${BUFFER_SIZE}
)

# MySQL Include Directory
# Get the MySQL include flags
if(NOT DEFINED MYSQL_INCLUDE_FLAG)
    execute_process(COMMAND mysql_config --include
            OUTPUT_VARIABLE MYSQL_INCLUDE_FLAG
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # Fallback for Win64
    if(WIN64 AND NOT DEFINED MYSQL_INCLUDE_FLAG)
        set(MYSQL_INCLUDE_FLAG "-I C:/Program Files/MySQL/MySQL Server 8.0/include")
    endif()
endif()

# Remove the '-I' prefix to get the include directory
string(REGEX REPLACE "^-I(.*)" "\\1" MYSQL_INCLUDE ${MYSQL_INCLUDE_FLAG})

message(STATUS "Include dir: ${MYSQL_INCLUDE}")

# Include directories with PUBLIC scope to propagate to linked targets
target_include_directories(damlev PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/include
        ${MYSQL_INCLUDE}
)
### Library Installation ###

# Determine MYSQL_PLUGIN_DIR using mysql_config
if(NOT DEFINED MYSQL_PLUGIN_DIR)
    if(INSTALL_PLUGINDIR)
        set(MYSQL_PLUGIN_DIR ${INSTALL_PLUGINDIR})
    else()
        # Note: `mysql_config` must be in the PATH for this to work.
        execute_process(COMMAND mysql_config --plugindir
                OUTPUT_VARIABLE MYSQL_PLUGIN_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()
endif()
message(STATUS "Plugin dir: ${MYSQL_PLUGIN_DIR}")

# Install the damlev library
install(TARGETS damlev LIBRARY DESTINATION ${MYSQL_PLUGIN_DIR})
