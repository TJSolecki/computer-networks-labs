// Name: Thomas Solecki
// Date: 4/10/2024
// Title: Lab1 – Steps 2 and 3
// Description: This program computes a number of probabilities relating to
// circit and packet switching

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double factorial(double n) {
    if (n == 0 || n == 1) // base case
        return 1.0;
    else
        return n * factorial(n - 1.0);
}
double choose(double n, double r) {
    return factorial(n) / (factorial(r) * factorial(n - r));
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr,
                "usage: %s <linkBandwidth> <userBandwidth> <tPSusers> "
                "<nPSusers> \n",
                argv[0]);
        exit(0);
    }
    int i, linkBandwidth, userBandwidth, nCSusers, nPSusers;
    double pPSusers, tPSusers, pPSusersBusy, pPSusersNotBusy;

    // Get values from command line
    linkBandwidth = atoi(argv[1]);
    userBandwidth = atoi(argv[2]);
    tPSusers = atof(argv[3]);
    nPSusers = atoi(argv[4]);

    // 2a: Circuit Switching Senario
    // how many users --> nCSusers
    nCSusers = linkBandwidth / userBandwidth;
    printf("2a: Circuit switching senario: %d users\n", nCSusers);

    // 3: Packet Switching Senario
    // compute: 3a, b, c, d, e, f, g, h
    pPSusers = tPSusers;                                     // a
    pPSusersNotBusy = 1 - pPSusers;                          // b
    double pAllNotBusy = pow(pPSusersNotBusy, nPSusers - 1); // c
    double pOneBusy = pPSusers * pAllNotBusy;                // d
    double pExactlyOne = nPSusers * pOneBusy;                // e
    double pTenBusy =
        pow(pPSusers, 10) * pow(pPSusersNotBusy, nPSusers - 10); // f
    double pAnyTenBusy = choose(nPSusers, 10.0) * pTenBusy;      // g
    double pTenMoreBusy = 0;                                     // h
    for (int i = 11; i <= nPSusers; i++) {
        pTenMoreBusy += choose((double)nPSusers, (double)i) * pow(pPSusers, i) *
                        pow(pPSusersNotBusy, nPSusers - i);
    }

    printf("%lg\n", factorial(10.0));
    printf("3: Packet switching senario");
    printf("3a: The probability that a given (specific) user is busy "
           "transmitting = %f\n",
           pPSusers);
    printf(
        "3b: The probability that ine specific other user is not busy =  %f\n",
        pPSusersNotBusy);
    printf("3c: The probablility that all of the other specific other users "
           "are not busy =  %e\n",
           pAllNotBusy);
    printf("3d: The probability that one specific user is transmitting and the "
           "remianing users are not transmitting =  %e\n",
           pOneBusy);
    printf("3e: The probability that exactly one of the nPSusers users is busy "
           "is pPSusers times the probability that a given specific user is "
           "transmitting and the remaining users are not transmitting = %le\n",
           pExactlyOne);
    printf("3f: The probabalitiy that 10 specific users of nPSusers are "
           "transmitting and the others are idle = %lg\n",
           pTenBusy);
    printf("3g: The probability that any 10 users of nPSusers are transmitting "
           "and the others are idle = %lg\n",
           pAnyTenBusy);
    printf("3h: The probability that more than 10 users of nPSusers are "
           "transmitting and the others are idle = %lg\n",
           pTenMoreBusy);
    return 0;
}
