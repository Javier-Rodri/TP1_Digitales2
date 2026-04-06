/*
 * cars.c
 *
 *      Author: Fede
 */

#include "cars.h"

int count_of_cars;

extern void count_of_cars_increase(void) {
	count_of_cars++;
}

extern void count_of_cars_decrease(void) {
	count_of_cars--;
}

extern void count_of_cars_reset(void) {
	count_of_cars = 0;
}

extern int count_of_cars_get(void) {
	return count_of_cars;
}






