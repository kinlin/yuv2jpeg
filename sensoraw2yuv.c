/*
Image pipeline:
sensor raw -->
Debayer -->
3x3 matrix -->      (rgb)
Gamma correct -->   (r'g'b')
3x3 matrix -->   (Y'CbCr)
chroma subsampling  -->   (YUV)
*/

/*
step 1: load raw picture
step 2: 
*/
#include <stdio.h>

void (*load_raw)();

main(){
}
