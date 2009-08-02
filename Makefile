OBJS = generate_static_lex.o lex.o
BOOST_INC=/home/dirk/localbuild/boost-trunk
#EXTRAFLAGS = -O2 -g 
EXTRAFLAGS = -g 

#derived variables
INCLUDES = -I$(BOOST_INC)
CPPFLAGS = $(INCLUDES) $(EXTRAFLAGS)

PROGRAMS=generate_static_lex lex

all: lex_static.hpp $(PROGRAMS)

generate_static_lex: generate_static_lex.o 

lex: lex.o 

lex_static.hpp:generate_static_lex
	./$^ $@

clean: 
	rm -f  $(OBJS:.o=.o) $(OBJS:.o=.d) $(PROGRAMS) lex_static.hpp

$(PROGRAMS):%:%.o
	${CXX} $^ -o $@

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(OBJS:.o=.d)
