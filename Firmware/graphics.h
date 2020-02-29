/*
*  Copyright [2020] [Hans Jørgen Grimstad]
*  
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*  
*      http://www.apache.org/licenses/LICENSE-2.0
*  
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

void RightArrowAnimation();
void RightArrowBump();
void RenderLineSensorResponse(int nw, int ne, int sw, int se);
void RenderRangeSensorResponse(int leftRange, int centerRange, int RightRange);
void DisplayFail();
void DisplayReady();

#endif // _GRAPHICS_H_