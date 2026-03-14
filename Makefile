main:
	gcc -g -o ytsearch src/main.c src/request.c src/parser.c src/tui.c src/helper.c -lcurl -lcjson
