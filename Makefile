count_TARGET       = count
count_OBJS         = count.o
addcount_TARGET    = addcount
addcount_OBJS      = addcount.o
threshcount_TARGET = threshcount
threshcount_OBJS   = threshcount.o
sortalph_TARGET    = sortalph
sortalph_OBJS      = sortalph.o
FLAGS              = -O2 -Wall
INCLUDES           =
LIBS               =

all : $(count_TARGET) $(addcount_TARGET) $(threshcount_TARGET) $(sortalph_TARGET)

clean :
	rm -f $(count_TARGET) $(count_OBJS) $(addcount_TARGET) $(addcount_OBJS) \
		$(threshcount_TARGET) $(threshcount_OBJS) $(sortalph_TARGET) $(sortalph_OBJS)

$(count_TARGET) : $(count_OBJS)
	g++ $(FLAGS) -o $@ $(count_OBJS) $(LIBS)

$(addcount_TARGET) : $(addcount_OBJS)
	g++ $(FLAGS) -o $@ $(addcount_OBJS) $(LIBS)

$(threshcount_TARGET) : $(threshcount_OBJS)
	g++ $(FLAGS) -o $@ $(threshcount_OBJS) $(LIBS)

$(sortalph_TARGET) : $(sortalph_OBJS)
	g++ $(FLAGS) -o $@ $(sortalph_OBJS) $(LIBS)

%.o : %.cpp
	g++ $(FLAGS) $(INCLUDES) -o $@ -c $<
