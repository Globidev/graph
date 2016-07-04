NAME            =   nav-route

rwildcard       =   $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)\
                    $(filter $(subst *,%,$2),$d))

COMPILER        =   g++

PCH             =   srcs/stdafx.hpp
PCH_SUFFIX      =   .gch
PCH_DIR         =   /tmp/pch_$(NAME)
PCH_TARGET      =   $(PCH_DIR)$(PCH_SUFFIX)/pch
PCH_FLAG        =   -include $(PCH_DIR)
PCH_IGN_FLAGS   =   -Wno-deprecated-declarations -Wno-unused-parameter

SRCS_DIR        =   ./srcs
INCLUDE_DIRS    =   ./srcs \
                    $(BOOST_INCLUDE_DIR) \
                    $(OSMIUM_INCLUDE_DIR)
LIB_DIRS        =   $(BOOST_LIB_DIR)
LIB_NAMES       =   cppnetlib-server-parsers \
                    cppnetlib-client-connections \
                    cppnetlib-uri \
                    boost_program_options \
                    boost_serialization \
                    z \
                    bz2 \
                    expat

CFLAGS          =   -Wall -Wextra -std=c++1z -O3\
                    -Wno-maybe-uninitialized\
                    $(foreach dir, $(INCLUDE_DIRS), -I $(dir))\
                    $(foreach define, $(PP_DEFINES), -D $(define))
LFLAGS          =   $(foreach dir, $(LIB_DIRS), -L $(dir))\
                    $(foreach name, $(LIB_NAMES), -l$(name))\
                    -static \
                    -Wl,--whole-archive -lpthread -Wl,--no-whole-archive # see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52590

OBJ_DIR         =   objs
SRCS            =   $(call rwildcard, $(SRCS_DIR), *.cpp)
OBJS            =   $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

OBJ_SUB_DIRS    =   $(dir $(OBJS))

all: show_fancy_name
	@$(MAKE) --no-print-directory $(NAME)

$(NAME): $(OBJS)
	@echo "$(bold)$(blue)linking  	$(green)$@$(reset)"
	@$(COMPILER) $^ $(LFLAGS) -o $@
	@echo "$(bold)$(green)build    	✓$(reset)"

$(PCH_TARGET): $(PCH)
	@mkdir -p $(PCH_DIR)$(PCH_SUFFIX)
	@echo "$(bold)$(cyan)precompiling	$(reset)$(white)$(dir $<)$(bold)$(notdir $<)$(reset)"
	@$(COMPILER) $(CFLAGS) $(PCH_IGN_FLAGS) $< -o $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	@$(foreach dir, $(OBJ_SUB_DIRS), mkdir -p $(dir);)

$(OBJ_DIR)/%.o: %.cpp $(PCH_TARGET)
	@echo "$(bold)$(cyan)compiling	$(reset)$(white)$(dir $<)$(bold)$(notdir $<)$(reset)"
	@$(COMPILER) $(CFLAGS) $(PCH_FLAG) -c $< -o $@

depend: .depend

.depend: $(SRCS)
	@rm -f ./.depend
	@$(foreach src, $^, $(COMPILER) $(CFLAGS) -MM -MT $(OBJ_DIR)/./$(src:.cpp=.o) $(src) >> ./.depend;)

-include .depend

clean: show_fancy_name
	@echo "$(bold)$(red)cleaning 	$(white)$(OBJ_DIR)$(reset)"
	@rm -rf $(OBJ_DIR)
	@rm -f ./.depend
	@echo "$(bold)$(red)cleaning 	$(white)$(PCH_DIR)$(reset)"
	@rm -rf $(PCH_DIR)$(PCH_SUFFIX)

fclean: clean
	@echo "$(bold)$(red)removing 	$(green)$(NAME)$(reset)"
	@rm -f $(NAME)

re:
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

show_fancy_name:
	@echo "\n$(bold)$(yellow)building 	$(NAME)$(reset)"


# ANSI helpers
ansi        =   \033[$1m

bold        =   $(call ansi,1)
fg_color    =   $(call ansi,38;5;$1)
reset       =   $(call ansi,0)

red         =   $(call fg_color,1)
green       =   $(call fg_color,10)
blue        =   $(call fg_color,27)
cyan        =   $(call fg_color,6)
yellow      =   $(call fg_color,11)
white       =   $(call fg_color,255)
