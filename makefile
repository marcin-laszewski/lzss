CFLAGS	+= -Wall
CFLAGS	+= -I.
CFLAGS	+= -g

C_NAME	= lzss-c
C_OUT	= $(C_NAME)
C_OBJS	= \
	buffer.o \
	c-main.o \
	encode.o \

D_NAME	= lzss-d
D_OUT	= $(D_NAME)
D_OBJS	= \
	buffer.o \
	d-main.o \
	decode.o \

all:	$(C_OUT) $(D_OUT)

$(C_OUT): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(D_OUT): $(D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

distclean: clean
	$(RM) $(C_OUT) $(D_OUT) *~

clean::
	$(RM) $(C_OBJS) $(D_OBJS)
