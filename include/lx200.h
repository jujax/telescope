#ifndef LX200_H
#define LX200_H

#include <Arduino.h>

/**
 * @brief Classe LX200
 *
 * Cette classe implémente la réception et le traitement des commandes
 * LX200 envoyées via une interface série. Elle permet par exemple de recevoir
 * des commandes pour définir l'ascension droite (RA) et la déclinaison (DEC),
 * ou de renvoyer les coordonnées actuelles.
 */
class LX200 {
public:
  /**
   * @brief Constructeur
   * @param serial Référence à l'objet Stream (ex : Serial, Serial2, etc.)
   */
  LX200(Stream &serial) : serialInterface(serial) {}

  /**
   * @brief Appeler cette fonction dans loop() pour traiter les commandes entrantes.
   */
  void processIncomingCommands() {
    // Lecture des données disponibles dans le buffer
    while (serialInterface.available()) {
      char c = serialInterface.read();
      if (c == '#') { // Fin de commande
        // Traite la commande complète stockée dans commandBuffer
        processCommand(commandBuffer);
        commandBuffer = ""; // Réinitialise le buffer
      } else {
        commandBuffer += c;
      }
    }
  }

  /**
   * @brief Définit les coordonnées internes (simulées) du télescope.
   * @param ra Chaîne représentant l'ascension droite (format "HH:MM:SS")
   * @param dec Chaîne représentant la déclinaison (format "+DD*MM:SS" ou "-DD*MM:SS")
   */
  void setCoordinates(const String &ra, const String &dec) {
    currentRA = ra;
    currentDEC = dec;
  }

  /**
   * @brief Optionnel : met à jour les coordonnées (par exemple, depuis des capteurs)
   */
  void updateCoordinates() {
    // Cette fonction pourrait être complétée pour lire les valeurs réelles
    // des encodeurs ou d'autres capteurs positionnels.
  }

private:
  Stream &serialInterface;
  String commandBuffer = "";
  String currentRA  = "00:00:00";   // Ascension droite par défaut
  String currentDEC = "+00*00:00";  // Déclinaison par défaut

  /**
   * @brief Traite une commande LX200 reçue.
   * @param command La commande (sans le caractère '#' final)
   */
  void processCommand(const String &command) {
    // Exemple de traitement de quelques commandes LX200 classiques :

    // Commande "Sr" : définir l'ascension droite
    if (command.startsWith("Sr")) {
      String ra = command.substring(2);  // Récupère la valeur après "Sr"
      currentRA = ra;
      // Renvoyer une confirmation (souvent "1" est renvoyé par certains télescopes)
      serialInterface.print("1");
    }
    // Commande "Sd" : définir la déclinaison
    else if (command.startsWith("Sd")) {
      String dec = command.substring(2);
      currentDEC = dec;
      serialInterface.print("1");
    }
    // Commande "GR" : renvoyer l'ascension droite actuelle
    else if (command.equals("GR")) {
      serialInterface.print(currentRA);
    }
    // Commande "GD" : renvoyer la déclinaison actuelle
    else if (command.equals("GD")) {
      serialInterface.print(currentDEC);
    }
    // Commande "MS" : démarrer le déplacement vers les coordonnées définies
    else if (command.equals("MS")) {
      // Ici, vous lanceriez le processus de déplacement (pilotage des moteurs, etc.)
      // On simule une réponse "0" indiquant que le mouvement démarre.
      serialInterface.print("0");
    }
    // Commande "Q" : arrêter le déplacement en cours
    else if (command.equals("Q")) {
      // Arrêter les moteurs, par exemple.
      // Optionnel : renvoyer une réponse ou rien.
    }
    // Commande "Gg" : renvoyer le nom de l'objet courant (si applicable)
    else if (command.equals("Gg")) {
      serialInterface.print("No Object");
    }
    // D'autres commandes peuvent être ajoutées ici en fonction de vos besoins.
  }
};

#endif // LX200_H
