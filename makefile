COMPILE_CMD = x86_64-w64-mingw32-g++
LINK_CMD = x86_64-w64-mingw32-g++
OBJ_DIR = bin/obj
OUT_DIR = bin/out
DEBUG_CC_FLAGS = -ggdb -c -Wall -D cdwDebugMode -D cdwTestBuild
RELEASE_CC_FLAGS = -O3 -c -Wall
DEBUG_LNK_FLAGS_POST = -ggdb -static-libgcc -static-libstdc++ -static
RELEASE_LNK_FLAGS_POST = -static-libgcc -static-libstdc++ -static

test: \
	debug \

debug: \
	dirs \
	$(OUT_DIR)/debug/gtune.exe \

all: \
	debug \
	$(OUT_DIR)/release/gtune.exe \

clean:
	rm -rf bin

dirs:
	@mkdir -p $(OBJ_DIR)/debug/cmn
	@mkdir -p $(OBJ_DIR)/debug/gtune
	@mkdir -p $(OBJ_DIR)/release/cmn
	@mkdir -p $(OBJ_DIR)/release/gtune
	@mkdir -p $(OUT_DIR)/debug
	@mkdir -p $(OUT_DIR)/release

.PHONY: debug all clean dirs test

# ----------------------------------------------------------------------
# cmn

CMN_SRC = \
	src/cmn/card.cpp \
	src/cmn/lines.cpp \
	src/cmn/textTable.cpp \

CMN_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CMN_SRC)))

$(OUT_DIR)/debug/cmn.lib: $(CMN_DEBUG_OBJ)
	$(info $< --> $@)
	@ar crs $@ $^

$(CMN_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CMN_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CMN_SRC)))

$(OUT_DIR)/release/cmn.lib: $(CMN_RELEASE_OBJ)
	$(info $< --> $@)
	@ar crs $@ $^

$(CMN_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# gtune

GTUNE_SRC = \
	src/gtune/cmdLoad.cpp \
	src/gtune/cmdSchema.cpp \
	src/gtune/cmdTable.cpp \
	src/gtune/defCard.cpp \
	src/gtune/intinput.cpp \
	src/gtune/main.cpp \
	src/gtune/run.cpp \

GTUNE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(GTUNE_SRC)))

$(OUT_DIR)/debug/gtune.exe: $(GTUNE_DEBUG_OBJ) $(OUT_DIR)/debug/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -o $@ $(GTUNE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -lcmn

$(GTUNE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

GTUNE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(GTUNE_SRC)))

$(OUT_DIR)/release/gtune.exe: $(GTUNE_RELEASE_OBJ) $(OUT_DIR)/release/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -o $@ $(GTUNE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -lcmn

$(GTUNE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
