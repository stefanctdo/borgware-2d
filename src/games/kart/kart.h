/*
 * kart.h
 *
 *  Created on: 07.02.2014
 *      Author: Stefan Kinzel
 */

#ifndef KART_H_
#define KART_H_

void kart_game(void);

void drive(void);
void save_street(uint8_t middle, uint8_t width, uint8_t obstacle_pos);
uint8_t check_collision(uint8_t carpos);
void blink();

#endif /* KART_H_ */
