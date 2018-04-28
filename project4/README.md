#Project 4 Readme File

things that need doing:

make project4.c, this needs to be project2.c, but with modifications to reporting code

add in log commands to many locations.

surround log commands with #ifdef LOGGING

figure out how to pipe output from BBB and HOST into a file (maybe use Tee?)

debug all the shit

DONE		add KL25z block on waiting for python script

DONE 		implement RTC setup in logger.c-\>logger\_init

DONE		put RTC timestamp into a few locations in logger.c

DONE 		implement log\_flush for KL25z

DONE 		update Makefile to add LOGGING as an option
