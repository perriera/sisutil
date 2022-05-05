/*
 * ValBase.cpp
 *
 *  Created on: Mar 13, 2018
 *      Author: ojones
 */

#include <sisutil/ValBase.hpp>

void TABSET_Index::Clear() { return; }
void TABSET_Index::Sort() { return; }
void TABSET_Index::Insert(Index_Element*) { return; }

TabSet* TabSet::clone() { return this; }  // TODO: fix this to actually do a clone
