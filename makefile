CFLAGS	+= -Wall
CFLAGS	+= -I.
CFLAGS	+= -g

C_NAME	= lzss-c
C_OUT	= $(C_NAME)
C_OBJS	= \
	c-main.o \
	encode.o \

D_NAME	= lzss-d
D_OUT	= $(D_NAME)
D_OBJS	= \
	d-main.o \
	decode.o \

EXEC_D_NAME	= lzss-exec-d
EXEC_D_OUT	= $(EXEC_D_NAME)
EXEC_D_OBJS	= \
	exec-d.o \
	decode.o \

all:	$(C_OUT) $(D_OUT) $(EXEC_D_OUT)

$(C_OUT): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(D_OUT): $(D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(EXEC_D_OUT): $(EXEC_D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

distclean: clean
	$(RM) $(C_OUT) $(D_OUT) $(EXEC_D_OUT) *~

clean::
	$(RM) $(C_OBJS) $(D_OBJS) $(EXEC_D_OBJS)
