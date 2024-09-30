all: bucketsort bubblesort selectionsort quicksort

.outputFolder:
	mkdir -p out

bucketsort: .outputFolder
	clang -std=c++17 -lstdc++ -lm bucketsort.cpp -Wall -Wextra -Werror -o out/bucketsort
	./out/bucketsort

bubblesort: .outputFolder
	clang -std=c++17 -lstdc++ -lm bubblesort.cpp -Wall -Wextra -Werror -o out/bubblesort
	./out/bubblesort

selectionsort: .outputFolder
	clang -std=c++17 -lstdc++ -lm selectionsort.cpp -Wall -Wextra -Werror -o out/selectionsort
	./out/selectionsort

quicksort: .outputFolder
	clang -std=c++17 -lstdc++ -lm quicksort.cpp -Wall -Wextra -Werror -o out/quicksort
	./out/quicksort


