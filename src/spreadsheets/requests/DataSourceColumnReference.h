#ifndef DATASOURCE_COLUMN_REFERENCE_H
#define DATASOURCE_COLUMN_REFERENCE_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"

namespace GSHEET
{

    /**
     * An unique identifier that references a data source column.
     */
    class DataSourceColumnReference : public Printable
    {
    private:
        String buf;
        GSheetJSONUtil jut;

    public:
        DataSourceColumnReference() {}
        // The display name of the column. It should be unique within a data source.
        DataSourceColumnReference &name(const String &value)
        {
            clear();
            jut.addObject(buf, "name", value, true, true);
            return *this;
        }
        const char *c_str() const { return buf.c_str(); }
        size_t printTo(Print &p) const { return p.print(buf.c_str()); }
        void clear() { buf.remove(0, buf.length()); }
    };

}
#endif