R = R
BISON = bison
FLEX = flex
MV = mv
CD = cd
RM = rm
CP = cp

FLEXFLAGS = -v
BISONFLAGS = -v

GRAMMARDIR = inst/grammar
SRCDIR = src

.PHONY: all build check document test grammar

all: document build check install

build: document
	$(R) CMD build .

check: build
	$(R) CMD check strictr*tar.gz

clean:
	-$(RM) -f strictr*tar.gz
	-$(RM) -fr strictr.Rcheck
	-$(RM) -rf src/*.o src/*.so

document:
	$(R) -e 'devtools::document()'

test:
	$(R) -e 'devtools::test()'

lintr:
	$(R) --slave -e "lintr::lint_package()"

install: clean
	$(R) CMD INSTALL .

grammar:
	$(CD) $(GRAMMARDIR) && $(BISON) $(BISONFLAGS) --xml --graph=Parser.gv Parser.yxx
	$(MV) $(GRAMMARDIR)/Parser_autogen.cpp $(SRCDIR)
	$(MV) $(GRAMMARDIR)/Parser_autogen.h $(SRCDIR)
	$(MV) $(GRAMMARDIR)/location.hh $(SRCDIR)
	$(CD) $(GRAMMARDIR) && $(FLEX) $(FLEXFLAGS) Lexer.lxx
	$(MV) $(GRAMMARDIR)/Lexer_autogen.cpp $(SRCDIR)
	$(MV) $(GRAMMARDIR)/Lexer_autogen.h $(SRCDIR)
