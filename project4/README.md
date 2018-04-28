#Project 4 Readme File

things that need doing:

make project4.c, this needs to be project2.c, but with modifications to reporting code

add in log commands to many locations.
	SYSTEM_ID
	SYSTEM_VERSION
		DONE	LOGGER_INITIALIZED
	GPIO_INITIALIZED
	SYSTEM_INITIALIZED
	SYSTEM_HALTED
	INFO
	WARNING
	ERROR
	PROFILING_STARTED	
	PROFILING_RESULT
	PROFILING_COMPLETED
	DATA_RECIEVED
	DATA_ANALYSIS_STARTED
	DATA_ALPHA_COUNT
	DATA_NUMERIC_COUNT
	DATA_PUNCTUATION_COUNT	
	DATA_MISC_COUNT
	DATA_ANALYSIS_COMPLETED
		DONE	HEARTBEAT
	CORE_DUMP

surround log commands with #ifdef LOGGING

figure out how to pipe output from BBB and HOST into a file (maybe use Tee?)

debug all the shit

DONE		add KL25z block on waiting for python script

DONE 		implement RTC setup in logger.c-\>logger\_init

DONE		put RTC timestamp into a few locations in logger.c

DONE 		implement log\_flush for KL25z

DONE 		update Makefile to add LOGGING as an option
