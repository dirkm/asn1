OBJS:= syntax/generate_static_lex.o syntax/lex.o syntax/parse.o
BOOST_INC:=/home/dirk/localbuild/boost-trunk
#EXTRAFLAGS:= -O2 -g
EXTRAFLAGS:= -g

#derived variables
INCLUDES:=-I$(BOOST_INC) -I.
CPPFLAGS:=$(INCLUDES) $(EXTRAFLAGS)

all: syntax/lex_static.hpp syntax/lex syntax/parse

syntax/generate_static_lex: syntax/generate_static_lex.o
	${CXX} $^ -o $@

syntax/parse: syntax/parse.o
	${CXX} $^ -o $@

syntax/lex: syntax/lex.o
	${CXX} $^ -o $@

syntax/lex_static.hpp: syntax/generate_static_lex
	./$^ $@

clean:
	rm -f  $(OBJS) $(OBJS:.o=.d) $(PROGRAMS) syntax/lex_static.hpp

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$ && \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@ && \
	rm -f $@.$$$$

syntax/parse.d syntax/lex.d: syntax/lex_static.hpp

include $(OBJS:.o=.d)
