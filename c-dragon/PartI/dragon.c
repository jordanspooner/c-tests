#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
  switch (total_iterations % 8)
	{
  	case 0: return (vector_t) {1, 0};
		case 1: return (vector_t) {1, 1};
		case 2: return (vector_t) {0, 1};
		case 3: return (vector_t) {-1, 1};
		case 4: return (vector_t) {-1, 0};
		case 5: return (vector_t) {-1, -1};
		case 6: return (vector_t) {0, -1};
		case 7: return (vector_t) {1, -1};
		default: assert(false);
  }
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst, long x, long y)
{
	int level = LEVEL * drawn_pixels / (dst->height * dst->height);
	uint8_t value;
	switch (level)
	{
		case 0: value = 100; break;
		case 1: value = 120; break;
		case 2: value = 150; break;
		case 3: value = 180; break;
		case 4: value = 200; break;
		default: value = 255;
	}
	set_pixel(dst, x, y, value);
}

/* 45 degrees rotation.
 */
static void rotate_clockwise(void) {
	if (direction.dx == 1 && (direction.dy == 0 || direction.dy == -1))
	{
		direction.dy += 1;
	}
	else if (direction.dy == 1 && (direction.dx == 0 || direction.dx == 1))
	{
		direction.dx -= 1;
	}
	else if (direction.dx == -1 && (direction.dy == 0 || direction.dy == 1))
	{
		direction.dy -= 1;
	}
	else if (direction.dy == -1 && (direction.dx == 0 || direction.dx == -1))
	{
		direction.dx += 1;
	}
}

/* 45 degrees rotation.
 */
static void rotate_anticlockwise(void) {
	if (direction.dx == 1 && (direction.dy == 0 || direction.dy == 1))
	{
		direction.dy -= 1;
	}
	else if (direction.dy == 1 && (direction.dx == 0 || direction.dx == -1))
	{
		direction.dx += 1;
	}
	else if (direction.dx == -1 && (direction.dy == 0 || direction.dy == -1))
	{
		direction.dy += 1;
	}
	else if (direction.dy == -1 && (direction.dx == 0 || direction.dx == 1))
	{
		direction.dx -= 1;
	}
}

/* Iterates though the characters of str, recusively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
	if (iterations >= 0 && str[0] != '\0') {
	  switch (str[0]) {
			case '-':
			{
				rotate_clockwise();
				rotate_clockwise();
				string_iteration(dst, str + 1, iterations);
				break;
			}
			case '+':
			{
				rotate_anticlockwise();
				rotate_anticlockwise();
				string_iteration(dst, str + 1, iterations);
				break;
			}
			case 'F':
			{
				drawn_pixels++;
				draw_greyscale(dst, x / scale, y / scale);
				x += direction.dx;
				y += direction.dy;
				string_iteration(dst, str + 1, iterations);
				break;
			}
			case 'X':
			{
				string_iteration(dst, "X+YF", iterations - 1);
				string_iteration(dst, str + 1, iterations);
				break;
			}
			case 'Y':
			{
				string_iteration(dst, "FX-Y", iterations - 1);
				string_iteration(dst, str + 1, iterations);
				break;
			}
		}
	}
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
	image_t **dst = malloc(sizeof(image_t *));
	image_error_t res = init_image(dst, size * 1.5, size, 1, 255);
	if (res != IMG_OK) {
		image_print_error(res);
		exit(EXIT_FAILURE);
	}
	x = size;
	y = size;
	scale = 2;
	direction = starting_direction(total_iterations);
	string_iteration(*dst, "FX+FX+", total_iterations);
	image_write("twindragon.pgm", *dst, PGM_FORMAT);
	if (res != IMG_OK) {
		image_print_error(res);
		exit(EXIT_FAILURE);
	}
	image_free(*dst);
	free(dst);
}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
	assert(argc >= 1);
	int iterations = atoi(argv[1]);
	dragon(pow(2, iterations), 2 * iterations);
	return EXIT_SUCCESS;
}
