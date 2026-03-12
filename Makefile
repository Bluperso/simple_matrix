CFLAGS = -Wall -Wextra -Werror -std=c11
FSANITIZE = -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable
SOURCE = src/matrix.c src/tests.c
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage -g
OBJECTS = matrix.o
LINKERS = -lcheck -lm -lpthread -lrt -lsubunit

all: matrix.a

matrix.a: matrix.o
	ar rcs matrix.a $(OBJECTS)

test:
	gcc $(CFLAGS) -g -o tests $(SOURCE) $(LINKERS)
	valgrind --leak-check=full --track-origins=yes --log-file="valgrind_report.txt" ./tests 

gcov_report:
	rm -f *.gcno *.gcda *.gcov
	rm -rf coverage_report
	gcc $(COVERAGE_FLAGS) $(CFLAGS) -c $(SOURCE)
	gcc $(COVERAGE_FLAGS) -o matrix $(SOURCE) $(LINKERS) -lgcov
	./matrix
	gcov -o . $(SOURCE)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report


clean: 
	rm -f tests *.gcno *.gcda *.gcov *.a *.o $(LIBRARY) test valgrind_report.txt matrix
	rm -rf coverage_report *.info

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCE) src/tests.c

matrix.o:
	gcc $(CFLAGS) -c src/matrix.c -lm -o matrix.o