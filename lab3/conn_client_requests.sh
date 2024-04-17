./client 127.0.0.1 8080 1.txt 1.dst.txt &
./client 127.0.0.1 8080 2.txt 2.dst.txt &
./client 127.0.0.1 8080 3.txt 3.dst.txt &
./client 127.0.0.1 8080 4.txt 4.dst.txt &
./client 127.0.0.1 8080 5.txt 5.dst.txt &

sleep 10

./client 127.0.0.1 8080 6.txt 6.dst.txt &
./client 127.0.0.1 8080 7.txt 7.dst.txt &
./client 127.0.0.1 8080 8.txt 8.dst.txt &
./client 127.0.0.1 8080 9.txt 9.dst.txt &
./client 127.0.0.1 8080 10.txt 10.dst.txt &
