//
// Created by nunol on 04/03/2020.
//

#include <cstdlib>
#include <tls.h>

int main(int argc, char **argv) {

    auto tls = tls_client();

    tls_free(tls);

    return EXIT_SUCCESS;
}