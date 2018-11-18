#include <stdio.h>
#include <pam.h>

int main(int argc, char **argv) {
    pm_init(argv[0], 0);
    struct pam inpam, outpam;

    pnm_readpaminit(stdin, &inpam, PAM_STRUCT_SIZE(PAM_PPM_TUPLETYPE));
    
    return 0;
}
