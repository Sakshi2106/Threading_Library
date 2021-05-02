OBJ_DIR := obj
SRC_DIR := library
TEST_DIR := testfiles
TEST_OBJ := testobj
EXECUTABLE := exe
CC := gcc

CFLAGS := -I include
.PHONY: all clean run

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TESTS := $(wildcard $(TEST_DIR)/*.c)
TESTOBJ := $(TESTS:$(TEST_DIR)/%.c=$(TEST_OBJ)/%.o)
#$(info SRC is $(SRC))
#$(info OBJ is $(OBJ))
#$(info TESTOBJ is $(TESTOBJ))

EXE := $(addprefix $(EXECUTABLE)/, $(notdir $(basename $(TESTS))))
$(info EXE is $(EXE))

all : $(OBJ) $(TESTOBJ) $(EXE)

$(OBJ_DIR):
	mkdir -p $@

$(TEST_OBJ):
	mkdir -p $@

$(EXECUTABLE):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -Wall -Wno-return-type -c $< -o $@

$(TEST_OBJ)/%.o: $(TEST_DIR)/%.c | $(TEST_OBJ)
	$(CC) $(CFLAGS) -Wall -Wno-return-type -c $< -o $@

$(EXECUTABLE)/%: $(TEST_DIR)/%.o | $(EXECUTABLE)
	$(CC)  $(OBJ) $^ $(LDLIBS) -o $@

clean: 
	$(RM) -rf $(EXECUTABLE) $(OBJ_DIR) $(TEST_OBJ)

run:
	@$(MAKE) && ./test.sh