ARCH=x86_64
CFLAGS=-Wall -Wextra -std=c11 -pedantic -arch $(ARCH)
#SDL_HEADERS=-I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers
#SDL_LIBS=-F/Library/Frameworks -framework SDL2 -framework SDL2_image

TARGET_EXEC ?= a.out

BUILD_ROOT_DIR ?= ./build
BUILD_DIR ?= $(BUILD_ROOT_DIR)
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)


INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) $(SDL_HEADERS) -MMD -MP
LDFLAGS ?= -arch $(ARCH) $(SDL_LIBS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source --> $(OBJS)
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(NO_DEBUG_FLAGS) $(DO_DEBUG_FLAGS) -c $< -o $@


debug: DO_DEBUG_FLAGS = -ggdb -D__DEBUG__=1
debug: $(BUILD_DIR)/$(TARGET_EXEC)
		$(CC) $(OBJS) -o $@ $(LDFLAGS)

release: NO_DEBUG_FLAGS ?= -U__DEBUG__
release: $(BUILD_DIR)/$(TARGET_EXEC)
		$(CC) $(OBJS) -o $@ $(LDFLAGS)

test: debug
	$(BUILD_DIR)/a.out

.PHONY: clean

clean:
	$(RM) -r $(BUILD_ROOT_DIR)


-include $(DEPS)

MKDIR_P ?= mkdir -p

