make: client server

client:
	gcc client.c -o client

server:
	gcc server.c -o server

run-server:
	./server &

run-client:
	./client shubham:sharma@127.0.0.1

clean:
	rm server client
