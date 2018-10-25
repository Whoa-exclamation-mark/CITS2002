# CITS2002 Project 2018
# Name(s):		Campbell J.H. Millar
# Student number(s):	22510848

# Make/Bake file for building Bake!

C99     =  cc -std=gnu99
CFLAGS  =  -Wall -pedantic -Werror -g -std=c99

bake : main.o runner.o phaser.o print_log.o lexicon.o history.o helpers.o command.o stack.o url_history.o file_history.o
	$(C99) $(CFLAGS) -o bake \
		main.o print_log.o runner.o phaser.o lexicon.o history.o helpers.o command.o stack.o url_history.o file_history.o -lm

main.o: src/main.c src/lexicon/lexicon.h src/phaser/phaser.h src/runner/runner.h src/helpers/helpers.h src/constants.h src/logger/logger.h
	$(C99) $(CFLAGS) -c src/main.c

runner.o: src/runner/runner.c src/runner/runner.h src/history/history.h src/phaser/phaser.h src/command/command.h src/logger/logger.h
	$(C99) $(CFLAGS) -c src/runner/runner.c

phaser.o: src/phaser/phaser.c src/phaser/phaser.h src/adt/stack.h src/helpers/helpers.h src/history/history.h src/runner/runner.h src/logger/logger.h
	$(C99) $(CFLAGS) -c src/phaser/phaser.c

print_log.o: src/logger/print_log.c src/logger/logger.h src/constants.h
	$(C99) $(CFLAGS) -c src/logger/print_log.c

lexicon.o: src/lexicon/lexicon.c src/lexicon/lexicon.h src/helpers/helpers.h src/phaser/phaser.h src/constants.h src/logger/logger.h
	$(C99) $(CFLAGS) -c src/lexicon/lexicon.c

history.o: src/history/history.c src/history/history.h
	$(C99) $(CFLAGS) -c src/history/history.c

url_history.o: src/history/url_history.c src/history/history.h src/logger/logger.h src/helpers/helpers.h
	$(C99) $(CFLAGS) -c src/history/url_history.c

file_history.o: src/history/file_history.c src/history/history.h src/helpers/helpers.h
	$(C99) $(CFLAGS) -c src/history/file_history.c

helpers.o: src/helpers/helpers.c src/helpers/helpers.h src/logger/logger.h
	$(C99) $(CFLAGS) -c src/helpers/helpers.c

command.o: src/command/command.c src/command/command.h src/logger/logger.h src/constants.h
	$(C99) $(CFLAGS) -c src/command/command.c

stack.o: src/adt/stack.c src/adt/stack.h src/logger/logger.h src/helpers/helpers.h
	$(C99) $(CFLAGS) -c src/adt/stack.c

clean:
	rm -rf bake *.o
