#ifndef GRID_RANGE_H
#define GRID_RANGE_H
#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/Common.h"

namespace GSHEET
{
    /**
     * A range on a sheet. All indexes are zero-based. Indexes are half open, i.e. the start index is inclusive and the end index is exclusive -- [startIndex, endIndex). Missing indexes indicate the range is unbounded on that side.
     */
    class GridRange : public O6
    {

    public:
        GridRange() {}
        // The sheet this range is on.
        GridRange &sheetId(int value) { return wr.set<GridRange &, int>(*this, value, buf, bufSize, buf[1], FPSTR(__func__)); }
        // The start row (inclusive) of the range, or not set if unbounded.
        GridRange &startRowIndex(int value) { return wr.set<GridRange &, int>(*this, value, buf, bufSize, buf[2], FPSTR(__func__)); }
        // The end row (exclusive) of the range, or not set if unbounded.
        GridRange &endRowIndex(int value) { return wr.set<GridRange &, int>(*this, value, buf, bufSize, buf[3], FPSTR(__func__)); }
        // The start column (inclusive) of the range, or not set if unbounded.
        GridRange &startColumnIndex(int value) { return wr.set<GridRange &, int>(*this, value, buf, bufSize, buf[4], FPSTR(__func__)); }
        // The end column (exclusive) of the range, or not set if unbounded.
        GridRange &endColumnIndex(int value) { return wr.set<GridRange &, int>(*this, value, buf, bufSize, buf[5], FPSTR(__func__)); }
    };
}

#endif