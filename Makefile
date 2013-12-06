count_TARGET    = count
count_OBJS      = count.o
addcount_TARGET = addcount
addcount_OBJS   = addcount.o
FLAGS           = -O2 -Wall
INCLUDES        =
LIBS            =

all : $(count_TARGET) $(addcount_TARGET)

clean :
	rm -f $(count_TARGET) $(count_OBJS) $(addcount_TARGET) $(addcount_OBJS)

$(count_TARGET) : $(count_OBJS)
	g++ $(FLAGS) -o $@ $(count_OBJS) $(LIBS)

$(addcount_TARGET) : $(addcount_OBJS)
	g++ $(FLAGS) -o $@ $(addcount_OBJS) $(LIBS)

%.o : %.cpp
	g++ $(FLAGS) $(INCLUDES) -o $@ -c $<

