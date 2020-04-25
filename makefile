CFLAGS	+= -Wall
CFLAGS	+= -I.
#CFLAGS	+= -g
#CFLAGS	+= -O
#CFLAGS	+= -Os
CFLAGS	+= -DCHMOD

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
	decode/init.o \
	getbit.o \

EXEC_D_NAME	= lzss-exec-d
EXEC_D_OUT	= $(EXEC_D_NAME)
EXEC_D_OBJS	= \
	exec-d.o \
	decode.o \
	decode/init.o \
	getbit.o \

GET_NAME	= lzss-get
GET_OUT		= $(GET_NAME)
GET_OBJS	= \
	decode/init.o \
	decode/get.o \
	getbit.o \
	g-main.o \

all:	$(C_OUT) $(D_OUT) $(EXEC_D_OUT) $(GET_OUT)

$(C_OUT): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(D_OUT): $(D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(EXEC_D_OUT): $(EXEC_D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(GET_OUT): $(GET_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

distclean: clean
	find . -name '*~' \
	| xargs $(RM) $(C_OUT) $(D_OUT) $(EXEC_D_OUT) $(GET_OUT)

clean::
	$(RM) $(C_OBJS) $(D_OBJS) $(EXEC_D_OBJS) $(GET_OBJS)
