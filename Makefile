CC			:=	gcc 
CPPFLAGS	:=  -Wall
INCLUDE		:= 	-I include/ -I test/
BIN 		:= 	bin
SRC 		:= 	src
TEST_SRC 	:=  test
TEST_FRAM	:=  test/sunittest

test: $(TEST_SRC)/test_internal
	./$(TEST_SRC)/test_internal

test_internal: $(TEST_SRC)/test_bigint_internal.c bigint.o hashmap.o sunittest.o
	$(CC) $(CPPFLAGS) $(TEST_SRC)/test_bigint_internal.c $(BIN)/bigint.o $(BIN)/hashmap.o $(TEST_FRAM)/sunittest.o $(INCLUDE) -o $(TEST_SRC)/test_internal

sunittest.o : $(TEST_FRAM)/sunittest.c
	$(CC) $(CPPFLAGS) -c $(TEST_FRAM)/sunittest.c -o $(TEST_FRAM)/sunittest.o $(INCLUDE)

bigint.o: $(SRC)/bigint.c
	$(CC) $(CPPFLAGS) -c $(SRC)/bigint.c -o $(BIN)/bigint.o $(INCLUDE)

hashmap.o: $(SRC)/hashmap.c
	$(CC) $(CPPFLAGS) -c $(SRC)/hashmap.c -o $(BIN)/hashmap.o $(INCLUDE)

.PHONY: test
