CC=$(CXX)
OBJECTS := \
	Environment.o \
	Event.o \
	FiniteStateMachine.o \
	FlappyBird.o \
	GameOverState.o \
	Loger.o \
	MainMenuState.o \
	PlayState.o \
	ScoreState.o \
	UniqueIntGenerator.o \

LDFLAGS := -lallegro -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives
BINARY := FlappyBird

$(BINARY): $(OBJECTS)

clean:
	-rm -f $(BINARY) $(OBJECTS)
.PHONY: clean
