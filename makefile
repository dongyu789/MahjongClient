main:main.cpp src/MyPrint.cpp src/Mahjong.cpp src/Controller.cpp src/MySocket.cpp src/MyTest.cpp src/Win.cpp
	g++ -static -std=c++11 main.cpp src/MyPrint.cpp src/Mahjong.cpp src/Controller.cpp src/MySocket.cpp src/MyTest.cpp src/Win.cpp -o main -I headers -lws2_32