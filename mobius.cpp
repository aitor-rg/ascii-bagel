#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <stdio.h>

//--------------------------------------------------------------
int main(){
    int w = 35; //width screen
    int h = 30; //height screen
    int buffer_size = w*h; //number of pixels
    int R = 10; //big radius
    int rc = 7; //small radius
    float A = 0; //rotation angle X
    float B = 0; //rotation angle Y
    float bdir[3] = {0.0,0.0,1.0}; //brightness direction
    char* bchar = ".,-~:;=!*#$@";
    printf("\x1b[2J\x1b[H");
    for (;;){
        A += 0.09;
        B += 0.1;
        float cx = cos(A);
        float sx = sin(A);
        float cy = cos(B);
        float sy = sin(B);
        float z_buffer[buffer_size];
        char mobius[buffer_size];
        std::memset(z_buffer, -R-rc, buffer_size*sizeof(float));
        std::memset(mobius, ' ', buffer_size);
        for (float theta=0; theta<2*M_PI; theta+=0.007){
            for (float r=-rc; r<=rc; r+=0.01){

                float cTheta = cos(theta);
                float sTheta = sin(theta);
                float priorx = (R + r*cTheta)*cTheta;
                float priory = (R + r*cTheta)*sTheta;
                float priorz = r*sTheta;
                int x = cy*priorx + sy*priorz;
                int y = -sx*sy*priorx + cx*priory + sx*cy*priorz;
                int z = -cx*sy*priorx - sx*priory + cx*cy*priorz;
                int index = (x+w/2) + w*(y+h/2);
                if ((x+w/2)>0 && (x+w/2)<w && (y+h/2)>0 && (y+h/2)<h && z>=z_buffer[index]){
                    //compute normals
                    float nx, ny, nz;
                    if (r == -rc){ // just need to compute once for each theta
                        //normals of one side prior to rotation
                        priorx = -r*cTheta*cTheta;
                        priory = -r*cTheta*sTheta;
                        priorz = r*cTheta;
                        nz = -1.0;
                        while (nz<0){ //check which side is facing up after rotation
                            priorx = priorx*(-1);
                            priory = priory*(-1);
                            priorz = priorz*(-1);
                            nz = -cx*sy*priorx - sx*priory + cx*cy*priorz;
                        }
                        //for that side we compute the rest of the normal components
                        nx = cy*priorx + sy*priorz;
                        ny = -sx*sy*priorx + cx*priory + sx*cy*priorz;
                        float n_norm = sqrt(nx*nx +ny*ny + nz*nz);
                        nx = nx/n_norm;
                        ny = ny/n_norm;
                        nz = nz/n_norm;
                    }
                    int bright = 12*(1 + nx*bdir[0] + ny*bdir[1] + nz*bdir[2])/2;
                    z_buffer[index] = z;
                    mobius[index] = bchar[bright];
                }
            }
        }
        //display möbius strip
        for (int i = 0; i<buffer_size; i++) {
            if (i%w == 0 && i!=0) { putchar('\n'); }
            putchar(mobius[i]);
        }
        printf("\x1b[2J\x1b[H");
    }
    return 0;
}

