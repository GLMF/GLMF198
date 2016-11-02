/************************************************
Programme de Master Mind
Christophe BORELLY - 2014-10-08 - IUT de Beziers
************************************************/
#include "lib-mastermind.h"

///////////////////////////////////////////////////////////////////////////////
// Renvoi TRUE si le code est trouve et FALSE sinon
// Utilise les variables globales code et combinaison
// Modifie les variables globales nbBienPlace, nbMalPlace et nbCoups
///////////////////////////////////////////////////////////////////////////////
gboolean checkCombinaison() {
  nbCoups++;
  nbBienPlace=0;
  nbMalPlace=0;
  if (debug) {
    printf("COUP=%d\n",nbCoups);
    printf("CODE=");printArray(code);
    printf("COMB=");printArray(combinaison);
  }
  return FALSE;
}
