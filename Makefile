# Main
PROGRAM = TinyShell
OBJS = Main.o CreateTCPClientSocket.o Login.o ClientMessanger.o ClientMemo.o HandleCmd.o HashPass.o CreateTCPServerSocket.o AcceptTCPConnection.o ProcessMain.o HandleTCPClient.o DieWithError.o InterruptSignalHandler.o
CC = gcc
CFLAGS = -Wall -O0
LIBRARIES = -lcrypt

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) $^ $(CFLAGS) $(LIBRARIES) -o $(PROGRAM)
	$(RM) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROGRAM) $(OBJS)
