CC := clang
CFLAGS := -Isrc -pedantic -Wall -Werror -Wextra
OBJS := obj/main.o obj/lexer.o

compile: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o app.exe && app.exe

obj/%.o: src/%.c
	cd obj && $(CC) $(CFLAGS) -c ../$^