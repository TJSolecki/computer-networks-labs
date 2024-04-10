make
sh make_files.sh
ls -l
sleep 5

echo "DEMO: part 1"
./1 10 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt 9.txt 10.txt \
    11.txt 12.txt 13.txt 14.txt 15.txt 16.txt 17.txt 18.txt 19.txt 20.txt

ls -l
printf "\n\n"
sleep 5

echo "DEMO: parts 2 & 3"
./2 200 20 0.10 19
printf "\n\n"
sleep 5

echo "DEMO: part 4"
./4 1
printf "\n\n"
sleep 5
printf "\n\n"
./4 4
sleep 5
printf "\n\n"
./4 6
printf "\n\n"
sleep 5
./4 12

rm *.txt
make clean
