# Choix du compilateur
CXX = g++

# Options de compilation : 
# -std=c++11 : Standard utilisé (courant à l'EPFL)
# -Wall : Affiche tous les avertissements (essentiel pour un code propre)
CXXFLAGS = -std=c++11 -Wall

# Liste des fichiers objets (les .o qui seront créés à partir des .cc)
# Pour le Rendu 1, on inclut : project, game, message, brick, ball, paddle et tools
OFILES = project.o game.o message.o brick.o ball.o paddle.o tools.o

# Règle principale : crée l'exécutable 'project' [cite: 16, 389]
project: $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o project

# Règle pour tools.o (dépend de tools.cc et tools.h)
tools.o: tools.cc tools.h
	$(CXX) $(CXXFLAGS) -c tools.cc

# Règles pour les autres modules (à compléter au fur et à mesure)
game.o: game.cc game.h tools.h message.h
	$(CXX) $(CXXFLAGS) -c game.cc

project.o: project.cc game.h
	$(CXX) $(CXXFLAGS) -c project.cc

# Règle pour compiler le module message (fourni par l'école)
message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c message.cc

# Règle 'clean' pour nettoyer les fichiers temporaires
clean:
	rm -f *.o project