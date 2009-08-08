OBJS:= syntax/generate_static_lex.o syntax/lex.o syntax/parse.o
BOOST_INC:=/home/dirk/localbuild/boost-trunk
#EXTRAFLAGS:= -O2 -g
EXTRAFLAGS:= -g

#derived variables
INCLUDES:=-I$(BOOST_INC) -I.
CPPFLAGS:=$(INCLUDES) $(EXTRAFLAGS)

all: syntax/lex_static.hpp syntax/lex syntax/parse

clean:
	rm -f  $(OBJS) $(OBJS:.o=.d) $(PROGRAMS) syntax/lex_static.hpp

syntax/generate_static_lex:	 syntax/generate_static_lex.o
	${CXX} $^ -o $@

syntax/parse: syntax/parse.o
	${CXX} $^ -o $@

syntax/lex: syntax/lex.o
	${CXX} $^ -o $@

syntax/lex_static.hpp: syntax/generate_static_lex
	./$^ $@

syntax/parse.d syntax/lex.d: syntax/lex_static.hpp

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$ && \
	sed 's,$(*F)\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.$$$$ > $@ && \
	rm -f $@.$$$$

ifneq ($(MAKECMDGOALS),clean)
sinclude $(OBJS:.o=.d)
endif