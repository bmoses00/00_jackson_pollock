#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 500

int fd;

int init_image();
void write_to_file(int rgb[]);

int min(int a, int b);
int max(int a, int b);

int draw_line_segment(int current_x, int current_y,
                      int start_x, int start_y,
                      int end_x, int end_y,
                      int thickness, int red, int green, int blue);

int draw_rectangle(int current_x, int current_y,
                   int lower_x, int upper_x,
                   int lower_y, int upper_y,
                   int red, int green, int blue);

int draw_circle(int current_x, int current_y,
                int center_x, int center_y, int radius,
                int thickness, int red, int green, int blue);

int draw_solid_circle(int current_x, int current_y,
                      int center_x, int center_y, int radius,
                      int red, int green, int blue);


int main() {
    int x, y;
    int rgb[3];

    fd = init_image();

    for (y = 0; y < IMAGE_HEIGHT; y++) {

        for (x = 0; x < IMAGE_WIDTH; x++) {
            if (
                // dirt and grass
                !draw_rectangle(x, y, -1, 500, 350, 500, 155, 118, 83)
             && !draw_rectangle(x, y, -1, 500, 342, 351, 124, 252, 0)

             // legs and body
             && !draw_line_segment(x, y, 200, 342, 231, 299, 4, 0, 0, 0)
             && !draw_line_segment(x, y, 260, 342, 229, 301, 4, 0, 0, 0)

             && !draw_line_segment(x, y, 230, 305, 230, 240, 2, 0, 0, 0)

             // face
             && !draw_circle(x, y, 230, 210, 30, 125, 0, 0, 0)
             && !draw_solid_circle(x, y, 230, 210, 30, 255, 255, 255)

             // arms
             && !draw_line_segment(x, y, 232, 270, 198, 255, 2, 0, 0, 0)
             && !draw_line_segment(x, y, 228, 270, 260, 255, 2, 0, 0, 0)

             // sky
             && !draw_rectangle(x, y, -1, 500, 0, 343, 135, 206, 250)
         ) {
            rgb[0] = 0;
            rgb[1] = 0;
            rgb[2] = 0;
            write_to_file(rgb);
            }
        }
    }
    close(fd);
}

int init_image() {
    char buffer[100];
    int fd = open("image.ppm", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    sprintf(buffer, "P3 %d %d 255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
    write(fd, buffer, strlen(buffer));
    return fd;
}

void write_to_file(int rgb[]) {
    char buffer[100];
    sprintf(buffer, "%d %d %d ", rgb[0], rgb[1], rgb[2]);
    write(fd, buffer, strlen(buffer));
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int draw_line_segment(int current_x, int current_y,
                      int start_x, int start_y,
                      int end_x, int end_y,
                      int thickness, int red, int green, int blue) {
    int rgb[3];

    // if line is vertical we cannot find the slope
    if (start_x == end_x) {
        if (
            abs(current_x - start_x) < thickness
         && current_y > min(start_y, end_y)
         && current_y < max(start_y, end_y)
     ) {
            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            write_to_file(rgb);
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        double m = (double)(end_y - start_y) / (end_x - start_x);
        if (
            (abs(
                current_y - m * current_x - start_y + m * start_x
            ) < thickness)
         && current_x > min(start_x, end_x)
         && current_x < max(start_x, end_x)
         && current_y > min(start_y, end_y)
         && current_y < max(start_y, end_y)
        ) {
            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            write_to_file(rgb);
            return 1;
        }
        else {
            return 0;
        }
    }
}

int draw_rectangle(int current_x, int current_y,
                   int lower_x, int upper_x,
                   int lower_y, int upper_y,
                   int red, int green, int blue) {
    int rgb[3];
    if (current_x > lower_x && current_x < upper_x
        && current_y > lower_y && current_y < upper_y) {
            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            write_to_file(rgb);
            return 1;
    }
    else {
        return 0;
    }
}

int draw_circle(int current_x, int current_y,
                int center_x, int center_y, int radius,
                int thickness, int red, int green, int blue) {
    int rgb[3];
    if (
        abs(
            pow(current_x - center_x, 2)
            + pow(current_y - center_y, 2)
            - pow(radius, 2)
        ) < thickness
    ) {
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;
        write_to_file(rgb);
        return 1;
    }
    else {
        return 0;
    }
}

int draw_solid_circle(int current_x, int current_y,
                      int center_x, int center_y, int radius,
                      int red, int green, int blue) {
    int rgb[3];
    if (pow(current_x - center_x, 2) + pow(current_y - center_y, 2) < pow(radius, 2)) {
        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;
        write_to_file(rgb);
        return 1;
    }
    else {
        return 0;
    }
}
