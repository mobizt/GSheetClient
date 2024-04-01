/**
 * Created March 26, 2024
 *
 * The MIT License (MIT)
 * Copyright (c) 2024 K. Suwatchai (Mobizt)
 *
 *
 * Permission is hereby granted, free of charge, to any person returning a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GSHEET_CORE_LIST_H
#define GSHEET_CORE_LIST_H
#include <Arduino.h>
#include <vector>

namespace gsheet
{
    class GSheetList
    {
    private:
    public:
        GSheetList(){};
        ~GSheetList(){};

        void addRemoveList(std::vector<uint32_t> &vec, uint32_t addr, bool add)
        {
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec[i] == addr)
                {
                    if (add)
                        return;
                    else
                        vec.erase(vec.begin() + i);
                }
            }
            if (add)
                vec.push_back(addr);
        }

        bool existed(std::vector<uint32_t> &vec, uint32_t addr)
        {
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec[i] == addr)
                    return true;
            }
            return false;
        }
    };
};

#endif
