TARGET   = count
OBJS     = count.o
FLAGS    = -g -O0 -Wall
INCLUDES =
LIBS     =

all       : $(TARGET)

clean     :
	rm -f $(TARGET) $(OBJS)

$(TARGET) : $(OBJS)
	g++ $(FLAGS) -o $@ $(OBJS) $(LIBS)

%.o       : %.cpp
	g++ $(FLAGS) $(INCLUDES) -o $@ -c $<

