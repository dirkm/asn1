OBJS:= syntax/generate_static_lex.o syntax/lex.o
TEST_OBJS:=test/syntax.o

ALL_OBJS:=$(OBJS) $(TEST_OBJS)

BOOST_INC:=/home/dirk/localbuild/boost-trunk
#BOOST_LIB:=/home/dirk/local/lib
#EXTRAFLAGS:= -O2 -g
EXTRAFLAGS:= 

#derived variables
INCLUDES:=-I$(BOOST_INC) -I.
CPPFLAGS:=$(INCLUDES) $(EXTRAFLAGS)

all: syntax/lex_static.hpp test/syntax syntax/lex 

clean:
	rm -f  $(ALL_OBJS) $(ALL_OBJS:.o=.d) $(PROGRAMS) syntax/lex_static.hpp

syntax/generate_static_lex: syntax/generate_static_lex.o
	${CXX} $^ -o $@

test/syntax: test/syntax.o 
	${CXX} $^ -o $@

syntax/lex: syntax/lex.o
	${CXX} $^ -o $@

syntax/lex_static.hpp: syntax/generate_static_lex
	./$^ $@

#explicit because lex_static might not exist
test/syntax.d syntax/lex.d: syntax/lex_static.hpp

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$ && \
	sed 's,$(*F)\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.$$$$ > $@ && \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
sinclude $(ALL_OBJS:.o=.d)
endif
