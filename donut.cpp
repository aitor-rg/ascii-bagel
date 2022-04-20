#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <stdio.h>

//--------------------------------------------------------------
int main(){
    int w = 40; //width screen
    int h = 22; //height screen
    int R = 9; //big radius
    int r = 4; //small radius
    int buffer_size = w*h;
    float A = 0; //rotation angle X
    float B = 0; //rotation angle Y
    //float C = 0; //rotation angle Z
    float bdir[3] = {0.0,1.0,0.0}; //brightness direction
    char* bchar = ".,-~:;=!*#$@";

    printf("\x1b[2J\x1b[H");
    for (;;){
        A += 0.01;
        B += 0.007;
        float cx = cos(A);
        float sx = sin(A);
        float cy = cos(B);
        float sy = sin(B);
        float z_buffer[buffer_size];
        char donut[buffer_size];
        std::memset(z_buffer, -R-r, buffer_size*sizeof(float));
        std::memset(donut, ' ', buffer_size);
        for (float theta=0; theta<2*M_PI; theta+=0.05){
            for (float phi=0; phi<2*M_PI; phi+=0.05){
                float cTheta = cos(theta);
                float sTheta = sin(theta);
                float cPhi = cos(phi);
                float sPhi = sin(phi);
                float priorx = (R + r*cPhi)*cTheta;
                float priory = (R + r*cPhi)*sTheta;
                float priorz = r*sPhi;
                int x = cy*priorx + sy*priorz;
                int y = -sx*sy*priorx + cx*priory + sx*cy*priorz;
                int z = -cx*sy*priorx - sx*priory + cx*cy*priorz;
                int index = (x+w/2) + w*(y+h/2);
                if ((x+w/2)>0 && (x+w/2)<w && (y+h/2)>0 && (y+h/2)<h && z>=z_buffer[index]){
                    float nx = x - cy*cTheta*R;
                    float ny = y + sx*sy*cTheta*R - cx*sTheta*R;
                    float nz = z + cx*sy*cTheta*R + sx*sTheta*R;
                    float n_norm = sqrt(nx*nx +ny*ny + nz*nz);
                    nx = nx/n_norm;
                    ny = ny/n_norm;
                    nz = nz/n_norm;
                    int bright = 12*(1 + nx*bdir[0] + ny*bdir[1] + nz*bdir[2])/2;
                    donut[index] = bchar[bright];
                    z_buffer[index] = z;
                }
            }
        }
        for (int i = 0; i<buffer_size; i++) {
            if (i%w == 0 && i!=0) { putchar('\n'); }
            putchar(donut[i]);
        }
        printf("\x1b[2J\x1b[H");
    }
    return 0;
}

