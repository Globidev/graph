NAME            =   nav-route

rwildcard       =   $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)\
                    $(filter $(subst *,%,$2),$d))

COMPILER        =   g++

SRCS_DIR        =   ./srcs
INCLUDE_DIRS    =   $(BOOST_INCLUDE_DIR)
LIB_DIRS        =   $(BOOST_LIB_DIR)
LIB_NAMES       =   boost_serialization \
                    cppnetlib-server-parsers \
                    cppnetlib-client-connections \
                    cppnetlib-uri \
                    pthread

CFLAGS          =   -Wall -Wextra -std=c++1z -O3\
                    $(foreach dir, $(INCLUDE_DIRS), -I $(dir))\
                    $(foreach define, $(PP_DEFINES), -D $(define))
LFLAGS          =   $(foreach dir, $(LIB_DIRS), -L $(dir))\
                    $(foreach name, $(LIB_NAMES), -l$(name))

OBJ_DIR         =   objs
SRCS            =   $(call rwildcard, $(SRCS_DIR), *.cpp)
OBJS            =   $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

OBJ_SUB_DIRS    =   $(dir $(OBJS))

all: show_fancy_name
	@$(MAKE) $(NAME)

$(NAME): $(OBJS)
	@echo "$(bold)$(blue)linking  	$(green)$@$(reset)"
	@$(COMPILER) $^ $(LFLAGS) -o $@
	@echo "$(bold)$(green)build    	✓$(reset)"

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	@$(foreach dir, $(OBJ_SUB_DIRS), mkdir -p $(dir);)

$(OBJ_DIR)/%.o: %.cpp
	@echo "$(bold)$(cyan)compiling	$(reset)$(white)$(dir $<)$(bold)$(notdir $<)$(reset)"
	@$(COMPILER) $(CFLAGS) -c $< -o $@

depend: .depend

.depend: $(SRCS)
	@rm -f ./.depend
	@$(foreach src, $^, $(COMPILER) $(CFLAGS) -MM -MT $(OBJ_DIR)/./$(src:.cpp=.o) $(src) >> ./.depend;)

-include .depend

clean: show_fancy_name
	@echo "$(bold)$(red)cleaning 	$(white)$(OBJ_DIR)$(reset)"
	@rm -rf $(OBJ_DIR)
	@rm -f ./.depend

fclean: clean
	@echo "$(bold)$(red)removing 	$(green)$(NAME)$(reset)"
	@rm -f $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

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
