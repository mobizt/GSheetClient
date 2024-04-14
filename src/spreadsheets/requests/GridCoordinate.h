#ifndef GRID_COORDINATE_H
#define GRID_COORDINATE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{

    /**
     * A coordinate in a sheet. All indexes are zero-based.
     */
    class GridCoordinate : public BaseG4
    {
    public:
        GridCoordinate() = default;

        // The sheet this coordinate is on.
        GridCoordinate &sheetId(int value) { return wr.set<GridCoordinate &, int>(*this, value, buf, bufSize, 1, FPSTR(__func__)); }

        // The row index of the coordinate.
        GridCoordinate &rowIndex(int value) { return wr.set<GridCoordinate &, int>(*this, value, buf, bufSize, 2, FPSTR(__func__)); }

        // The column index of the coordinate.
        GridCoordinate &columnIndex(int value) { return wr.set<GridCoordinate &, int>(*this, value, buf, bufSize, 3, FPSTR(__func__)); }
    };

}
#endif