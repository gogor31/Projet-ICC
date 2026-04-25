# Compilateur et options de compilation
CXX = g++
PKG_CONFIG = pkg-config
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 $(shell $(PKG_CONFIG) --cflags gtkmm-4.0)
LDLIBS = $(shell $(PKG_CONFIG) --libs gtkmm-4.0)

# Nom de l'exécutable
TARGET = project

# Liste des fichiers objets (un pour chaque fichier source .cc)
OFILES = project.o game.o ball.o brick.o paddle.o tools.o message.o gui.o graphic.o

# Règle principale : lie les fichiers objets pour créer l'exécutable
$(TARGET): $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o $(TARGET) $(LDLIBS)

# Règles de compilation pour chaque module (création des .o)
# Les dépendances (.h) assurent que si un header est modifié, le .o est recompilé
project.o: project.cc game.h
	$(CXX) $(CXXFLAGS) -c project.cc

game.o: game.cc game.h ball.h brick.h paddle.h message.h constants.h
	$(CXX) $(CXXFLAGS) -c game.cc

ball.o: ball.cc ball.h tools.h constants.h message.h
	$(CXX) $(CXXFLAGS) -c ball.cc

brick.o: brick.cc brick.h tools.h constants.h
	$(CXX) $(CXXFLAGS) -c brick.cc

paddle.o: paddle.cc paddle.h tools.h constants.h
	$(CXX) $(CXXFLAGS) -c paddle.cc

tools.o: tools.cc tools.h
	$(CXX) $(CXXFLAGS) -c tools.cc

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c message.cc

gui.o: gui.cc gui.h game.h graphic.h
	$(CXX) $(CXXFLAGS) -c gui.cc

graphic.o: graphic.cc graphic.h graphic_gui.h
	$(CXX) $(CXXFLAGS) -c graphic.cc

# Règle pour nettoyer le répertoire (supprime les .o et l'exécutable)
clean:
	rm -f $(OFILES) $(TARGET)