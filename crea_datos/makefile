CC = g++-6
SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc,%.o,$(SRCS))
target = main.e
FLGS = 

all: $(target) clean

$(target): $(OBJS)
	@$(CC) -o $@ $^
$(OBJS): %.o:%.cc
	@$(CC) -c $< -o $@

clean:
	@rm -f *.o *~
borra:
	@rm -f *.o *.exe *~
run:
	@./$(target)
