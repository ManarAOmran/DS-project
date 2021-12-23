/*
 * XML_To_JASON.h
 *
 *  Created on: Dec 15, 2021
 *      Author: Kyrillos
 */

#ifndef XML_TO_JSON_H_
#define XML_TO_JSON_H_

#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>
#include "types.h"
#include "Node.h"

int XML_TO_JSON(Node *root, int level, std::string *input, std::vector<Node *> children_data_arr = std::vector<Node *>());




#endif /* XML_TO_JSON_H_ */
