// TEA Tiny Encryption Algorithm in "CBC mode" C Implementation by Sam Muldrow.
// 3-24-23
// UNH CS727
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char *keyFileName = NULL;
char *plainTextFileName = NULL;
char *cipherTextFileName = NULL;
char *ivFileName = NULL;

FILE *keyFile = NULL;
FILE *plainTextFile = NULL;
FILE *cipherTextFile = NULL;
FILE *ivFile = NULL;

// TEA decode
void decode(uint32_t v[2], const uint32_t k[4]) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up; sum is (delta << 5) & 0xFFFFFFFF */
    uint32_t delta=0x9E3779B9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<32; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}


int main(int argc, char** argv){

    // Set file pointers and report errors
    // Initial value
    ivFileName = argv[1];
    printf("-- IV file name: %s\n", ivFileName);
    ivFile = fopen(ivFileName, "r");
    if(!ivFile) {
        printf("--- Could not open file %s \n", ivFileName);
        return -1;
    }

    // Key File
    keyFileName = argv[2];
    printf("-- Key file name: %s\n", keyFileName);
    keyFile = fopen(keyFileName, "r");
    if(!keyFile) {
        printf("--- Could not open file %s \n", keyFileName);
        return -1;
    }


    // Plain Text File 
    plainTextFileName = argv[3];
    printf("-- Plain text file name: %s\n", plainTextFileName);
    plainTextFile = fopen(plainTextFileName, "rb");
    if(!plainTextFileName) {
        printf("--- Could not open file %s \n", plainTextFileName);
        return -1;
    }

    // Cipher Text File
    cipherTextFileName = argv[4];
    printf("-- Cipher text file name: %s\n", cipherTextFileName);
    cipherTextFile = fopen(cipherTextFileName, "w");
    if(!cipherTextFileName) {
        printf("--- Could not open file %s \n", cipherTextFileName);
        return -1;
    }

    printf("- Initialized files: %s, %s, %s \n", keyFileName, plainTextFileName, cipherTextFileName);
    
    // Buffer to be used to store 2 32 bit blocks
    uint32_t *v = (uint32_t*)calloc(2, sizeof(uint32_t));
    // Buffer to store IV value and feedback value
    uint32_t *v2 = (uint32_t*)calloc(2, sizeof(uint32_t));
    // Buffer to store ciphertext of previous block
    uint32_t *v3 = (uint32_t*)calloc(2, sizeof(uint32_t));
    // Buffer to store 128 bit key
    uint32_t *k = (uint32_t*)calloc(4, sizeof(uint32_t));

    // Initialize buffers to null
    v[0] = 0; v[1] = 0; v2[0] = 0; v2[1] = 0; v3[0] = 0;
    v3[1] = 0; k[0] = 0; k[1] = 0; k[2] = 0; k[3] = 0;

    // Load key value into buffer k
    fread(k, sizeof(*k), 4, keyFile);
    //Load the IV value into buffer v2
    fread(v2, sizeof(*v2), 2, ivFile);
    
    // Iterate over plaintext
    int blockNum = 0;
    printf("Starting decode \n");
    while(fread(v, sizeof(*v), 2, plainTextFile) > 0) {
        // Inc block #
        printf("Encoding block #: %d \n", blockNum++);
        // Store ciphertext cN into temp variable
        v3[0] = v[0]; v3[1] = v[1];
        decode(v,k);
        // XOR IV or feedback with cipher text
        v[0] ^= v2[0];
        v[1] ^= v2[1];
        // Write to plaintext file
        fwrite(v, sizeof(v[0]), 2, cipherTextFile);
        // Set feedback value and reset buffer v to null
        v2[0] = v3[0]; v2[1] = v3[1];
        v[0] = 0; v[1] = 0;
    }

    printf("Decoding complete. See: %s \n", cipherTextFileName);
    // Close files
    fclose(cipherTextFile);
    fclose(plainTextFile);
    fclose(keyFile);
    return 0;
}