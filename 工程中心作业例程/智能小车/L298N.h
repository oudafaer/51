#ifndef __L298N_H
#define __L298N_H


void stop(); // 停
void forward(); // 向前
void back(); // 向后
void left(); // 向左，只有左轮动
void avoid_left();//向左，左右伦同时转动
void right(); // 向右，只有右轮动
void avoid_right();//向左，左右伦同时转动
void increase(void);//加速
void reduce(void);//减速
void avoid(void);//避障模式


#endif
