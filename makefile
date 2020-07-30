CFLAGS	+= -Wall
CFLAGS	+= -I.
#CFLAGS	+= -g
#CFLAGS	+= -O
#CFLAGS	+= -Os
CFLAGS	+= -DCHMOD
CFLAGS	+= -Dlzss_EI=11	# typically 10..13
CFLAGS	+= -Dlzss_EJ=4	# typically 4..5
#CFLAGS	+= -Dlzss_buf_INTERNAL

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

STREAM_NAME	= lzss-stream
STREAM_OUT	= $(STREAM_NAME)
STREAM_OBJS	= \
	decode/init.o \
	stream/get.o \
	stream/open.o \
	getbit.o \
	g-main.o \

all:	$(C_OUT) $(D_OUT) $(EXEC_D_OUT) $(STREAM_OUT)

$(C_OUT): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(D_OUT): $(D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(EXEC_D_OUT): $(EXEC_D_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(STREAM_OUT): $(STREAM_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

distclean: clean
	find . -name '*~' \
	| xargs $(RM) $(C_OUT) $(D_OUT) $(EXEC_D_OUT) $(STREAM_OUT)

clean::
	$(RM) $(C_OBJS) $(D_OBJS) $(EXEC_D_OBJS) $(STREAM_OBJS)
