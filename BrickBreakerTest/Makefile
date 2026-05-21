# Compilateur et options de compilation
CXX = g++
PKG_CONFIG = pkg-config
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O3 $(shell $(PKG_CONFIG) --cflags gtkmm-4.0)
LDLIBS = $(shell $(PKG_CONFIG) --libs gtkmm-4.0)

# Nom de l'exécutable
TARGET = project

# Liste des fichiers objets (un pour chaque fichier source .cc)
OFILES = project.o game.o ball.o brick.o paddle.o tools.o message.o gui.o graphic.o

# Règle principale : lie les fichiers objets pour créer l'exécutable
$(TARGET): $(OFILES)
	@echo "Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(OFILES) -o $(TARGET) $(LDLIBS)

# Règles de compilation pour chaque module
project.o: project.cc game.h
	@echo "Compiling project.cc..."
	@$(CXX) $(CXXFLAGS) -c project.cc

game.o: game.cc game.h ball.h brick.h paddle.h message.h constants.h
	@echo "Compiling game.cc..."
	@$(CXX) $(CXXFLAGS) -c game.cc

ball.o: ball.cc ball.h tools.h constants.h message.h
	@echo "Compiling ball.cc..."
	@$(CXX) $(CXXFLAGS) -c ball.cc

brick.o: brick.cc brick.h tools.h constants.h
	@echo "Compiling brick.cc..."
	@$(CXX) $(CXXFLAGS) -c brick.cc

paddle.o: paddle.cc paddle.h tools.h constants.h
	@echo "Compiling paddle.cc..."
	@$(CXX) $(CXXFLAGS) -c paddle.cc

tools.o: tools.cc tools.h
	@echo "Compiling tools.cc..."
	@$(CXX) $(CXXFLAGS) -c tools.cc

message.o: message.cc message.h
	@echo "Compiling message.cc..."
	@$(CXX) $(CXXFLAGS) -c message.cc

gui.o: gui.cc gui.h game.h graphic.h
	@echo "Compiling gui.cc..."
	@$(CXX) $(CXXFLAGS) -c gui.cc

graphic.o: graphic.cc graphic.h graphic_gui.h
	@echo "Compiling graphic.cc..."
	@$(CXX) $(CXXFLAGS) -c graphic.cc

# Règle pour nettoyer
clean:
	@echo "Cleaning up..."
	@rm -f $(OFILES) $(TARGET)