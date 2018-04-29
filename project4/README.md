#Project 4 Readme File

things that need doing:

add in log commands to many locations.
    SYSTEM_ID
    SYSTEM_VERSION
    GPIO_INITIALIZED
    SYSTEM_HALTED
    WARNING
    ERROR - 1 implemented
    PROFILING_STARTED
    PROFILING_RESULT
    PROFILING_COMPLETED
    CORE_DUMP

    DONE    DATA_RECIEVED
    DONE    DATA_ALPHA_COUNT
    DONE    DATA_NUMERIC_COUNT
    DONE    DATA_PUNCTUATION_COUNT
    DONE    DATA_MISC_COUNT
    DONE    DATA_ANALYSIS_COMPLETED
    DONE    DATA_ANALYSIS_STARTED
    DONE    LOGGER_INITIALIZED
    DONE    SYSTEM_INITIALIZED
    DONE    INFO (COULD BE PLACED IN MORE LOCATIONS)
    DONE    HEARTBEAT



debug all the shit

DONE        surround log commands with #ifdef LOGGING - this has been done for the ones that are in, so far. 

DONE        figure out how to pipe output from BBB and HOST into a file (maybe use Tee?)

DONE        make project4.c, this needs to be project2.c, but with modifications to reporting code

DONE        add KL25z block on waiting for python script

DONE        implement RTC setup in logger.c-\>logger\_init

DONE        put RTC timestamp into a few locations in logger.c

DONE        implement log\_flush for KL25z

DONE        update Makefile to add LOGGING as an option
