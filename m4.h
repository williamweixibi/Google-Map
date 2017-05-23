/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4.h
 * Author: zhousha3
 *
 * Created on March 18, 2016, 3:33 PM
 */

#ifndef M4_H
#define M4_H

struct DeliveryInfo {
    //Specifies a delivery order.
    //
    //To satisfy the order the item-to-be-delivered must have been picked-up 
    //from the pickUp intersection before visiting the dropOff intersection.

    DeliveryInfo(unsigned pick_up, unsigned drop_off)
    : pickUp(pick_up), dropOff(drop_off) {
    }


    //The intersection id where the item-to-be-delivered is picked-up.
    unsigned pickUp;

    //The intersection id where the item-to-be-delivered is dropped-off.
    unsigned dropOff;
};

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, const std::vector<unsigned>& depots);

#endif /* M4_H */
