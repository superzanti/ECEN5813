#Project 4 Readme File

things that need doing:

implement RTC setup in logger.c-\>logger\_init

put RTC timestamp into a few locations in logger.c

implement log\_flush for KL25z

make project4.c, this needs to be project2.c, but with modifications to reporting code

add in log commands to many locations.

surround log commands with #ifdef LOGGING

update Makefile to add LOGGING as an option

add KL25z block on waiting for python script

figure out how to pipe output from BBB and HOST into a file (maybe use Tee?)

debug all the shit
