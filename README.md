# Tiny Encryption Algorithm Implementation In C!

https://www.tayloredge.com/reference/Mathematics/TEA-XTEA.pdf

## How to use:
Run makefile `make cbc`

Encrypt file: `./tea-cbc-enc iv key plaintext ciphertext`

Decrypt file: `./tea-cbc-dec iv key ciphertext plaintext`

### Please note:
- The parameters are file names
- iv should be a 64 bit value
- key should be a 128 bit key
