CC           := cc
CCFLAGS      := -Wall -Wextra -ggdb
TARGET       := aec
SRCDIR       := src
OBJDIR       := build
INCDIR       := -I./$(SRCDIR)/include
SOURCES      := $(wildcard $(SRCDIR)/*.c)
OBJECTS      := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TESTDIR      := test
TESTTARGET   := $(TESTDIR)/testrunner
TESTSRCDIR   := $(TESTDIR)/src
TESTOBJDIR   := $(TESTDIR)/build
TESTINCDIR   := -I./$(TESTSRCDIR)/include
TESTSOURCES  := $(wildcard $(TESTSRCDIR)/*.c)
TESTOBJECTS  := $(TESTSOURCES:$(TESTSRCDIR)/%.c=$(TESTOBJDIR)/%.o)

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

cleanall: clean testclean

clean:
	rm -rf $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CCFLAGS) $(INCDIR) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CCFLAGS) $^ -o $@

test: $(TESTTARGET)
	./$(TESTTARGET)

testclean:
	rm -rf $(TESTOBJDIR) $(TEST)

$(TESTOBJDIR):
	mkdir -p $@

$(TESTOBJDIR)/%.o: $(TESTSRCDIR)/%.c | $(TESTOBJDIR)
	$(CC) $(CCFLAGS) $(TESTINCDIR) -c $< -o $@

$(TESTTARGET): $(TESTOBJECTS)
	$(CC) $(CCFLAGS) $^ -o $@
