include config/target.conf
include config/build.conf
include config/options.conf

all: $(TARGET) link clean
link:
	$(LD) $(LFLAGS) $(TARGET) -o $(BIN)kernel
clean:
	$(RM) $(TARGET) 
