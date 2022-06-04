#include "ilg_kineis.c"
#include "ilg_kineis.h"
#include "constantes.h"

int main(void) {

    if (initModuleKineis() == 0) {

        w_kineis(REQUEST_SERIAL_NUMBER);

        w_kineis(REQUEST_SET_POWER);
        w_kineis(REQUEST_SET_BAND);

        for (int x = 0; x < 2000; x++) { w_kineis(REQUEST_TX_DATA); sleep(60);}

        closeKineis();

        return EXIT_SUCCESS;

    }else { return EXIT_FAILURE; }
}
