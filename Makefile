main: format
	g++ main.cpp -Wextra -Wall -o main.out

format:
	clang-format -i main.cpp

clean:
	rm -vf *.out
