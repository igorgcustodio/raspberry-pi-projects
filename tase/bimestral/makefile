PROJETO	:= program

DESC    := 0

CC 		:= gcc
FLAGS	:= -lpaho-mqtt3c -Wall -lsqlite3 -lwiringPi -lwiringPiDev
SRCDIR	:= .
OBJDIR  := obj

SRCS    := $(shell find $(SRCDIR) -name '*.c')
SRCDIRS := $(shell find . -name '*.c' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: $(PROJETO)

$(PROJETO): buildrepo $(OBJS)
ifeq ($(DESC),mostra)
	$(CC) $(OBJS) $(FLAGS) -s -o $@
	@echo "EXECUTAVEL $(PROJETO) GERADA COM SUCESSO"
	@echo "GERADADA EM: $$(date)"
else
	$(CC) $(OBJS) $(FLAGS) -s -o $@
endif

$(OBJDIR)/%.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: unbuildrepo clean
	$(RM) $(PROJETO)

buildrepo:
	@$(call make-repo)

unbuildrepo:
	@$(call unmake-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
	mkdir -p $(OBJDIR)/$$dir; \
   done
endef

define unmake-repo
   for dir in $(SRCDIRS); \
   do \
	rm -rf $(OBJDIR)/$$dir; \
   done
endef