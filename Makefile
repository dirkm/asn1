OBJS:= syntax/generate_static_lex.o 
TEST_OBJS:=test/syntax.o test/read_from_file.o

ALL_OBJS:=$(OBJS) $(TEST_OBJS)

BOOST_INC:=/home/dirk/localbuild/boost-trunk
#BOOST_LIB:=/home/dirk/local/lib
#EXTRAFLAGS:= -O2 -g
EXTRAFLAGS:= 

#derived variables
INCLUDES:=-I$(BOOST_INC) -I.
CPPFLAGS:=$(INCLUDES) $(EXTRAFLAGS)

all: syntax/lex_static.hpp test/syntax
clean:
	rm -f  $(ALL_OBJS) $(ALL_OBJS:.o=.d) $(PROGRAMS) syntax/lex_static.hpp

syntax/generate_static_lex: syntax/generate_static_lex.o
	${CXX} $^ -o $@

test/syntax: test/read_from_file.o test/syntax.o 
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
